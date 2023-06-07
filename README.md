## [PC] Keyboard Shortcuts Fix - Skyrim SE/AE

# Good News!
## The issue with this mod causing Skyrim to process off-screen keyboard keys have been resolved!
## A very big thanks to LePhilanthrope @ NexusMods for finding the issue (my patch was incorrect)!
--------------------------------------------------------------------------------------------------------------------------------------------------------------------

**What is this?**

It's a plugin I made that enables keyboard shortcuts by patching SkyrimSE.exe during runtime. It's designed to be version independent.
Alt-F4, volume keys - it just works!

**How to install**

1. Install my Dll Loader manually -OR- SKSE64.
2. Install this mod via your mod manager.
3. Refer to these images ([1](https://staticdelivery.nexusmods.com/mods/1704/images/3620/3620-1685904986-1236963600.jpeg), [2](https://staticdelivery.nexusmods.com/mods/1704/images/3620/3620-1685904986-1614642548.jpeg)) to make sure it's installed correctly.
4. Play!

**How to uninstall**

1. Uninstall via your mod manager.

**How to install (MANUALLY)**

1. Install my Dll Loader manually -OR- SKSE64.
2. Copy the Plugins & SKSE folder to Skyrim SE's Data folder (SKSE folder is only required if you are not using my dll loader).
3. Refer to these images ([1](https://staticdelivery.nexusmods.com/mods/1704/images/3620/3620-1685904986-1236963600.jpeg), [2](https://staticdelivery.nexusmods.com/mods/1704/images/3620/3620-1685904986-1614642548.jpeg)) to make sure it's installed correctly.
4. Play!

**How to uninstall (MANUALLY)**

1. Delete:
- Data\Plugins\KeyboardShortcutsFix.dll
- Data\Plugins\KeyboardShortcutsFix.ini
- Data\Plugins\KeyboardShortcutsFix.dll_Exports.txt
- Data\Plugins\KeyboardShortcutsFix.log (if present)
- Data\SKSE\Plugins\KeyboardShortcutsFixLoader.dll

**Options**:
KeyboardShortcutsFix.ini located in Data\Plugins\Sumwunn contains these options:

- iEnableLogging=1
- 0 = Disabled
- 1 = Enabled
- iIgnoreExpectedProcessName=0
- 0 = Expected process name detection enabled
- 1 = Ignore SkyrimSE.exe name detection. Allows mod to work regardless of EXE name

**How do I know if it's working?**

Try some keyboard shortcuts! Alternatively, you can check the log file.

After you've started Skyrim SE (you can alt-tab for this part), go to Data\Plugins\Sumwunn, there's a log file called: KeyboardShortcutsFix.log. 

It will either say "YES" or "NO", indicating whether or not the plugin successfully patched Skyrim SE. If it says NO, lemme know!

**Will it need updating?**

Probably not! I've designed this to be version independent; however, in the case that it does, I will try to fix it quickly.
Below is a list of game versions that have been tested and working.

**Skyrim SE tested & working versions w/ 1.0.0.3+**
- 1.6.640.0
- 1.5.97.0
- 1.5.3.0.8
- 1.4.2.0.8
- 1.3.9.0.8
- 1.3.5.0.8
- 1.2.39.0.8
- 1.2.36.0.8
- 1.1.51.0.8
- 1.1.47.0.8

**Discussion**: 

http://www.nexusmods.com/skyrimspecialedition/mods/3620

**Compile Notes**

- UASM must be setup before opening the project.

**NOTES**

- This is not available for consoles.
- This will not get you VAC banned in any manner as Skyrim SE does not use any anti-cheat.
- If the mod is not working, try restarting Steam and/or your computer.
- The dll loader version does not use SKSE. It will also not conflict with it.
- Written in C++/ASM.

**CREDITS**

- LePhilanthrope @ NexusMods for solving the keyboard keys being processed off-screen issue
- Bethesda for Skyrim SE. (http://store.steampowered.com/app/489830/)
- Microsoft for Visual Studio. (https://www.visualstudio.com/)
- The UASM devs. (https://github.com/Terraspace/UASM)
