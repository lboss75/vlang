set root_folder=%~d0%~p0
call setenv.bat

rmdir %root_folder%\build /s /q
mkdir %root_folder%\build
cd %root_folder%\build

cmake.exe  ../ -G %project_style%

start vlang.sln 