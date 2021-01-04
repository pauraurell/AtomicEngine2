#include "ModuleWwise.h"

#include "AK/SoundEngine/Common/AkMemoryMgr.h"        
#include "AK/SoundEngine/Common/AkModule.h"                  
#include "AK/SoundEngine/Common/IAkStreamMgr.h"            
#include "AK/Tools/Common/AkPlatformFuncs.h"
#include "AK/Win32/AkFilePackageLowLevelIOBlocking.h"
#include "AK/MusicEngine/Common/AkMusicEngine.h"            
#include "AK/SpatialAudio/Common/AkSpatialAudio.h"		
#include <AK/SoundEngine/Common/AkMemoryMgr.h>                  
#include <AK/SoundEngine/Common/AkModule.h>                    
#include "AK/Plugin/AkRoomVerbFXFactory.h"
#include <AK/SoundEngine/Common/AkSoundEngine.h>   

#include <assert.h>

//Doc: https://www.audiokinetic.com/library/edge/?source=SDK&id=workingwithsdks.html

CAkFilePackageLowLevelIOBlocking g_lowLevelIO;

namespace AK
{
	void* AllocHook(size_t in_size)
	{
		return malloc(in_size);
	}

	void FreeHook(void* in_ptr)
	{
		free(in_ptr);
	}

	void* VirtualAllocHook(
		void* in_pMemAddress,
		size_t in_size,
		DWORD in_dwAllocationType,
		DWORD in_dwProtect
	)
	{
		return VirtualAlloc(in_pMemAddress, in_size, in_dwAllocationType, in_dwProtect);
	}
	void VirtualFreeHook(
		void* in_pMemAddress,
		size_t in_size,
		DWORD in_dwFreeType
	)
	{
		VirtualFree(in_pMemAddress, in_size, in_dwFreeType);
	}
}

bool ModuleWwise::InitSoundEngine()
{
	//Memory Manager Init
	{
	AkMemSettings memSettings;
	//AK::MemoryMgr::GetDefaultSettings(memSettings);

	if (AK::MemoryMgr::Init(&memSettings) != AK_Success)
	{
		assert(!"Could not create the memory manager.");
		return false;
	}
	}
	//Streaming Manager Init
	{
    // Create and initialize an instance of the default streaming manager. Note
    // that you can override the default streaming manager with your own. 
	AkStreamMgrSettings stmSettings;
	AK::StreamMgr::GetDefaultSettings(stmSettings);

	// Customize the Stream Manager settings here.
	if (!AK::StreamMgr::Create(stmSettings))
	{
		assert(!"Could not create the Streaming Manager");
		return false;
	}

	// Create a streaming device with blocking low-level I/O handshaking.
	// Note that you can override the default low-level I/O module with your own. 
	AkDeviceSettings deviceSettings;

	AK::StreamMgr::GetDefaultDeviceSettings(deviceSettings);

	// Customize the streaming device settings here.
	// CAkFilePackageLowLevelIOBlocking::Init() creates a streaming device
	// in the Stream Manager, and registers itself as the File Location Resolver.

	if (g_lowLevelIO.Init(deviceSettings) != AK_Success)
	{
		assert(!"Could not create the streaming device and Low-Level I/O system");
		return false;
	}
	}

	//Sound Engine Init
	{
	// Create the Sound Engine
	// Using default initialization parameters
	AkInitSettings initSettings;
	AkPlatformInitSettings platformInitSettings;
	AK::SoundEngine::GetDefaultInitSettings(initSettings);
	AK::SoundEngine::GetDefaultPlatformInitSettings(platformInitSettings);

	if (AK::SoundEngine::Init(&initSettings, &platformInitSettings) != AK_Success)
	{
		assert(!"Could not initialize the Sound Engine.");
		return false;
	}
	}

	//Music Engine Init
	{
	// Initialize the music engine
	// Using default initialization parameters

	AkMusicSettings musicInit;
	AK::MusicEngine::GetDefaultInitSettings(musicInit);

	if (AK::MusicEngine::Init(&musicInit) != AK_Success)
	{
		assert(!"Could not initialize the Music Engine.");
		return false;
	}
	}

	//Spartial Audio Init
	{
	// Initialize Spatial Audio
	// Using default initialization parameters
	AkSpatialAudioInitSettings settings; // The constructor fills AkSpatialAudioInitSettings with the recommended default settings. 

	if (AK::SpatialAudio::Init(settings) != AK_Success)
	{
		assert(!"Could not initialize the Spatial Audio.");
		return false;
	}
	}

	//Comms Init
	{
#ifndef AK_OPTIMIZED

	// Initialize communications (not in release build!)

	AkCommSettings commSettings;
	AK::Comm::GetDefaultInitSettings(commSettings);

	if (AK::Comm::Init(commSettings) != AK_Success)
	{
		assert(!"Could not initialize communication.");
		return false;
	}
	}
#endif // AK_OPTIMIZED

	AKRESULT base_path = g_lowLevelIO.SetBasePath(AKTEXT("../Engine/Assets/SoundBanks/"));
	if (base_path != AK_Success)
	{
		LOG("Invalid base path!");
		return false;
	}

	AK::StreamMgr::SetCurrentLanguage(AKTEXT("English(US)"));
	// Load banks synchronously (from file name).
	AkBankID bankID; // Not used. These banks can be unloaded with their file name.

	/*AKRESULT eResult = AK::SoundEngine::LoadBank("Init.bnk", AK_DEFAULT_POOL_ID, bankID);
	if (eResult != AK_Success)
	{
		LOG("Couldn't init the soundbank!");
		return false;
	}*/

	/*assert(eResult == AK_Success);
	eResult = AK::SoundEngine::LoadBank(BANKNAME_CAR, bankID);
	assert(eResult == AK_Success);
	eResult = AK::SoundEngine::LoadBank(BANKNAME_HUMAN, bankID);
	assert(eResult == AK_Success);
	eResult = AK::SoundEngine::LoadBank(BANKNAME_MARKERTEST, bankID);
	assert(eResult == AK_Success);*/

	LOG("Sound Engine Initialized");
	return true;
}


void ModuleWwise::ProcessAudio()
{
	// Process bank requests, events, positions, RTPC, etc.
	AK::SoundEngine::RenderAudio();
}

bool ModuleWwise::TermSoundEngine()
{
#ifndef AK_OPTIMIZED
	// Terminate Communication Services
	AK::Comm::Term();
#endif // AK_OPTIMIZED
	AK::Comm::Term();

	// Terminate the spatial audio
	/*AK::SpatialAudio::Term();*/

	// Terminate the music engine
	AK::MusicEngine::Term();

	// Terminate the sound engine
	AK::SoundEngine::Term();

	// Terminate IO device
	g_lowLevelIO.Term();
	if (AK::IAkStreamMgr::Get())
		AK::IAkStreamMgr::Get()->Destroy();

	// Terminate the Memory Manager
	AK::MemoryMgr::Term();

	return true;
}