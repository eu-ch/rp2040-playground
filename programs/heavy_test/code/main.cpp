#include <math.h>
#include <stdint.h>
#include <stdio.h>

#include "pico/stdio.h"
#include "pico/stdlib.h"
#include "hardware/vreg.h"

#include "pico/audio.h"
#include "pico/audio_i2s.h"

#include "HeavyContextInterface.hpp"
#include "Heavy_PdPatch.h"
#include "Heavy_PdPatch.hpp"

#define SAMPLE_RATE 48000
#define SAMPLES_PER_BUFFER 128

static float hv_buffer[SAMPLES_PER_BUFFER * 2]; // double for two channels

audio_buffer_pool_t *init_audio()
{
	static audio_format_t audio_format = {
		.sample_freq = SAMPLE_RATE,
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
	
    vreg_set_voltage(VREG_VOLTAGE_1_30);
    sleep_ms(1);
    set_sys_clock_khz(402000, true);

	stdio_init_all();

	const uint LED_PIN = PICO_DEFAULT_LED_PIN;
	gpio_init(LED_PIN);
	gpio_set_dir(LED_PIN, GPIO_OUT);
	gpio_put(LED_PIN, 0);

	for (int32_t i = 0; i < SAMPLES_PER_BUFFER; ++i) {
		hv_buffer[i] = 0.f;
	}

	audio_buffer_pool_t *buffer_pool = init_audio();
	HeavyContextInterface *hv_ctx = hv_PdPatch_new(SAMPLE_RATE);
	if (!buffer_pool || !hv_ctx) {
		return 1;
	}

	gpio_put(LED_PIN, 1);

	while (true) {
		audio_buffer_t *buffer = take_audio_buffer(buffer_pool, true);
		hv_ctx->processInlineInterleaved(hv_buffer, hv_buffer,
						 buffer->max_sample_count / 2);

		int16_t *samples = (int16_t *)buffer->buffer->bytes;
		for (uint32_t i = 0; i < buffer->max_sample_count; ++i) {
			const float v = hv_buffer[i] * 0.35f; // scale to avoid clipping
			samples[i] = (int16_t)(INT16_MAX * v);
		}

		// signal gets distorted if max_sample_count not divided by 2, dunno why
		buffer->sample_count = buffer->max_sample_count / 2;
		give_audio_buffer(buffer_pool, buffer);
	}

	return 0;
}
