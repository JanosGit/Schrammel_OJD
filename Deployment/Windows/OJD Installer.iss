[Setup]
AppName=OJD
AppVersion=0.9.5
DefaultDirName={commoncf64}
DefaultGroupName=OJD
OutputBaseFilename="Install OJD"
 
[Files]
Source: "Bin\VST3\OJD.vst3"; DestDir: "{commoncf64}\VST3"; Flags: recursesubdirs
//Source: "OJD.aaxplugin\*"; DestDir: "{app}\Avid\Audio\Plug-Ins\OJD.aaxplugin"; Flags: recursesubdirs