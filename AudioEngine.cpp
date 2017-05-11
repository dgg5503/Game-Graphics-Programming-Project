#include <AK/SoundEngine/Common/AkSoundEngine.h>
#include <AK/IBytes.h>
#include <AK/SoundEngine/Common/AkMemoryMgr.h>                  // Memory Manager
#include <AK/SoundEngine/Common/AkModule.h>                     // Default memory and stream managers
#include <AK/SoundEngine/Common/IAkStreamMgr.h>                 // Streaming Manager
#include <AK/MusicEngine/Common/AkMusicEngine.h>                // Music Engine
#include <AK/Tools/Common/AkPlatformFuncs.h>                    // Thread defines
#include <AK/Plugin/AkMatrixReverbFXFactory.h>

// Include for communication between Wwise and the game -- Not needed in the release version
#ifndef AK_OPTIMIZED
#include <AK/Comm/AkCommunication.h>
#endif // AK_OPTIMIZED

#include "AudioEngine.h"
#include "MemoryDebug.h"

// Custom alloc/free functions. These are declared as "extern" in AkMemoryMgr.h
// and MUST be defined by the game developer.
namespace AK
{
#ifdef WIN32
	void * AllocHook(size_t in_size)
	{
		return malloc(in_size);
	}
	void FreeHook(void * in_ptr)
	{
		free(in_ptr);
	}
	// Note: VirtualAllocHook() may be used by I/O pools of the default implementation
	// of the Stream Manager, to allow "true" unbuffered I/O (using FILE_FLAG_NO_BUFFERING
	// - refer to the Windows SDK documentation for more details). This is NOT mandatory;
	// you may implement it with a simple malloc().
	void * VirtualAllocHook(
		void * in_pMemAddress,
		size_t in_size,
		DWORD in_dwAllocationType,
		DWORD in_dwProtect
	)
	{
		return VirtualAlloc(in_pMemAddress, in_size, in_dwAllocationType, in_dwProtect);
	}
	void VirtualFreeHook(
		void * in_pMemAddress,
		size_t in_size,
		DWORD in_dwFreeType
	)
	{
		VirtualFree(in_pMemAddress, in_size, in_dwFreeType);
	}
#endif
}


AudioEngine* AudioEngine::instance = nullptr;

AudioEngine * const AudioEngine::Instance()
{
	assert(instance != nullptr);
	return instance;
}

AudioEngine * const AudioEngine::InitializeInstance()
{
	assert(instance == nullptr);
	instance = new AudioEngine();
	return instance;
}

bool AudioEngine::Initialize()
{
	//
	// Create and initialize an instance of the default memory manager. Note
	// that you can override the default memory manager with your own. Refer
	// to the SDK documentation for more information.
	//

	AkMemSettings memSettings;
	memSettings.uMaxNumPools = 20;

	if (AK::MemoryMgr::Init(&memSettings) != AK_Success)
	{
		assert(!"Could not create the memory manager.");
		return false;
	}

	//
	// Create and initialize an instance of the default streaming manager. Note
	// that you can override the default streaming manager with your own. Refer
	// to the SDK documentation for more information.
	//

	AkStreamMgrSettings stmSettings;
	AK::StreamMgr::GetDefaultSettings(stmSettings);

	// Customize the Stream Manager settings here.

	if (!AK::StreamMgr::Create(stmSettings))
	{
		assert(!"Could not create the Streaming Manager");
		return false;
	}

	//
	// Create a streaming device with blocking low-level I/O handshaking.
	// Note that you can override the default low-level I/O module with your own. Refer
	// to the SDK documentation for more information.        
	//
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

	//
	// Create the Sound Engine
	// Using default initialization parameters
	//

	AkInitSettings initSettings;
	AkPlatformInitSettings platformInitSettings;
	AK::SoundEngine::GetDefaultInitSettings(initSettings);
	AK::SoundEngine::GetDefaultPlatformInitSettings(platformInitSettings);

	if (AK::SoundEngine::Init(&initSettings, &platformInitSettings) != AK_Success)
	{
		assert(!"Could not initialize the Sound Engine.");
		return false;
	}

	//
	// Initialize the music engine
	// Using default initialization parameters
	//

	AkMusicSettings musicInit;
	AK::MusicEngine::GetDefaultInitSettings(musicInit);

	if (AK::MusicEngine::Init(&musicInit) != AK_Success)
	{
		assert(!"Could not initialize the Music Engine.");
		return false;
	}

#ifndef AK_OPTIMIZED
	//
	// Initialize communications (not in release build!)
	//
	AkCommSettings commSettings;
	AK::Comm::GetDefaultInitSettings(commSettings);
	if (AK::Comm::Init(commSettings) != AK_Success)
	{
		assert(!"Could not initialize communication.");
		return false;
	}
#endif // AK_OPTIMIZED

	return true;
}

