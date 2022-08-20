c_ext=".c"
ard_dev="/dev/ttyACM0"
ard_3="115200"
if [[ "$1" =~ "$c_ext" ]]
then
    program_name="${1//$c_ext/}"
else
    program_name=$1;
fi

avr-gcc -Os -DF_APU=16000000UL -mmcu=atmega2560 -c -o tmp/${program_name}.o ${program_name}.c
avr-gcc -mmcu=atmega2560 tmp/${program_name}.o -o tmp/${program_name}.bin
avr-objcopy -O ihex -R .eeprom tmp/${program_name}.bin tmp/${program_name}.hex
#sudo avrdude -C/home/yves/Downloads/arduino-1.8.19-linux64/arduino-1.8.19/hardware/tools/avr/etc/avrdude.conf -F -V -c arduino -p atmega2560 -cwiring -P $ard_dev -b $ard_3 -U flash:w:${program_name}.hex
#avrdude -C/home/yves/Downloads/arduino-1.8.19-linux64/arduino-1.8.19/hardware/tools/avr/etc/avrdude.conf -v -patmega2560 -cwiring -P/dev/ttyACM0 -b115200 -D -Uflash:w:/tmp/arduino_build_103871/sketch_aug20a.ino.hex:i 
sudo /home/yves/Downloads/arduino-1.8.19-linux64/arduino-1.8.19/hardware/tools/avr/bin/avrdude -C/home/yves/Downloads/arduino-1.8.19-linux64/arduino-1.8.19/hardware/tools/avr/etc/avrdude.conf -v -patmega2560 -cwiring -P/dev/ttyACM0 -b115200 -D -Uflash:w:$tmp/{program_name}.hex:i
