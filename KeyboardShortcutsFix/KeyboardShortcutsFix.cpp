/*                             The MIT License (MIT)

Copyright (c) 2017 Sumwunn @ github.com

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.*/

#include "stdafx.h"
#include <windows.h>
#include <fstream>
#include <iostream>
#include <string>

// Defined functions.
// ASM
extern "C" void* BinSearch(void* Search, int SearchLength, unsigned char* Bytes, int BytesLength, int AddMod, int SubMod);
// Work around because my ASM function GetTextSectionData has multiple return value data types.
extern "C" void* GetTextSectionAddr(HMODULE Module, int DataType);
extern "C" int GetTextSectionSize(HMODULE Module, int DataType);
// C++
int BinPatch(HMODULE hModule, unsigned char* BytesToFind, int BytesToFindSize, unsigned char* BytesPatch, int BytesPatchSize, int AddressModifierAdd, int AddressModifierSub);

// Return values
// 0 = Patching failed, bytes not found.
// 1 = Patching successful, bytes found.
// -1 = Process is NOT expected target.
// -2 = Log file creation failed.

extern "C" __declspec(dllexport) int Setup(int ScriptExtenderType)
// ScriptExtenderType
// 1 = (reserved). 2 = SKSE64.
{
	LPCTSTR ExpectedProcess01 = L"SkyrimSE.exe";
	// These bytes will land us exactly at where IDirectInputDevice8::SetCooperativeLevel's is.
	unsigned char BytesToFind01_01[] = { 0x41, 0xB8, 0x15, 0x00, 0x00, 0x00, 0x48, 0x8B, 0xD0, 0xFF, 0x53, 0x68 };
	// This is what we patch it with (check notes.txt).
	unsigned char BytesPatch01_01[] = { 0x48, 0x31, 0xD2 }; // hwnd.
	unsigned char BytesPatch01_02[] = { 0x41, 0xB8, 0x0D }; // dwFlags.
	// We need to go back X bytes so we land at the right address.
	int AddressModifierSub01_01 = 0x6; // hwnd.

	//////// Setup Part 1 - Config ////////

	TCHAR ConfigFilePath[MAX_PATH];
	int iEnableLogging = 1;
	// 0 = Disable.
	// 1 = Enable.
	int iIgnoreExpectedProcessName = 0;
	// 0 = Expected process name detection enabled.
	// 1 = Ignore SkyrimSE.exe name detection. Allows mod to work regardless of EXE name.
	int iPatchMethod = 1;
	// 1 = hwnd.
	// 2 = dwFlags.

	// Get config path.
	GetCurrentDirectory(MAX_PATH, ConfigFilePath);
	if (ScriptExtenderType == 2) // SKSE64 path.
	{
		_tcscat_s(ConfigFilePath, MAX_PATH, L"\\Data\\SKSE\\Plugins\\KeyboardShortcutsFix.ini");
	}
	else // Dll loader path.
	{
		_tcscat_s(ConfigFilePath, MAX_PATH, L"\\Data\\Plugins\\Sumwunn\\KeyboardShortcutsFix.ini");
	}
	// Get config settings.
	iEnableLogging = GetPrivateProfileInt(L"General", L"iEnableLogging", 1, ConfigFilePath);
	iIgnoreExpectedProcessName = GetPrivateProfileInt(L"General", L"iIgnoreExpectedProcessName", 0, ConfigFilePath);
	iPatchMethod = GetPrivateProfileInt(L"General", L"iPatchMethod", 1, ConfigFilePath);

	// Checking for incorrect values.
	if (iEnableLogging < 0 || iEnableLogging > 1)
	{
		iEnableLogging = 1;
	}
	if (iIgnoreExpectedProcessName < 0 || iIgnoreExpectedProcessName > 1)
	{
		iIgnoreExpectedProcessName = 0;
	}
	if (iPatchMethod < 0 || iPatchMethod > 2)
	{
		iPatchMethod = 1;
	}

	// Misc.
	HMODULE hModule = NULL;
	std::ofstream LogFileHandle;

	//////// Setup Part 2 - Addresses & Logging ////////

	if (iEnableLogging == 1)
	{
		// Open up fresh log file.
		if (ScriptExtenderType == 2) // SKSE64 path.
		{
			LogFileHandle.open(L"Data\\SKSE\\Plugins\\KeyboardShortcutsFix.log");
		}
		else // Dll loader path.
		{
			LogFileHandle.open(L"Data\\Plugins\\Sumwunn\\KeyboardShortcutsFix.log");
		}

		// Log file creation failed.
		if (!LogFileHandle)
		{
			return -2;
		}
	}

	// Skyrim SE.
	if (iIgnoreExpectedProcessName == 1)
	{
		hModule = GetModuleHandle(NULL);
	}
	else
	{
		hModule = GetModuleHandle(ExpectedProcess01);
	}
	// Did we get hModule?
	if (hModule != NULL)
	{
		// Find bytes and patch them.
		int PatchingResult = NULL;
		// hwmd method.
		if (iPatchMethod = 1) 
		{
			PatchingResult = BinPatch(hModule, BytesToFind01_01, sizeof BytesToFind01_01, BytesPatch01_01, sizeof BytesPatch01_01, AddressModifierSub01_01, NULL);
		}
		// dwFlags method.
		else if (iPatchMethod = 2)
		{
			PatchingResult = BinPatch(hModule, BytesToFind01_01, sizeof BytesToFind01_01, BytesPatch01_02, sizeof BytesPatch01_02, NULL, NULL);
		}
		// Compare patching result and write result into log file.
		if (PatchingResult == 0)
		{
			if (iEnableLogging == 1)
			{
				// Bytes not found!
				// Log message.
				LogFileHandle << "NO" << std::endl;
				// Cleanup.
				LogFileHandle.close();
			}
			return 0;
		}
		else
		{
			if (iEnableLogging == 1)
			{
				// Bytes found!
				// Log message.
				LogFileHandle << "YES" << std::endl;
				// Cleanup.
				LogFileHandle.close();
			}
			return 1;
		}
	}

	if (iEnableLogging == 1)
	{
		// Process not found.
		// Cleanup.
		// Log message.
		LogFileHandle << "SkyrimSE.exe not detected." << std::endl;
		LogFileHandle.close();
	}
	return -1;
}

