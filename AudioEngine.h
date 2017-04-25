#pragma once

#include <AK/SoundEngine/Common/AkSoundEngine.h>
#include <AK/IBytes.h>
#include <AK/SoundEngine/Common/AkMemoryMgr.h>                  // Memory Manager
#include <AK/SoundEngine/Common/AkModule.h>                     // Default memory and stream managers
#include <AK/SoundEngine/Common/IAkStreamMgr.h>                 // Streaming Manager
#include <AK/MusicEngine/Common/AkMusicEngine.h>                // Music Engine
#include <AK/Tools/Common/AkPlatformFuncs.h>                    // Thread defines
#include "AudioIO/AkFilePackageLowLevelIOBlocking.h"			// Sample low-level I/O implementation
#include <assert.h>
#include <vector>
#include <memory>

// -- TEMPORARY, REMOVE THIS LATER --
#include <stdio.h>

// Include for communication between Wwise and the game -- Not needed in the release version
#ifndef AK_OPTIMIZED
	#include <AK/Comm/AkCommunication.h>
#endif // AK_OPTIMIZED

class AudioEngine
{
public:
	static AudioEngine * const InitializeInstance();
	static AudioEngine * const Instance();
	bool Initialize();
	void Shutdown();
	
	bool LoadSoundBanks(wchar_t* path);
	void ProcessAudio();
	void Suspend();
	void WakeUp();

private:
	AudioEngine();
	~AudioEngine();
	static AudioEngine* instance;

	CAkFilePackageLowLevelIOBlocking g_lowLevelIO;
};

