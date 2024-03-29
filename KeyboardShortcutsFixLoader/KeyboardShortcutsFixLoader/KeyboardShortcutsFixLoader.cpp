#include "stdafx.h"
#define _SKSE64_

////// SKSE64 //////
#ifdef _SKSE64_
#include "common\IPrefix.h"
#include "SKSE64\SKSE64\PluginAPI.h"
#include "skse64\skse64_common\skse_version.h"

// For pre-AE
extern "C" __declspec(dllexport) bool SKSEPlugin_Query(const SKSEInterface * skse, PluginInfo * info)
{
	info->infoVersion = PluginInfo::kInfoVersion;
	info->name = "Keyboard Shortcuts Fix Loader";
	info->version = 1;

	return TRUE;
}

// For AE
extern "C" {
	// Modified example
	__declspec(dllexport) SKSEPluginVersionData SKSEPlugin_Version =
	{
		SKSEPluginVersionData::kVersion,

		1, // version number
		"Keyboard Shortcuts Fix Loader", // plugin name

		"Sumwunn @ GitHub.com", // plugin author name
		"", // support address

		SKSEPluginVersionData::kVersionIndependentEx_NoStructUse, // versionIndependenceEx
		SKSEPluginVersionData::kVersionIndependent_Signatures, // versionIndependence

		{ 0 }, // compatibleVersions

		0, // seVersionRequired; 0: works with any version of the script extender. you probably do not need to put anything here
	};
};

extern "C" __declspec(dllexport) bool SKSEPlugin_Load(const SKSEInterface * skse)
{
	// Get dll path.
	TCHAR DllFilePath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, DllFilePath);
	_tcscat_s(DllFilePath, MAX_PATH, L"\\Data\\Plugins\\Sumwunn\\KeyboardShortcutsFix.dll");

	// Return if already loaded.
	if (GetModuleHandle(DllFilePath) != 0) 
		return FALSE;

	// Load it.
	HMODULE hModule = LoadLibrary(DllFilePath);
	// Prevent crash.
	if (!hModule)
		return FALSE;

	FARPROC Function01 = GetProcAddress(hModule, "SetSKSEMode");
	FARPROC Function02 = GetProcAddress(hModule, "Setup");

	// Call stuff.
	Function01();
	Function02();

	return TRUE;
}

#endif