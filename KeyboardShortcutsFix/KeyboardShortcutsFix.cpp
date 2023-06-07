/*                             The MIT License (MIT)

Copyright (c) 2023 Sumwunn @ github.com

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

// Defined functions
// ASM
extern "C" void* BinSearch(void* Search, int SearchLength, unsigned char* Bytes, int BytesLength, int AddMod, int SubMod);
// Work around because my ASM function GetTextSectionData has multiple return value data types
extern "C" void* GetTextSectionAddr(HMODULE Module, int DataType);
extern "C" int GetTextSectionSize(HMODULE Module, int DataType);
// C++
int BinPatch(HMODULE hModule, unsigned char* BytesToFind, int BytesToFindSize, unsigned char* BytesPatch, int BytesPatchSize, int AddressModifierAdd, int AddressModifierSub);

// - Data -
// ScriptExtenderType
// 0 = None
// 1 = Fallout 4
// 2 = Skyrim SE
int ScriptExtenderType = 0;

// Return values
// 0 = Patching failed, bytes not found
// 1 = Patching successful, bytes found
// -1 = Process is NOT expected target
// -2 = Log file creation failed

extern "C" __declspec(dllexport) int Setup()
{
	LPCTSTR ExpectedProcess01 = L"SkyrimSE.exe";
	// These bytes will land us exactly at where IDirectInputDevice8::SetCooperativeLevel's is
	unsigned char BytesToFind01[] = { 0x41, 0xB8, 0x15, 0x00, 0x00, 0x00, 0x48, 0x8B, 0xD0, 0xFF, 0x53, 0x68 };
	// This is what we patch it with (check notes.txt)
	unsigned char BytesPatch01[] = { 0x41, 0xB8, 0x06 }; // dwFlags

	//////// Setup Part 1 - Config ////////

	TCHAR ConfigFilePath[MAX_PATH];
	bool bEnableLogging = true;
	bool bIgnoreExpectedProcessName = false;
	// false = Expected process name detection enabled
	// true = Ignore SkyrimSE.exe name detection; allows mod to work regardless of EXE name

	// Get config path
	GetCurrentDirectory(MAX_PATH, ConfigFilePath);
	_tcscat_s(ConfigFilePath, MAX_PATH, L"\\Data\\Plugins\\Sumwunn\\KeyboardShortcutsFix.ini");
	// Get config settings
	bEnableLogging = GetPrivateProfileInt(L"General", L"bEnableLogging", 1, ConfigFilePath);
	bIgnoreExpectedProcessName = GetPrivateProfileInt(L"General", L"bIgnoreExpectedProcessName", 0, ConfigFilePath);

	// Misc
	HMODULE hModule = NULL;
	std::ofstream LogFileHandle;

	//////// Setup Part 2 - Addresses & Logging ////////

	if (bEnableLogging)
	{
		// Open up fresh log file
		LogFileHandle.open(L"Data\\Plugins\\Sumwunn\\KeyboardShortcutsFix.log");

		// Log file creation failed
		if (!LogFileHandle)
			return -2;
	}

	// Skyrim SE
	if (bIgnoreExpectedProcessName)
	{
		hModule = GetModuleHandle(NULL);
	}
	else
		hModule = GetModuleHandle(ExpectedProcess01);

	// Did we get hModule?
	if (hModule != NULL)
	{
		// Find bytes and patch them
		bool patchResult;
		// dwFlags method
		patchResult = BinPatch(hModule, BytesToFind01, sizeof BytesToFind01, BytesPatch01, sizeof BytesPatch01, NULL, NULL);

		// Compare patching result and write result into log file
		if (!patchResult)
		{
			if (bEnableLogging)
			{
				// Bytes not found!
				// Log message
				LogFileHandle << "NO" << std::endl;
				// Cleanup
				LogFileHandle.close();
			}

			return 0;
		}
		else
		{
			if (bEnableLogging)
			{
				// Bytes found!
				// Log message
				LogFileHandle << "YES" << std::endl;
				// Cleanup
				LogFileHandle.close();
			}

			return 1;
		}
	}

	if (bEnableLogging)
	{
		// Process not found
		// Cleanup
		// Log message
		LogFileHandle << "SkyrimSE.exe not detected." << std::endl;
		LogFileHandle.close();
	}

	return -1;
}

int BinPatch(HMODULE hModule, unsigned char* BytesToFind, int BytesToFindSize, unsigned char* BytesPatch, int BytesPatchSize, int AddressModifierAdd, int AddressModifierSub) // BinSearch + MEMCPY patching.
{
	// The address we get from GetTextSectionAddr
	void* SearchAddress = (void*)NULL;
	// The size too
	int SearchSize = NULL;
	// The address we get from BinSearch
	void* PatchAddress = (void*)NULL;
	// Misc
	DWORD lpflOldProtect = NULL;

	// Get size and address of ExpectedProcess's .text section
	SearchSize = GetTextSectionSize(hModule, 1);
	SearchAddress = GetTextSectionAddr(hModule, 2);
	// Get address and patch it
	PatchAddress = BinSearch(SearchAddress, SearchSize, BytesToFind, BytesToFindSize, AddressModifierAdd, AddressModifierSub);

	// Bytes not found
	if (PatchAddress == NULL)
		return 0;
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

////// Script Extender //////

/*
extern "C" __declspec(dllexport) void SetF4SEMode()
{
	ScriptExtenderType = 1;

	return;
}
*/

extern "C" __declspec(dllexport) void SetSKSEMode()
{
	ScriptExtenderType = 2;

	return;
}
