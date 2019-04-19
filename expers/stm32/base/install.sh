arm-none-eabi-objcopy -O binary firmware firmware.bin
st-flash write firmware.bin 0x8000000
