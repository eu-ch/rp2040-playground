#include <math.h>
#include <stdio.h>

#include "pico/stdio.h"
#include "pico/stdlib.h"

#include "pico/audio.h"
#include "pico/audio_i2s.h"

#define TABLE_LEN 128
#define SAMPLES_PER_BUFFER 256

static int16_t sine_wave_table[TABLE_LEN];

audio_buffer_pool_t *init_audio()
{
	static audio_format_t audio_format = {
		.sample_freq = 24000,
		.format = AUDIO_BUFFER_FORMAT_PCM_S16,
		.channel_count = 2
	};
	static audio_buffer_format_t producer_format = {
		.format = &audio_format, .sample_stride = 2
	};
	struct audio_i2s_config config = {
		.data_pin = PICO_AUDIO_I2S_DATA_PIN,
		.clock_pin_base = PICO_AUDIO_I2S_CLOCK_PIN_BASE,
		.dma_channel = 0,
		.pio_sm = 0,
	};

	static audio_buffer_pool_t *producer_pool = audio_new_producer_pool(
		&producer_format, 3, SAMPLES_PER_BUFFER);
	const audio_format_t *output_format =
		audio_i2s_setup(&audio_format, &config);

	if (!output_format) {
		return nullptr;
	}
	if (!audio_i2s_connect(producer_pool)) {
		return nullptr;
	}
	audio_i2s_set_enabled(true);

	return producer_pool;
}

extern "C" int main()
{
	const uint LED_PIN = PICO_DEFAULT_LED_PIN;
	gpio_init(LED_PIN);
	gpio_set_dir(LED_PIN, GPIO_OUT);
	gpio_put(LED_PIN, 0);
	stdio_init_all();

	for (int32_t i = 0; i < TABLE_LEN; ++i) {
		sine_wave_table[i] =
			32767 * cosf(i * (2 * M_PI / TABLE_LEN)) * 0.25f;
	}

	audio_buffer_pool_t *buffer_pool = init_audio();
	gpio_put(LED_PIN, buffer_pool ? 1 : 0);

	if (!buffer_pool) {
		return 1;
	}

	printf("sample stride: %d\r", buffer_pool->format->format);

	int32_t pos = 0;
	while (true) {
		audio_buffer_t *buffer = take_audio_buffer(buffer_pool, true);
		int16_t *samples = (int16_t *)buffer->buffer->bytes;
		for (uint32_t i = 0; i < buffer->max_sample_count;) {
			if (pos >= TABLE_LEN) {
				pos = 0;
			}
			samples[i] = sine_wave_table[pos];
			samples[i + 1] = sine_wave_table[pos];
			pos += 1;
			i += 2;
		}
		// signal gets distorted if max_sample_count not divided by 2, dunno why
		buffer->sample_count = buffer->max_sample_count / 2;
		give_audio_buffer(buffer_pool, buffer);
	}

	return 0;
}
