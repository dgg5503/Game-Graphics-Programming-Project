#include "ParticleEmitter.h"


// SingleBurst()
// ContinousBurst(bool val)
// TextureAtlas functionality
// Forward Create Functions functions to renderer
// Possibly remove emitter name

// SetActive HANDLED INTERNALLY
// SetLoopable

ParticleEmitter::ParticleEmitter(unsigned int numParticles) :
	type(ParticleEmitterType::BURST),
	counter(0),
	numLoops(1),
	currNumLoops(1),
	numParticles(numParticles),
	isActive(false),
	isLoopable(false)
{
	// Zero out structs to ensure we're starting fresh.
	memset(&emitter, 0, sizeof(Emitter));

	// Find the closest aligned power for 2 for dispatching compute shaders
	numParticlesAligned = (numParticles - 1 + NUM_PARTICLE_THREADS) & (~(NUM_PARTICLE_THREADS - 1));
}

ParticleEmitter::ParticleEmitter(unsigned int particlesPerRate, float rate) :
	type(ParticleEmitterType::CONTINOUS),
	counter(0),
	numLoops(1),
	currNumLoops(1),
	numParticles(particlesPerRate),
	emitRate(rate),
	isActive(false),
	isLoopable(true)
{
	// Zero out structs to ensure we're starting fresh.
	memset(&emitter, 0, sizeof(Emitter));

	// Find the closest aligned power for 2 for dispatching compute shaders
	numParticlesAligned = (numParticles - 1 + NUM_PARTICLE_THREADS) & (~(NUM_PARTICLE_THREADS - 1));
}


ParticleEmitter::~ParticleEmitter()
{
}

void ParticleEmitter::SetTint(DirectX::XMFLOAT3 & initialTint)
{
	emitter.iMinTint = initialTint;
	emitter.flags &= (~RAND_ITINT);
}

void ParticleEmitter::SetEndTint(DirectX::XMFLOAT3 & endTint)
{
	emitter.eMinTint = endTint;
	emitter.flags &= (~RAND_ETINT);
}

void ParticleEmitter::SetSpeed(float initialSpeed)
{
	emitter.iMinSpeed = initialSpeed;
	emitter.flags &= (~RAND_ISPEED);
}

void ParticleEmitter::SetEndSpeed(float endSpeed)
{
	emitter.eMinSpeed = endSpeed;
	emitter.flags &= (~RAND_ESPEED);
}

void ParticleEmitter::SetSize(DirectX::XMFLOAT2 & initialSize)
{
	emitter.iMinSize = initialSize;
	emitter.flags &= (~RAND_ISIZE);
}

void ParticleEmitter::SetEndSize(DirectX::XMFLOAT2 & endSize)
{
	emitter.eMinSize = endSize;
	emitter.flags &= (~RAND_ESIZE);
}

void ParticleEmitter::SetAlpha(float initialAlpha)
{
	emitter.iAlpha = initialAlpha;
}

void ParticleEmitter::SetEndAlpha(float endAlpha)
{
	emitter.eAlpha = endAlpha;
}

void ParticleEmitter::SetDirection(DirectX::XMFLOAT3 & dir)
{
	emitter.minXYZTheta = dir;
	emitter.flags &= (~RAND_DIR);
}

void ParticleEmitter::SetAge(float age)
{
	emitter.minAge = age;
	emitter.flags &= (~RAND_AGE);
}

void ParticleEmitter::SetPosition(const DirectX::XMFLOAT3 & pos)
{
	emitter.position = pos;
}

void ParticleEmitter::SetNonce(unsigned int nonce)
{
	emitter.nonce = nonce;
}

bool ParticleEmitter::SetTextureID(unsigned int textureID)
{
	// If larger then 1 << TEXT_ID_SIZE, wont fit in options
	if (textureID > ((1 << (TEXT_ID_SIZE + 1)) - 1))
		return false;
	
	// Create mask to preserve lower 12 bits
	unsigned int mask = 0xFFF;

	// And with mask to wipe out top 20
	emitter.flags &= mask;

	// Bit shift texture ID to left 12 bits
	textureID <<= ((sizeof(unsigned int) * 8) - TEXT_ID_SIZE);

	// Or with flags (done...)
	emitter.flags |= textureID;
	return true;
}

void ParticleEmitter::SetInitialTintRange(DirectX::XMFLOAT3 & initialMinTint, DirectX::XMFLOAT3 & initialMaxTint)
{
	emitter.iMinTint = initialMinTint;
	emitter.iMaxTint = initialMaxTint;
	emitter.flags |= RAND_ITINT;
}

