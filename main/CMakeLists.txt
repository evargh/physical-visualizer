idf_component_register(SRCS "physical_visualizer.c"
                    PRIV_REQUIRES esp_driver_i2s esp_driver_gpio esp_driver_uart i2s_examples_common vfs esp_gdbstub fft
                    INCLUDE_DIRS ".")
