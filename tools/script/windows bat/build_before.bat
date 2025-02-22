@echo off
chcp 65001
echo build_before.bat主脚本执行开始
call ./build_info.bat
:: 这里应该删除之前的info文件，当然也可以使用gitnore忽略该文件上传
echo build_before.bat主脚本执行完毕