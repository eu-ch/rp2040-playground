add_executable(i2s_test code/main.cpp )
target_include_directories(i2s_test PRIVATE include)

# pull in common dependencies
target_link_libraries(i2s_test
        pico_stdlib
        pico_audio_i2s
        pico_util_buffer
        )
target_compile_definitions(i2s_test PRIVATE
        PICO_AUDIO_I2S_MONO_OUTPUT=0
        PICO_AUDIO_I2S_DATA_PIN=10
        PICO_AUDIO_I2S_CLOCK_PIN_BASE=11
        )

pico_add_extra_outputs(i2s_test)
pico_enable_stdio_usb(i2s_test 0)
pico_enable_stdio_uart(i2s_test 1)
