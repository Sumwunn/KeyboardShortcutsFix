## [PC] Keyboard Shortcuts Fix - Skyrim SE

**What is this?**

It's a plugin I made that enables keyboard shortcuts by patching SkyrimSE.exe during runtime. It's designed to be version independent. 
Alt-F4, volume keys, it just works!

**How to install**

1. Install my Dll Loader manually -OR- SKSE64.
2. If using the dll loader version: copy the plugin folder to Skyrim SE's Data folder.
3. If using the SKSE64 version: copy the SKSE folder to Skyrim SE's Data folder.
3. Play!

**How to uninstall**

1. Uninstall via NMM or manually:
- If using dll loader version, delete KeyboardShortcutsFix.dll, KeyboardShortcutsFix.dll_Exports.txt & KeyboardShortcutsFix.ini from Data\Plugins\Sumwunn.
- If using SKSE64 version, delete KeyboardShortcutsFix.dll & KeyboardShortcutsFix.ini from Data\SKSE\Plugins.

**Options**:
KeyboardShortcutsFix.ini located in Data\Plugins\Sumwunn contains these options:

- iEnableLogging=1
- 0 = Disabled.
- 1 = Enabled.
- iIgnoreExpectedProcessName=0
- 0 = Expected process name detection enabled.
- 1 = Ignore SkyrimSE.exe name detection. Allows mod to work regardless of EXE name.
- iPatchMethod=1
- 1 = hwnd method.
- 2 = dwFlags method.

**How do I know if it's working?**

Try some keyboard shortcuts! Alternatively you can check the log file.

After you've started Skyrim SE (you can alt-tab for this part), go to Data\Plugins\Sumwunn (for SKSE64: Data\SKSE\Plugins), there's a log file called: KeyboardShortcutsFix.log. 

It will either say "YES" or "NO". Indicating whether or not the plugin successfully patched Skyrim SE. So if it says NO, lemme know!

**Will it need updating?**

Probably not! I've designed this to be version independent.
However in the case that it does, I will have it fixed very quickly.
Below is a list of game versions that have been tested and working.

**Skyrim SE tested & working versions w/ 1.0.0.1**

- 1.5.3.0.8
- 1.4.2.0.8
- 1.3.9.0.8
- 1.3.5.0.8
- 1.2.39.0.8
- 1.2.36.0.8
- 1.1.51.0.8
- 1.1.47.0.8

**Discussion**: 

http://www.nexusmods.com/skyrimspecialedition/mods/245/?

**Compile Notes**

- UASM must be setup before opening the project.

**NOTES**

- This is not available for consoles.
- This will not get you VAC banned in any manner as Skyrim SE does not use any anti-cheat.
- If the mod is not working, try restarting Steam and/or your computer.
- The dll loader version does not use SKSE. It will also not conflict with it.
- Written in C++/ASM.

**CREDITS**

Bethesda for Fallout 4. (https://store.steampowered.com/app/377160/)

Bethesda for Skyrim SE. (http://store.steampowered.com/app/489830/)

Microsoft for Visual Studio. (https://www.visualstudio.com/)

The UASM devs. (https://github.com/Terraspace/UASM)
