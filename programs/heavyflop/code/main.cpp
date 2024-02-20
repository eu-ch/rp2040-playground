/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "pico/stdlib.h"

#include "HeavyContextInterface.hpp"
#include "Heavy_flip_led.h"

#define HV_BUF_SIZE 64

float hv_buf[HV_BUF_SIZE];

#define ALARM_NUM 0
#define ALARM_IRQ TIMER_IRQ_0

static volatile uint32_t led_state = 0;

void hv_send_hook(HeavyContextInterface *context, const char *sendName,
                  hv_uint32_t sendHash, const HvMessage *msg) {
  if (sendHash == 0xA2A904DC) {
    printf("sendHash: %s\n", sendName);

    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_put(LED_PIN, led_state ? 1 : 0);
    led_state = !led_state;
  }
}

// Alarm interrupt handler
static volatile bool alarm_fired;

static void alarm_irq(void) {
  // Clear the alarm irq
  hw_clear_bits(&timer_hw->intr, 1u << ALARM_NUM);

  // Assume alarm 0 has fired
  alarm_fired = true;
}

static void alarm_in_us(uint32_t delay_us) {
  // Enable the interrupt for our alarm (the timer outputs 4 alarm irqs)
  hw_set_bits(&timer_hw->inte, 1u << ALARM_NUM);
  // Set irq handler for alarm irq
  irq_set_exclusive_handler(ALARM_IRQ, alarm_irq);
  // Enable the alarm irq
  irq_set_enabled(ALARM_IRQ, true);
  // Enable interrupt in block and at processor

  // Alarm is only 32 bits so if trying to delay more
  // than that need to be careful and keep track of the upper
  // bits
  uint64_t target = timer_hw->timerawl + delay_us;

  // Write the lower 32 bits of the target time to the alarm which
  // will arm it
  timer_hw->alarm[ALARM_NUM] = (uint32_t)target;
}

extern "C" int main() {
#ifndef PICO_DEFAULT_LED_PIN
#warning blink example requires a board with a regular LED
#else
  const uint LED_PIN = PICO_DEFAULT_LED_PIN;
  gpio_init(LED_PIN);
  gpio_set_dir(LED_PIN, GPIO_OUT);
  stdio_init_all();

  HeavyContextInterface *ctx = hv_flip_led_new(48000.f);
  ctx->setSendHook(hv_send_hook);

  while (1) {
    alarm_fired = false;
    alarm_in_us(1000 * 2);
    ctx->processInline(hv_buf, hv_buf, HV_BUF_SIZE);

    while (!alarm_fired)
      ;
  }
#endif
}
