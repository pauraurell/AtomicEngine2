#include "ModuleWwise.h"

#include "AK/SoundEngine/Common/AkMemoryMgr.h"        
#include "AK/SoundEngine/Common/AkModule.h"                  
#include "AK/SoundEngine/Common/IAkStreamMgr.h"            
#include "AK/Tools/Common/AkPlatformFuncs.h"
#include "AK/Win32/AkFilePackageLowLevelIOBlocking.h"
#include "AK/MusicEngine/Common/AkMusicEngine.h"            
#include "AK/SpatialAudio/Common/AkSpatialAudio.h"				

#include "AK/Plugin/AkRoomVerbFXFactory.h"

#include <assert.h>

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
