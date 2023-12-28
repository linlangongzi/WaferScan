svnversion > SoftwareVersion.inc
set /p version="" < SoftwareVersion.inc
echo #define SOFTWARE_VER_STR ^"%version%^" > SoftwareVersion.inc
