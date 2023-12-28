function cancelInstaller(message)
{
    installer.setDefaultPageVisible(QInstaller.Introduction, false);
    installer.setDefaultPageVisible(QInstaller.TargetDirectory, false);
    installer.setDefaultPageVisible(QInstaller.ComponentSelection, false);
    installer.setDefaultPageVisible(QInstaller.ReadyForInstallation, false);
    installer.setDefaultPageVisible(QInstaller.StartMenuSelection, false);
    installer.setDefaultPageVisible(QInstaller.PerformInstallation, false);
    installer.setDefaultPageVisible(QInstaller.LicenseCheck, false);

    var abortText = "<font color='red'>" + message +"</font>";
    installer.setValue("FinishedText", abortText);
}

function majorVersion(str)
{
    return parseInt(str.split(".", 1));
}

function Component()
{
    // default constructor
    if (systemInfo.kernelType != "winnt") {
        cancelInstaller(component.displayName + " 不支持非 Windows 操作系统");
    }
    if ( systemInfo.currentCpuArchitecture != "x86_64") {
        cancelInstaller(component.displayName + " 不支持 32 位操作系统.");
    }
    if (majorVersion(systemInfo.kernelVersion) < 6) {
        cancelInstaller(component.displayName + " 不支持 Windows XP 及更低的 Windows 版本");
    }
}

Component.prototype.createOperations = function()
{
    // call default implementation to actually install README.txt!
    component.createOperations();

    if (systemInfo.productType === "windows") {
        component.addOperation("CreateShortcut", "@TargetDir@/WaferScan.exe", "@StartMenuDir@/WaferScan.lnk",
            "workingDirectory=@TargetDir@", "iconPath=@TargetDir@/Logo.ico");
    }
    if (systemInfo.productType === "windows") {
        component.addOperation("CreateShortcut", "@TargetDir@/maintenancetool.exe", "@StartMenuDir@/maintenancetool.lnk",
            "workingDirectory=@TargetDir@", "iconPath=%SystemRoot%/system32/SHELL32.dll",
            "iconId=2");

        component.addElevatedOperation("Execute", "{0,1638}", "@TargetDir@/vcredist_x64.exe", "/quiet", "/norestart");
    }
}
