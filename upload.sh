c_ext=".c"
ard_dev="/dev/ttyACM0"
ard_3="115200"

arduino_install_path="/home/yves/Downloads/arduino-1.8.19-linux64/arduino-1.8.19"
running_path="/home/yves/Schreibtisch/mega2560-fun"

# already built disassembler and dotnet-runtime-3.1 required
avrdisassembler_path="/home/yves/Downloads/AVRDisassembler"

programn=$1;

avr-gcc -Os -DF_APU=16000000UL -mmcu=atmega2560 -c -o tmp/${programn}.o ${programn}.c
avr-gcc -mmcu=atmega2560 tmp/${programn}.o -o tmp/${programn}.bin
avr-objcopy -O ihex -R .eeprom tmp/${programn}.bin tmp/${programn}.hex

if [[ $avrdisassembler_path != "" ]]
then
${avrdisassembler_path}/Source/AVRDisassembler/AVRDisassembler/bin/release/netcoreapp3.1/AVRDisassembler -i ${running_path}/tmp/${programn}.hex > ${running_path}/tmp/${programn}.dasm
fi

sudo ${arduino_install_path}/hardware/tools/avr/bin/avrdude -C${arduino_install_path}/hardware/tools/avr/etc/avrdude.conf -v -patmega2560 -cwiring -P/dev/ttyACM0 -b115200 -D -Uflash:w:tmp/${programn}.hex:i
