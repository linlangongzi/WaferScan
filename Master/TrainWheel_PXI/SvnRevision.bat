
rem Usage: SvnRevision.bat [output path] [repository path]

set "output_path=%1"
set "repo_path=%2"
if [%output_path%]==[] set "output_path=%cd%"
set "output=%output_path%\SvnRevision.inc"

del %output%
svnversion -n %repo_path% > %output%
set /p version="" < %output%
SET "number=" & for /f "delims=0123456789" %%i in ("%version%") do set number=%%i
if not defined number (
    echo SVN revision not found, try git
    git log --max-count=1 --format=%%h:%%b > %output%
    set /p version="" < %output%
)
echo #define ARS_REVISION_STR ^"%version%^" >%output%
