#include "GlobalAmbiance.h"

void GlobalAmbiance::Init()
{
	AK::SoundEngine::RegisterGameObj(static_cast<AkGameObjectID>(2));
}

void GlobalAmbiance::EmitAmbiance()
{
	AK::SoundEngine::PostEvent(AK::EVENTS::GLOBAL_AMBIANCE, static_cast<AkGameObjectID>(2));
}

void GlobalAmbiance::ShutDown()
{
	AK::SoundEngine::UnregisterGameObj(static_cast<AkGameObjectID>(2));
}
