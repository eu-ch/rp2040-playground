set(HEAVY_SRC_FILES
    heavy/HvControlBinop.c
    heavy/HvControlCast.c
    heavy/HvControlDelay.c
    heavy/HvControlRandom.c
    heavy/HvControlSlice.c
    heavy/HvControlUnop.c
    heavy/HvLightPipe.c
    heavy/HvMessage.c
    heavy/HvMessagePool.c
    heavy/HvMessageQueue.c
    heavy/HvTable.c
    heavy/HvUtils.c
    heavy/Heavy_flip_led.cpp
    heavy/HeavyContext.cpp
    heavy/HvHeavy.cpp
   )

add_executable(heavyflop code/main.cpp ${HEAVY_SRC_FILES})
target_include_directories(heavyflop PRIVATE heavy)

# pull in common dependencies
target_link_libraries(heavyflop pico_stdlib)

# create map/bin/hex file etc.
pico_add_extra_outputs(heavyflop)

pico_enable_stdio_usb(heavyflop 0)
pico_enable_stdio_uart(heavyflop 1)
