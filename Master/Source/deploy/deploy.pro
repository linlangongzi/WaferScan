# Build all-in-one installer package.

TEMPLATE = aux

DISTFILES += config.xml.in \
    package/package.xml.in \
    package/installscript.qs \
    package/license.txt

# Write package xml
config.input = $$PWD/config.xml.in
config.output = $$OUT_PWD/config.xml

package.input = $$PWD/package/package.xml.in
package.output = $$DS_PACKAGE_META/package.xml

QMAKE_SUBSTITUTES += config package

# Target for deploying Qt libraries
qtPrepareTool(QMAKE_DEPLOYQT, windeployqt, _DEP)
deployQt.commands = $$QMAKE_DEPLOYQT -release $$DS_PACKAGE_DATA #$$DS_PLUGIN_DLL_PATH
#deployQt.depends = copy_package_data

# Prepare Qt Installer Framework "bincreator" executable
QMAKE_ROOT = $$dirname(QMAKE_QMAKE)
BINCREATOR_PATH = $$clean_path($$QMAKE_ROOT/../../../Tools/QtInstallerFramework/2.0/bin)
BINCREATOR = $$BINCREATOR_PATH/binarycreator.exe

INPUT_FILE = $$OUT_PWD/config.xml

# Target for creating installer
createInstaller.input += INPUT_FILE
createInstaller.output = $$DS_APP_NAME-Release-$$DS_VERSION.exe
isEmpty(vcproj):createInstaller.variable_out = PRE_TARGETDEPS
createInstaller.commands = $$BINCREATOR -c $$INPUT_FILE -p $$DS_PACKAGE_PATH $${createInstaller.output}
createInstaller.name = createInstaller
createInstaller.config += no_link
createInstaller.depends = deployQt
QMAKE_EXTRA_COMPILERS += createInstaller

QMAKE_EXTRA_TARGETS += createInstaller deployQt

# Copy output files to packaging dir.
package_data.files = \
    $$DS_BIN_PATH
package_data.path = $$DS_PACKAGE_DATA
package_data.base = $$DS_BIN_PATH

# Copy meta files to packaging dir.
package_meta.files = \
    package/installscript.qs \
    package/license.txt
package_meta.path = $$DS_PACKAGE_META

COPIES = package_data package_meta
