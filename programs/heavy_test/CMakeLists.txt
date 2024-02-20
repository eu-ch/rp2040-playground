file(GLOB heavy_src CONFIGURE_DEPENDS
          hvcc_gen/c/*.hpp
          hvcc_gen/c/*.h
          hvcc_gen/c/*.c
          hvcc_gen/c/*.cpp
          )

add_executable(heavy_test code/main.cpp ${heavy_src})
target_include_directories(heavy_test
        PRIVATE
        include
        hvcc_gen/c
        )
target_compile_definitions(heavy_test
        PRIVATE
        PICO_AUDIO_I2S_MONO_OUTPUT=0
        PICO_AUDIO_I2S_DATA_PIN=10
        PICO_AUDIO_I2S_CLOCK_PIN_BASE=11
        )
target_compile_options(heavy_test 
        PRIVATE
        -Wno-strict-aliasing
        )

# pull in common dependencies
target_link_libraries(heavy_test
        pico_stdlib
        pico_audio
        pico_audio_i2s
        pico_util_buffer
        )

pico_define_boot_stage2(slower_boot2 ${PICO_DEFAULT_BOOT_STAGE2_FILE})
target_compile_definitions(slower_boot2 PRIVATE PICO_FLASH_SPI_CLKDIV=4)

pico_set_boot_stage2(heavy_test slower_boot2)

pico_add_extra_outputs(heavy_test)
pico_enable_stdio_usb(heavy_test 1)
pico_enable_stdio_uart(heavy_test 1)
