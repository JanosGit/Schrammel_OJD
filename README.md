[![Github All Releases](https://img.shields.io/github/downloads/janosgit/schrammel_ojd/total.svg)]() ![Build](https://github.com/janosgit/schrammel_ojd/workflows/Build/badge.svg)

# Schrammel OJD - Model of a modern classic guitar overdrive pedal

Hi! My name is Janos, I like playing e-guitar & tweaking my guitar pedalboard as a hobby, I'm working as an audio software programmer and studied electrical engineering. Coming from this background, I thought that the (guitar/audio) world would need a good sounding, good looking & open source guitar overdrive plugin. 

So... May I introduce to you: The Schrammel OJD. Heavily inspired by the schematics of a modern classic analog pedal, digitally built with my favourite C++ framework [JUCE](https://github.com/juce-framework/JUCE).

Just add it to your FX chain before your digital amp simulation of choice and get that distortion sound that simply fits the mix perfectly.

![](Documentation/Images/OJD_Cubase.png)

## License
The OJD is licensed under a [GPLv3](https://www.gnu.org/licenses/licenses.en.html#GPL) license. This license applies to all parts of this repository except for 
- The external third party dependencies that can be found as git submodules in the `Ext` subdirectory and contain their own license information. These licenses are all GPLv3 compatible, so the OJD as a combined product is under GPLv3 license
- The Windows installer, found in the `Deployment/Windows` subdirectory, which is licensed under a [LGPLv3](https://www.gnu.org/licenses/licenses.en.html#LGPL) license as the distributed versions of the installer will contain the closed source Microsoft Visual C++ redistributable runtime library installer

## How to get the OJD? 🤷‍♀️🤷‍♂️

The plugin is currently in a late, mostly stable beta phase. To download the latest build, go to [my website](https://schrammel.io) and download it for free or alternatively browse the releases directly here on GitHub. Feeback is welcome, you can reach me via the contact via my website or post issues here on GitHub! 

The OJD is currently available for Mac OS and Windows as VST3 and AU (Mac OS only) plugin. I'm working on getting AAX signing up & running to also release an AAX version in the near future.

## Thank goes out to...
1. My long time good friend and band mate [Henning Oskamp](http://www.henningoskamp.de) for his incredible work on the user interface and website design. The plugin would not look half as good if I had tried designing it ❤️
2. My employer [sonible](https://www.sonible.com) for supporting me to work on open source projects beneath my job and for promoting this project through a [blog article](https://www.sonible.com/blog/ojd-open-source-plugin/)
3. [JetBrains](https://www.jetbrains.com/?from=schrammel_ojd), a company that builds IDEs that are superior to any other option that I tried out there. I'm very thankful that they support this project with a free license for their CMake IDE Clion!

<a href="https://www.jetbrains.com/?from=schrammel_ojd"><img src="Documentation/Images/icon_CLion.svg" width="15%">

## How to build the plugin from source

The OJD is a free open source plugin, there is no license key required. Beneath downloading the ready-to-use installers, you can always build it yourself from sources if you are familiar with all those software development stuff. The project is based on a CMake build script. If you are familiar with CMake, building the plugin should be as much as two commands on your command line or one click in a CMake capable IDE like e.g. CLion. I'll post some more detailed information on that in the near future.
