
call "setup_mingw.bat"

cd .

if "%1"=="" ("D:\MATLAB\bin\win64\gmake"  -f test.mk all) else ("D:\MATLAB\bin\win64\gmake"  -f test.mk %1)
@if errorlevel 1 goto error_exit

exit /B 0

:error_exit
echo The make command returned an error of %errorlevel%
exit /B 1