@echo off

echo 正在生成 Release %qt_version%

choice /M "使用MSVC编译最优化Release?"
if ERRORLEVEL 2 (
        set fresh_compile=NO
) else (
        set fresh_compile=YES
)

set source_dir=%cd%
echo source_dir %source_dir%
set qt_jom_dir=%qt_root%\Tools\mingw482_32\bin
set qt_jom=%qt_jom_dir%\mingw32-make.exe

if %fresh_compile%==YES (
        set bin_dir=Release-mingw32-TrainWheel_PXI%install_suffix%
) else (
        set bin_dir=%qt_default_output%
)

svn info | findstr /C:"Last Changed Rev" > version.txt
set /p revision= < version.txt
del /q version.txt
set revision=%revision:~18%
echo Revision %revision%

set install=TrainWheel_PXI-r%revision%%install_suffix%

path %path%;%qt_dir%\bin;C:\Qt\Tools\mingw482_32\bin;

cd ..
rd /S /Q %bin_dir%
mkdir %bin_dir%
cd %bin_dir%

if %fresh_compile%==YES (
        echo 正在qmake...
        %qt_dir%\bin\qmake.exe %source_dir%\TrainWheel_PXI.pro -r -spec win32-g++ > nul

        echo 正在编译...
        echo on

        %qt_jom%
        if ERRORLEVEL 1 (
                echo .
                echo === Error with compilation ===
                echo .
                pause
        )
        @echo off
)

cd ..

rd %install% /s /q

echo 正在复制 Qt dlls...
robocopy %qt_dir%\bin %install%\ Qt5Core.dll Qt5Gui.dll Qt5Widgets.dll Qt5OpenGL.dll Qt5Network.dll libgcc_s_dw2-1.dll libstdc++-6.dll libwinpthread-1.dll icudt52.dll icuin52.dll icuuc52.dll > nul
robocopy %qt_dir%\plugins\platforms %install%\platforms\ qwindows.dll > nul
robocopy %source_dir%\ConfigTemplates %install%\ *.json > nul
echo 正在复制 pci9054 dlls...
robocopy %bin_dir%\ %install%\ plx_dll.dll wdapi901.dll
REM robocopy %source_dir%\ConfigTemplates %install%\ConfigTemplates *.json > nul
robocopy %bin_dir%\release\ %install% TrainWheel_PXI.exe > nul

if %fresh_compile%==YES (
        echo Cleaning up...
        rd %bin_dir% /s /q
)

cd %source_dir%

echo DONE.

pause