void AudioEngine::Shutdown()
{
	if (instance)
	{
		instance->Terminate();
		delete instance;
		instance = nullptr;
	}
}

bool AudioEngine::LoadSoundBanks(wchar_t* path)
{
	AKRESULT ret;

	//
	// Setup banks path
	//
	ret = g_lowLevelIO.SetBasePath(path);
	if (ret != AK_Success)
		return false;
	ret = AK::StreamMgr::SetCurrentLanguage(AKTEXT("English(US)"));
	if (ret != AK_Success)
		return false;

	// Allocate a string with enough space to hold "/*.bnk"
	// do we need to have "/" at the end of a valid path from above???
	size_t str_len = wcsnlen(path, MAX_PATH);

	std::unique_ptr<wchar_t[]> fileSearchUPtr(new wchar_t[str_len + 7]);
	wcsncpy(&fileSearchUPtr[0], path, str_len);
	wcsncpy(&fileSearchUPtr[str_len], L"/*.bnk", 7);

	// Get all .bnk files in folder
	AkBankID bankID; // Not used. These banks can be unloaded with their file name.
	AKRESULT eResult;
	WIN32_FIND_DATAW findData;
	HANDLE hFind;
	hFind = FindFirstFileW(&fileSearchUPtr[0], &findData);

	// NOTE: IF IT FAILS HERE, INIT PROBABLY WASNT LOADED YET....
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do {
			//
			// Load banks synchronously (from file name).
			//
			eResult = AK::SoundEngine::LoadBank(findData.cFileName, AK_DEFAULT_POOL_ID, bankID);
			assert(eResult == AK_Success);
		} while (FindNextFileW(hFind, &findData));
		FindClose(hFind);
	}
	//fileSearchUPtr.reset();
	return true;
}

void AudioEngine::ProcessAudio()
{
	// Process bank requests, events, positions, RTPC, etc.
	AK::SoundEngine::RenderAudio();
}

void AudioEngine::Suspend()
{
	AK::SoundEngine::Suspend(false);
}

void AudioEngine::WakeUp()
{
	AK::SoundEngine::WakeupFromSuspend();
	AK::SoundEngine::RenderAudio();
}

AudioEngine::AudioEngine()
{
}


AudioEngine::~AudioEngine()
{
	
}

void AudioEngine::Terminate()
{
#ifndef AK_OPTIMIZED
	// Caution: It is important to terminate the communication modules before the other modules.
	//
	// Terminate Communication Services
	//
	AK::Comm::Term();
#endif // AK_OPTIMIZED

	//
	// Terminate the music engine
	//
	AK::MusicEngine::Term();

	//
	// Terminate the sound engine
	//

	AK::SoundEngine::Term();

	g_lowLevelIO.UnloadAllFilePackages();

	// Terminate the streaming device and streaming manager

	// CAkFilePackageLowLevelIOBlocking::Term() destroys its associated streaming device 
	// that lives in the Stream Manager, and unregisters itself as the File Location Resolver.
	g_lowLevelIO.Term();

	if (AK::IAkStreamMgr::Get())
		AK::IAkStreamMgr::Get()->Destroy();

	// Terminate the Memory Manager
	AK::MemoryMgr::Term();
}
