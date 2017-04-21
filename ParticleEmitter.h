#pragma once

// DEBUG
#include <stdio.h>

#include <memory.h>
#include "EmitterLayout.h"
#include "ParticleLayout.h"
#include "ShaderConstants.h"

// MUST MATCH THAT OF OPTIONS MENTIONED IN PARTICLELAYOUT.H 
// Defines all options available by the emitter.
/*
struct EmitterOptions
{
	unsigned int RandAge		: 1; // Generate a random age in range?
	unsigned int InterpAlpha	: 1; // Interpolate between alpha values?
	unsigned int RandDirection	: 1; // Generate a random direction in range?
	unsigned int RandInitSpeed	: 1; // Generate random initial speed in range?
	unsigned int RandEndSpeed	: 1; // Generate random end speed in range?
	unsigned int RandInitSize	: 1; // Generate random initial size in range?
	unsigned int RandEndSize	: 1; // Generate random end size in range?
	unsigned int RandInitTint	: 1; // Generate random initial tint in range?
	unsigned int RandEndTint	: 1; // Generate random end tint in range?
	unsigned int InterpSpeed	: 1; // Interpolate between speed values over lifetime?
	unsigned int InterpSize		: 1; // Interpolate between sizes over lifetime?
	unsigned int InterpTint		: 1; // Interpolate tint over lifetime?
	unsigned int TextID			: TEXT_ID_SIZE; // The ID of the particles texture
};
*/

enum class ParticleEmitterType
{
	BURST,		// numParticles emitted ONCE, wait for emitter lifetime to die down
	CONTINOUS	// numParticles, but particles emitted every some number of frames
};

class ParticleEmitter
{
	// Allow particle renderer to directly reference emitter information
	friend class ParticleRenderer;
public:
	// Non-random
	void SetTint(DirectX::XMFLOAT3& initialTint);
	void SetEndTint(DirectX::XMFLOAT3& endTint);
	void SetSpeed(float initialSpeed);
	void SetEndSpeed(float endSpeed);
	void SetSize(DirectX::XMFLOAT2& initialSize);
	void SetEndSize(DirectX::XMFLOAT2& endSize);
	void SetAlpha(float initialAlpha);
	void SetEndAlpha(float endAlpha);
	void SetDirection(DirectX::XMFLOAT3& dir);
	void SetAge(float age);
	void SetPosition(DirectX::XMFLOAT3& pos);
	void SetNonce(unsigned int nonce);
	bool SetTextureID(unsigned int textureID);

	// Random ranges
	// If any of these are set, the initial values set using the correlating functions above
	// are overwritten and ignored.
	void SetInitialTintRange(DirectX::XMFLOAT3& initialMinTint, DirectX::XMFLOAT3& initialMaxTint);
	void SetEndTintRange(DirectX::XMFLOAT3& endMinTint, DirectX::XMFLOAT3& endMaxTint);
	void SetInitialSpeedRange(float initialMinSpeed, float initialMaxSpeed);
	void SetEndSpeedRange(float endMinSpeed, float endMaxSpeed);
	void SetInitialSizeRange(DirectX::XMFLOAT2& initialMinSize, DirectX::XMFLOAT2& initialMaxSize);
	void SetEndSizeRange(DirectX::XMFLOAT2& endMinSize, DirectX::XMFLOAT2& endMaxSize);
	void SetDirectionRange(DirectX::XMFLOAT3& minDir, DirectX::XMFLOAT3& maxDir);
	void SetAgeRange(float minAge, float maxAge);

	// Interp options
	void SetInterpAlpha(bool val);
	void SetInterpSpeed(bool val);
	void SetInterpSize(bool val);
	void SetInterpTint(bool val);
	
	// Emitter specific options
	void SetLoopable(bool val);
private:
	ParticleEmitter(unsigned int numParticles);
	ParticleEmitter(unsigned int particlesPerRate, float rate);
	~ParticleEmitter();

	bool CanEmit(float dt);

	Emitter emitter;
	ParticleEmitterType type;
	float counter; // as long as the emitters largest age, max(minAge, maxAge), no overconsumption
	float emitRate;
	unsigned int numParticles;
	unsigned int numParticlesAligned;
	bool isActive;
	bool isLoopable;
};