int BinPatch(HMODULE hModule, unsigned char* BytesToFind, int BytesToFindSize, unsigned char* BytesPatch, int BytesPatchSize, int AddressModifierAdd, int AddressModifierSub) // BinSearch + MEMCPY patching.
{
	// The address we get from GetTextSectionAddr.
	void* SearchAddress = (void*)NULL;
	// The size too.
	int SearchSize = NULL;
	// The address we get from BinSearch.
	void* PatchAddress = (void*)NULL;
	// Misc.
	DWORD lpflOldProtect = NULL;

	// Get size and address of ExpectedProcess's .text section.
	SearchSize = GetTextSectionSize(hModule, 1);
	SearchAddress = GetTextSectionAddr(hModule, 2);
	// Get address and patch it.
	PatchAddress = BinSearch(SearchAddress, SearchSize, BytesToFind, BytesToFindSize, AddressModifierAdd, AddressModifierSub);
	if (PatchAddress == NULL)
	{
		// Bytes not found.
		return 0;
	}
	// Bytes found!
	else
	{
		// Patch it! (with NOPS)
		VirtualProtect(PatchAddress, BytesPatchSize, PAGE_EXECUTE_READWRITE, &lpflOldProtect);
		memcpy(PatchAddress, BytesPatch, BytesPatchSize);
		VirtualProtect(PatchAddress, BytesPatchSize, lpflOldProtect, &lpflOldProtect);
		return 1;
	}

	return 0;
}

#ifdef _SKSE64_
////// SKSE64 //////
#include "common\IPrefix.h"
#include "skse64\PluginAPI.h"

extern "C" __declspec(dllexport) bool SKSEPlugin_Query(const SKSEInterface * skse, PluginInfo * info)
{
	info->infoVersion = PluginInfo::kInfoVersion;
	info->name = "KeyboardShortcutsFix";
	info->version = 1;

	return TRUE;
}

extern "C" __declspec(dllexport) bool SKSEPlugin_Load(const SKSEInterface * skse)
{
	Setup(2);

	return TRUE;
}
#endif