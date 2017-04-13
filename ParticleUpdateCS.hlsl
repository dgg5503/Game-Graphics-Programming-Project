// ALL BASED ON http://twvideo01.ubm-us.net/o1/vault/GDC2014/Presentations/Gareth_Thomas_Compute-based_GPU_Particle.pdf
// WITH HELP FROM CHRIS Cascioli
// RESET COUNTERS EACH DRAW HERE https://www.gamedev.net/topic/662293-reset-hidden-counter-of-structuredappendconsume-buffers/
// CLEAR ALIVE LIST EACH DRAW
#include "Particle.hlsli"

// Constant info for this frame
cbuffer externalData : register(b0)
{
	float3 cameraPos;	// Position of camera in world space
	float maxAge;		// Time to death, maybe place this in Particle struct?
	float dt;			// Delta time
};

// Actual particle information
// See header for more information about the structs
RWStructuredBuffer<Particle> particlePool		: register(u0); // All particles
RWStructuredBuffer<ParticleAlive> aliveList		: register(u1); // Alive particles (will be used for drawing)
AppendStructuredBuffer<ParticleDead> deadList	: register(u2); // Dead particles

// Ran for all particles
[numthreads(32, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
	// Check if dead from last draw
	if(particlePool[DTid.x].age >= maxAge)
	{
		// If dead, add to deadList, will be re-emitted by the emitter shader
		ParticleDead particleDead;
		particleDead.index = DTid.x;
		deadList.Append(particleDead);
	}
	else
	{
		// Update our particle
		particlePool[DTid.x].age += dt;
		particlePool[DTid.x].worldPos += particlePool[DTid.x].velocity * dt;

		// Grab vector from camera to particle
		float3 camToParticle = particlePool[DTid.x].worldPos - cameraPos;

		// Add to alive list and increment counter
		ParticleAlive particleAlive;
		particleAlive.distanceSq = camToParticle;
		particleAlive.index = DTid.x;
		aliveList[aliveList.IncrementCounter()] = particleAlive;
	}
}