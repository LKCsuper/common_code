
@ECHO OFF

set BOOT_LOCA=./
set APP_LOCA=./

copy %BOOT_LOCA%\*.bin .\
copy %APP_LOCA%\*.bin .\

set BOOT_SIZE=0x00004000

set FLASH_ADDR=0x80000000

set APP_OFFSET=%BOOT_SIZE%

set BOOT_NAME=BOOT

set APP_NAME=APP

set COMBIN_NAME=HOOK

del %COMBIN_NAME%.bin

del %COMBIN_NAME%.hex

fsutil file createnew %COMBIN_NAME%.bin 0

..\srecord-1.65.0-win64\srecord-1.65.0-win64\bin\srec_cat.exe -output "%COMBIN_NAME%.bin" -binary %BOOT_NAME%.bin -binary -fill 0xff 0x00000000 %BOOT_SIZE% %APP_NAME%.bin -binary -offset %APP_OFFSET%

..\srecord-1.65.0-win64\srecord-1.65.0-win64\bin\srec_cat.exe %COMBIN_NAME%.bin -binary -offset -%FLASH_ADDR% -o %COMBIN_NAME%.hex -intel

echo 合成文件%COMBIN_NAME%.bin成功

echo 转换文件%COMBIN_NAME%.hex成功

pause