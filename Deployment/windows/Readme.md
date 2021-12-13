# Windows installer

This folder contains a script for an Inno Setup based installer project. To successfully install on Windows machines, we need to make sure that the Visual C++ Runtime library is installed. This can be handled by the `VC_redist.x64.exe` which is available for free [from Microsoft](https://www.microsoft.com/en-US/download/details.aspx?id=48145). In order to build the compiler, you need to place `VC_redist.x64.exe` manually in the `Bin` subfolder of this repository.

Legal Note: While the plugin itself and the rest of this project is licensed under a GPL v3 license if not stated differently in any subfolder, the Windows installer is licensed under a LGPL v3 license. I do this, as the GPL v3 license does allow dynamically linking against system libraries such as the Visual C++ runtime but does not allow shipping such libraries along with the product.
