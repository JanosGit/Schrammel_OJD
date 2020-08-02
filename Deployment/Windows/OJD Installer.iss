[Setup]
AppName=OJD
AppVersion=0.9.3
DefaultDirName={commoncf64}
DefaultGroupName=OJD
OutputBaseFilename="Install OJD"
 
[Files]
Source: "Bin\VST3\OJD.vst3"; DestDir: "{app}\VST3"
//Source: "OJD.aaxplugin\*"; DestDir: "{app}\Avid\Audio\Plug-Ins\OJD.aaxplugin"; Flags: recursesubdirs