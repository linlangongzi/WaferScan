
@echo off
setlocal ENABLEDELAYEDEXPANSION
set "prev_cd=%cd%"

echo Welcome to automatic release tool for WaferScan
set choice=%1

:: Detect, list (and maybe select if possible) Qt kits.
set "count=1"
for /D %%r IN (C:\Qt; D:\Qt) DO (
    set "root=%%r"
    set "root_name=%%~nxr"
    for /D %%v IN (!root!\?.?*) DO (
        set "version=%%~nxv"
        set "version_path=!root!\!version!"

        for /D %%k IN (!version_path!\*) DO (
            set "kit_name=%%~nxk"
            if not [!kit_name!]==[Src] (
                echo !kit_name! | findstr /C:"64" > nul && (
                    set arch=x64
                ) || (
                    set arch=x86
                )
                echo !count!. !root_name! !version!: !arch! - !kit_name!
                set "root[!count!]=!root!"
                set "root_name[!count!]=!root_name!"
                set "version[!count!]=!version!"
                set "kit[!count!]=!kit_name!"
                set "arch[!count!]=!arch!"
                if [!kit_name!]==[!choice!] (
                    set "choice=!count!"
                )

                set /A count = !count! + 1
            )
        )
    )
    if exist !root!\Tools\QtCreator\bin\jom.exe (
        set "qt_jom=!root!\Tools\QtCreator\bin\jom.exe"
    )
)

if [!count!]==[1] (
    echo Qt kits not found.
    goto :cleanup
)

:: Parse arguments
:loop
if not [%1]==[] (
    if not [!kit[%1]!]==[] (
        set choice=%1
        echo Using choice %choice%
    )
    shift
    goto loop
)
if [%choice%]==[] (
    set /p choice=Please select a Qt version:
)
if [!kit[%choice%]!]==[] (
    echo Bad choice. Aborting.
    goto :eof
)

:: Setup Qt kit and compiler environment
set qt_root=!root[%choice%]!
set qt_root_name=!root_name[%choice%]!
set qt_version=!version[%choice%]!
set qt_kit_name=!kit[%choice%]!
set architecture=!arch[%choice%]!

:: Detect and confiture compiler environment
echo Configuring compiler environment for %qt_kit_name%

if [%qt_kit_name:~0,4%]==[msvc] (
    set qt_make=!qt_jom!
    if %qt_kit_name:~0,8%==msvc2015 (
        echo Configuring with VS 14.0 vcvarsall
        set vcvarsall_path="%VS140COMNTOOLS%\..\..\VC\vcvarsall.bat"
        echo done.
    ) else if %qt_kit_name:~0,8%==msvc2013 (
        echo Configuring with VS 12.0 vcvarsall
        set vcvarsall_path="%VS120COMNTOOLS%\..\..\VC\vcvarsall.bat"
    ) else if /I %qt_kit_name:~4,4% GEQ 2017 (
        echo Looking for VS location with vswhere...

        set VS_WHERE="%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
        if not exist !VS_WHERE! (
            echo Cannot find vswhere.exe, make sure you have VS 2017 version 15.2 or later
            goto quit
        )
        for /f "usebackq tokens=*" %%j in (`!VS_WHERE! -latest -products * -requires Microsoft.Component.MSBuild -property installationPath`) do (
            set VS_INSTALL_DIR=%%j
        )
        echo Found VS installed at !VS_INSTALL_DIR!
        set vcvarsall_path="!VS_INSTALL_DIR!\VC\Auxiliary\Build\vcvars64.bat"

    ) else (
        echo Cannot find msvc compiler for kit !qt_kit_name!
        goto quit
    )
    if not exist !vcvarsall_path! (
        echo Cannot find msvc compiler init script on %vcvarsall_path%
        goto quit
    )
    call !vcvarsall_path! !architecture!
) else (
    echo Kit !qt_kit_name! currently not supported.
    goto quit
)

echo Making Release with Qt %qt_version% %qt_kit_name%

:: Prepare tool path
set "qt_dir=%qt_root%\%qt_version%\%qt_kit_name%"
set "qt_qmake=%qt_dir%\bin\qmake.exe"
set "qt_deploy=%qt_dir%\bin\windeployqt.exe"
set "qt_ifw=%qt_root%\Tools\QtInstallerFramework\2.0\bin\binarycreator.exe"
set "qt_full_name=%qt_root_name%%qt_version%-%qt_kit_name%"

set "app_name=WaferScan"
set "source_pro=ars.pro"
set "app_identifier=cn.com.allrising.dolphinsense"
set "copy=robocopy /NJH /NJS"

:: Detect repository root, read svn revision and branch
for /f %%a in ('svn info --show-item wc-root --no-newline') do set "repo_root=%%a"
set "repo_root=%repo_root:/=\%"

for /f %%a in ('svn info %repo_root% --show-item last-changed-revision --no-newline') do set "svn_revision=%%a"
for /f %%a in ('svn info %repo_root% --show-item relative-url --no-newline') do set "svn_branch=%%~nxa"

echo Repo root at %repo_root% revision %svn_revision% in branch %svn_branch%

:: Prepare build/package paths
set "source_dir=%repo_root%\Source"
set "build_dir=%repo_root%\build-%app_name%-%qt_full_name%"
set "package_root=%repo_root%\build-%app_name%-%qt_full_name%-package"
set "package_data=%package_root%\%app_identifier%\data"
set "package_out=%repo_root%\%app_name%-%svn_branch%-%svn_revision%"

:: Cleanup and create build dir
echo Cleanup...
if exist %build_dir% (
    rd /S /Q %build_dir%
)
mkdir %build_dir%

cd %build_dir%

:: Build step: qmake
echo qmake...
%qt_qmake% %source_dir%\%source_pro% -r %qt_qmake_args% DEFINES+=ARS_FINAL_RELEASE CONFIG+=force_debug_info

if ERRORLEVEL 1 (
    echo.
    echo === Error with qmake ===
    pause
    goto cleanup
)

:: Build step: make
echo Compiling...
%qt_make% /S Release

if ERRORLEVEL 1 (
    echo.
    echo === Error with compilation ===
    echo --- Dumping environment variables ---
    set
    echo --- End of environment variables ---
    echo === Error with compilation ===
    echo.
    pause
    goto cleanup
)

:: Create package dir
echo Deploying...
set "build_bin=%build_dir%\bin"

if exist %package_root% (
    rd /S /Q %package_root%
)
:: Copy deploy package meta-info
%copy% /E %source_dir%\deploy\%app_identifier% %package_root%\%app_identifier% *
%copy% %source_dir%\app %package_data% Logo.ico

:: Copy binaries and plugins
%copy% /S %build_bin% %package_data% *
rem %copy% %build_bin%\plugins %package_data%\plugins *.dll

:: Deploy Qt library to binary dir
rem %qt_deploy% -release %package_data% %package_data%\plugins
%qt_deploy% -release %package_data%

:: del /Q %package_data%\*.pdb

:: Create installer
if exist %qt_ifw% (
    echo Creating installer...
    %qt_ifw% -c %source_dir%\deploy\config.xml -p %package_root% %package_out%
)

:cleanup
cd %prev_cd%

:quit
