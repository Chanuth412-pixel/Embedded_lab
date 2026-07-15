# Embedded_lab
For lab tasks in Embedded system related labs
avr-gcc -Os -DF_CPU=16000000UL -mmcu=atmega328p -o q1.elf q1.c
avr-objcopy -O ihex -R .eeprom q1.elf q1.hex
avrdude -F -V -c arduino -p ATMEGA328P -P COM3 -b 115200 -U flash:w:q1.hex

"C:\Users\USER\AppData\Local\Arduino15\packages\arduino\tools\avrdude\8.0.0-arduino1\bin\avrdude.exe" -F -V -c arduino -p ATMEGA328P -P COM3 -b 115200 -U flash:w:q1.hex