void ParticleEmitter::SetEndTintRange(DirectX::XMFLOAT3 & endMinTint, DirectX::XMFLOAT3 & endMaxTint)
{
	emitter.eMinTint = endMinTint;
	emitter.eMaxTint = endMaxTint;
	emitter.flags |= RAND_ETINT;
}

void ParticleEmitter::SetInitialSpeedRange(float initialMinSpeed, float initialMaxSpeed)
{
	emitter.iMinSpeed = initialMinSpeed;
	emitter.iMaxSpeed = initialMaxSpeed;
	emitter.flags |= RAND_ISPEED;
}

void ParticleEmitter::SetEndSpeedRange(float endMinSpeed, float endMaxSpeed)
{
	emitter.eMinSpeed = endMinSpeed;
	emitter.eMaxSpeed = endMaxSpeed;
	emitter.flags |= RAND_ESPEED;
}

void ParticleEmitter::SetInitialSizeRange(DirectX::XMFLOAT2 & initialMinSize, DirectX::XMFLOAT2 & initialMaxSize)
{
	emitter.iMinSize = initialMinSize;
	emitter.iMaxSize = initialMaxSize;
	emitter.flags |= RAND_ISIZE;
}

void ParticleEmitter::SetEndSizeRange(DirectX::XMFLOAT2 & endMinSize, DirectX::XMFLOAT2 & endMaxSize)
{
	emitter.eMinSize = endMinSize;
	emitter.eMaxSize = endMaxSize;
	emitter.flags |= RAND_ESIZE;
}

void ParticleEmitter::SetDirectionRange(DirectX::XMFLOAT3 & minDir, DirectX::XMFLOAT3 & maxDir)
{
	emitter.minXYZTheta = minDir;
	emitter.maxXYZTheta = maxDir;
	emitter.flags |= RAND_DIR;
}

void ParticleEmitter::SetAgeRange(float minAge, float maxAge)
{
	emitter.minAge = minAge;
	emitter.maxAge = maxAge;
	emitter.flags |= RAND_AGE;
}

void ParticleEmitter::SetInterpAlpha(bool val)
{
	if(val)
		emitter.flags |= INTERP_ALPHA;
	else
		emitter.flags &= (~INTERP_ALPHA);
}

void ParticleEmitter::SetInterpSpeed(bool val)
{
	if (val)
		emitter.flags |= INTERP_SPEED;
	else
		emitter.flags &= (~INTERP_SPEED);
}

void ParticleEmitter::SetInterpSize(bool val)
{
	if (val)
		emitter.flags |= INTERP_SIZE;
	else
		emitter.flags &= (~INTERP_SIZE);
}

void ParticleEmitter::SetInterpTint(bool val)
{
	if (val)
		emitter.flags |= INTERP_TINT;
	else
		emitter.flags &= (~INTERP_TINT);
}

// Set loop amount to...
//	-1 for infinite loop
//	 0 to stop emission
//	 # for a number of loops
void ParticleEmitter::SetLoop(int loopAmount = -1)
{
	numLoops = loopAmount;
	currNumLoops = numLoops;
}

void ParticleEmitter::Emit()
{
	// If already active, do nothing
	if (isActive)
		return;

	// Reset timer and num particles to emit
	// Both types require numParticles to be set
	/*
	emitter.numToEmit = numParticles;
	switch (type)
	{
	case ParticleEmitterType::BURST:
		// Based on max possible lifetime
		counter = DirectX::XMMax<float>(emitter.minAge, emitter.maxAge);
		break;
	case ParticleEmitterType::CONTINOUS:
		// Based on emit rat provided
		counter = emitRate;
		break;
	default:
		break;
	}
	*/

	// Set active
	isActive = true;
	
	// Reset loop count
	currNumLoops = numLoops;
}

// TODO: Pretty messy D:... Will come back to clean up later.
bool ParticleEmitter::CanEmit(float dt)
{
	if (isActive)
	{
		// Decrease counter
		counter -= dt;
		if (counter < 0.0f)
		{
			emitter.numToEmit = numParticles;
			switch (type)
			{
			case ParticleEmitterType::BURST:
				// Based on max possible lifetime
				counter = DirectX::XMMax<float>(emitter.minAge, emitter.maxAge);
				break;
			case ParticleEmitterType::CONTINOUS:
				// Based on emit rat provided
				counter = emitRate;
				break;
			default:
				break;
			}

			if (currNumLoops >= 0 && currNumLoops-- == 0)
			{
				isActive = false;
				counter = 0;
			}

			return isActive;
		}
		return false;
	}
	return false;
}