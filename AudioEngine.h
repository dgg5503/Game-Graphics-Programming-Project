#pragma once

#include "AudioIO/AkFilePackageLowLevelIOBlocking.h"			// Sample low-level I/O implementation
#include <assert.h>
#include <vector>
#include <memory>
#include "Wwise_IDs.h"

// -- TEMPORARY, REMOVE THIS LATER --
#include <stdio.h>

class AudioEngine
{
public:
	static AudioEngine * const InitializeInstance();
	static AudioEngine * const Instance();
	static void Shutdown();
	bool Initialize();
	
	
	bool LoadSoundBanks(wchar_t* path);
	void ProcessAudio();
	void Suspend();
	void WakeUp();

private:
	AudioEngine();
	~AudioEngine();
	static AudioEngine* instance;

	void Terminate();

	CAkFilePackageLowLevelIOBlocking g_lowLevelIO;
};

