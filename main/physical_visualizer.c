/*
 * SPDX-FileCopyrightText: 2021-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Unlicense OR CC0-1.0
 */

#include "fft.h"
#include "pin_config.h"

#include "driver/gpio.h"
#include "driver/i2s_common.h"
#include "driver/i2s_std.h"
#include "driver/uart.h"
#include "esp_err.h"
#include "esp_system.h"
#include "esp_vfs_dev.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"
#include <inttypes.h>
#include <stdint.h>
#include <stdlib.h>

#define AUDIO_SAMPLE_RATE 44100
#define SERIAL_BAUD_RATE 115200
#define AUDIO_BUFF_SIZE 1024
#define REBOOT_WAIT 5000
#define CAPTURE_LENGTH 20
#define DEBUG_MODE false

static i2s_chan_handle_t rx_chan; // I2S rx channel handler

esp_err_t i2s_setup(void) {
  i2s_chan_config_t channel_cfg =
      I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_AUTO, I2S_ROLE_MASTER);
  ESP_ERROR_CHECK(i2s_new_channel(&channel_cfg, NULL, &rx_chan));

  i2s_std_config_t std_cfg = {
      .clk_cfg =
          {.sample_rate_hz = AUDIO_SAMPLE_RATE,
           .clk_src =
               I2S_CLK_SRC_APLL, // APLL is better, but I suspect that
                                 // there's a bug with the signal being
                                 // output--investigating further, but seems to
                                 // work fine under certain circumstances
           .mclk_multiple = I2S_MCLK_MULTIPLE_256,
           .bclk_div = 8}, // the configuration appears finicky with the APLL
      .slot_cfg = I2S_STD_PHILIPS_SLOT_DEFAULT_CONFIG(
          I2S_DATA_BIT_WIDTH_32BIT,
          I2S_SLOT_MODE_MONO), // 32-bit works with the 24-bit PCM, since it
                               // occasionally looks like the device sends
                               // all-zero signals. Need to investigate the
                               // logic of that more, since it could be either
                               // due to startup or it could be due to the
                               // PCM1808 trying to pre-empt the 8-bit padding
      .gpio_cfg =
          {
              // refer to config.h for pin setup
              .mclk = I2S_MCLK_PIN,
              .bclk = I2S_BLK_PIN,
              .ws = I2S_LRC_PIN,
              .dout = I2S_GPIO_UNUSED,
              .din = I2S_IN_PIN,
              .invert_flags =
                  {
                      .mclk_inv = false,
                      .bclk_inv = false,
                      .ws_inv = false,
                  },
          },
  };
  return i2s_channel_init_std_mode(rx_chan, &std_cfg);
}

void init_uart1(void) {
  uart_driver_delete(UART_NUM_0); // My board doesn't give me access to GPIO0,
                                  // so I use GPIO1 and disable UART0
  const uart_port_t uart_num = UART_NUM_1;
  uart_config_t uart_config = {
      .baud_rate = SERIAL_BAUD_RATE,
      .data_bits = UART_DATA_8_BITS,
      .parity = UART_PARITY_DISABLE,
      .stop_bits = UART_STOP_BITS_1,
      .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
      .rx_flow_ctrl_thresh = 122, // magic number, investigate
  };
  // Configure UART parameters
  ESP_ERROR_CHECK(
      uart_driver_install(uart_num, 1024, 1024, 10, NULL,
                          0)); // magic numbers, investigate/substantiate
  ESP_ERROR_CHECK(uart_param_config(uart_num, &uart_config));
  ESP_ERROR_CHECK(uart_set_pin(uart_num, UART1_TX, UART1_RX, -1, -1));

  esp_vfs_dev_uart_use_driver(uart_num);
}

void app_main(void) {
  init_uart1();

  ESP_ERROR_CHECK(i2s_setup());

  ESP_ERROR_CHECK(i2s_channel_enable(rx_chan));

  int32_t *buf = heap_caps_malloc(
      AUDIO_BUFF_SIZE * sizeof(uint32_t),
      MALLOC_CAP_DMA); // https://esp32.com/viewtopic.php?t=45663
  size_t bytes_to_read = AUDIO_BUFF_SIZE * sizeof(uint32_t);
  size_t bytes_read;
  // there's some on-time
  // to collect (duration) amount of samples, we take the number of seconds, multiply that by the sample rate, and then (for this mono channel) divide it by the buffer size
  size_t capture_amount = CAPTURE_LENGTH * AUDIO_SAMPLE_RATE / AUDIO_BUFF_SIZE;
  for (size_t i = 0; i < capture_amount; i++) {
    esp_err_t ret = i2s_channel_read(rx_chan, buf, bytes_to_read, &bytes_read,
                                     portMAX_DELAY);
    if (ret != ESP_OK || bytes_read != bytes_to_read) {
      printf("Read failed or incomplete: got %d bytes\n", (int)bytes_read);
      continue;
    }
#if DEBUG_MODE
    for (size_t j = 0; j < AUDIO_BUFF_SIZE; j++) {
      printf("result: %ld\n", (int32_t)buf[j]);
    }
    fflush(stdout);
#endif
  }
  free(buf);

  // reboot
  vTaskDelay(pdMS_TO_TICKS(REBOOT_WAIT));

  ESP_ERROR_CHECK(i2s_channel_disable(rx_chan));
  ESP_ERROR_CHECK(i2s_del_channel(rx_chan));
  esp_restart();
}
