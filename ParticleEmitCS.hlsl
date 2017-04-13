// WITH HELP FROM CHRIS Cascioli
// RESET COUNTERS EACH DRAW HERE https://www.gamedev.net/topic/662293-reset-hidden-counter-of-structuredappendconsume-buffers/
// CLEAR ALIVE LIST EACH DRAW
#include "Particle.hlsli"

// Emitter information
cbuffer externalData : register(b0)
{
	//float3 position;	// World position of emitter
	// Other stuff here
	// Need to implement a random number generator for useful stuff here
};

// Actual particle information
// See header for more information about the structs
RWStructuredBuffer<Particle> particlePool		: register(u0);	// All particles
ConsumeStructuredBuffer<ParticleDead> deadList	: register(u1); // Dead particles coming from AppendStructuredBuffer

// Ran for number of particles in the dead list
[numthreads(32, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
	// Reinitalize all dead particles
	uint pIndex = deadList.Consume().index;
	
	// Create new particle to copy
	Particle particle;
	particle.age = 0;
	particle.color = float4(0.0f, 1.0f, 0.0f, 1.0f);
	particle.worldPos = float3(0, 0, 0);
	particle.velocity = sin(DTid.x); // sample velocity for some sort of variation.

	// Copy it over!
	particlePool[pIndex] = particle;
}