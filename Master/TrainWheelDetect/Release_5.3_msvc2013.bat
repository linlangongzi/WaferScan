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
set redist="C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\redist\x64\Microsoft.VC120.CRT"
set qt_jom_dir=%qt_root%\Tools\QtCreator\bin
set qt_jom=%qt_jom_dir%\jom.exe

if %fresh_compile%==YES (
        set bin_dir=Release-MSVC2013-TrainWheelDetect%install_suffix%
) else (
        set bin_dir=%qt_default_output%
)

svn info | findstr /C:"Last Changed Rev" > version.txt
set /p revision= < version.txt
del /q version.txt
set revision=%revision:~18%
echo Revision %revision%

set install=TrainWheelDetect-r%revision%%install_suffix%
set LIB=C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\LIB\amd64;C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\ATLMFC\LIB\amd64;C:\Program Files (x86)\Windows Kits\8.1\lib\winv6.3\um\x64;
REM set LIB="%VS120COMNTOOLS%\..\..\VC\LIB\amd64;%VS120COMNTOOLS%\..\..\VC\ATLMFC\LIB\amd64;C:\Program Files (x86)\Windows Kits\8.1\lib\winv6.3\um\x64;"
set INCLUDE="%VS120COMNTOOLS%\..\..\VC\include;%VS120COMNTOOLS%\..\..\VC\ATLMFC\INCLUDE;C:\Program Files (x86)\Windows Kits\8.1\include\shared;C:\Program Files (x86)\Windows Kits\8.1\include\um;C:\Program Files (x86)\Windows Kits\8.1\include\winrt"

path %path%;%qt_dir%\bin;C:\Program Files (x86)\Microsoft Visual Studio 12.0\Common7\IDE\CommonExtensions\Microsoft\TestWindow;C:\Program Files (x86)\MSBuild\12.0\bin\amd64;C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\BIN\amd64;C:\Windows\Microsoft.NET\Framework64\v4.0.30319;C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\VCPackages;C:\Program Files (x86)\Microsoft Visual Studio 12.0\Common7\IDE;C:\Program Files (x86)\Microsoft Visual Studio 12.0\Common7\Tools;C:\Program Files (x86)\HTML Help Workshop;C:\Program Files (x86)\Microsoft Visual Studio 12.0\Team Tools\Performance Tools\x64;C:\Program Files (x86)\Microsoft Visual Studio 12.0\Team Tools\Performance Tools;C:\Program Files (x86)\Windows Kits\8.1\bin\x64;C:\Program Files (x86)\Windows Kits\8.1\bin\x86

cd ..
rd /S /Q %bin_dir%
mkdir %bin_dir%
cd %bin_dir%

if %fresh_compile%==YES (
        echo 正在qmake...
        %qt_dir%\bin\qmake.exe %source_dir%\TrainWheelDetect.pro -r -spec win32-msvc2013 > nul

        echo 正在编译...
        echo on

        %qt_jom% Release
        if ERRORLEVEL 1 (
                echo .
                echo === Error with compilation ===
                echo .
                pause
        )
        @echo off
)

echo 正在编译翻译文件...
%qt_dir%\bin\lupdate.exe -no-obsolete %source_dir%\TrainWheelDetect.pro
%qt_dir%\bin\lrelease.exe %source_dir%\TrainWheelDetect.pro

cd ..

rd %install% /s /q

echo 正在复制 Qt dlls...
robocopy %qt_dir%\bin %install%\ Qt5Core.dll Qt5Gui.dll Qt5Widgets.dll Qt5OpenGL.dll Qt5Network.dll icudt52.dll icuin52.dll icuuc52.dll > nul
robocopy %qt_dir%\plugins\platforms %install%\platforms\ qwindows.dll > nul
robocopy %source_dir%\ConfigTemplates %install%\ *.json > nul
REM robocopy %source_dir%\ConfigTemplates %install%\ConfigTemplates *.json > nul
robocopy %bin_dir%\release\ %install% TrainWheelDetect.exe > nul
robocopy %redist%\ %install%\ msvcp120.dll msvcr120.dll vccorlib120.dll > nul
robocopy %source_dir%\MainWindow %install%\ TrainWheelDetect_zhCN.qm > nul

if %fresh_compile%==YES (
        echo Cleaning up...
        rd %bin_dir% /s /q
)

cd %source_dir%

echo DONE.

pause
