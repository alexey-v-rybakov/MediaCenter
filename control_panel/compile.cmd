rem set ARDUINO_PATH=C:\Program Files (x86)\Arduino
rem set AVRDUDE_PATH=C:\Users\rybak\AppData\Local\Arduino15\packages\arduino\tools\avrdude\6.3.0-arduino17
rem set ARDUINO_COM_LOAD=COM12
mkdir "%cd%\output"

"%ARDUINO_PATH%"\arduino-builder.exe -built-in-libraries "%ARDUINO_PATH%\libraries" -libraries "%cd%\libraries" -hardware "%ARDUINO_PATH%"\hardware -tools "%ARDUINO_PATH%"\hardware\tools -tools "%ARDUINO_PATH%"\tools-builder -fqbn=arduino:avr:nano:cpu=atmega328old -build-path "%cd%\output" -compile control_panel.ino
"%AVRDUDE_PATH%"\bin\avrdude.exe -C"%AVRDUDE_PATH%\etc\avrdude.conf" -v -patmega328p -carduino -P%ARDUINO_COM_LOAD% -b57600 -D -Uflash:w:output/control_panel.ino.hex:i