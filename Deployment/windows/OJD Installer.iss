[Setup]
AppName=OJD
AppVersion=0.9.8
DefaultDirName={commoncf64}
DefaultGroupName=OJD
OutputBaseFilename="Install OJD" 
DisableDirPage=yes

[Files]
Source: "VST3\OJD.vst3"; DestDir: "{commoncf64}\VST3"; Flags: recursesubdirs
//Source: "AAX\OJD.aaxplugin\*"; DestDir: "{app}\Avid\Audio\Plug-Ins\OJD.aaxplugin"; Flags: recursesubdirs
Source: "vc_redist.x64.exe"; DestDir: {tmp}; Flags: deleteafterinstall

[Run]
Filename: {tmp}\vc_redist.x64.exe; Parameters: "/install /passive /norestart"; StatusMsg: Installing VC++ 2019 Redistributables...
