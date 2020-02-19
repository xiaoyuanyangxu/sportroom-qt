function Component()
{
    // constructor
    var programFiles = installer.environmentVariable("ProgramFiles");
    if (programFiles !== "")
        installer.setValue("TargetDir", programFiles + "//Dimsum");
}

Component.prototype.isDefault = function()
{
    // select the component by default
    return true;
}

Component.prototype.createOperations = function()
{
    try {
        // call the base create operations function
        component.createOperations();
        if (installer.value("os") === "win") {
           component.addOperation("CreateShortcut", "@TargetDir@/Dimsum.exe", "@StartMenuDir@/Dimsum.lnk");
        }
    } catch (e) {
        print(e);
    }
}
