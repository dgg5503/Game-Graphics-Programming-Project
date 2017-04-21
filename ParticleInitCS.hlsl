// ALL BASED ON http://twvideo01.ubm-us.net/o1/vault/GDC2014/Presentations/Gareth_Thomas_Compute-based_GPU_Particle.pdf
// WITH HELP FROM CHRIS Cascioli
// RESET COUNTERS EACH DRAW HERE https://www.gamedev.net/topic/662293-reset-hidden-counter-of-structuredappendconsume-buffers/
// CLEAR ALIVE LIST EACH DRAW
#include "ShaderConstants.h"
#include "ParticleLayout.h"
//#include "Particle.hlsli"

// Actual particle information
// See header for more information about the structs
AppendStructuredBuffer<ParticleDead> deadList : register(u0); // Dead particles coming from AppendStructuredBuffer
//RWStructuredBuffer<ParticleAlive> aliveList : register(u1); // All alive particles


// Ran for all particles
[numthreads(NUM_PARTICLE_THREADS, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    // zero out all structs
    //Particle zeroParticle = (Particle)0;
   // particlePool[DTid.x] = zeroParticle;
    //ParticleAlive particleAlive;
   // particleAlive.index = 0;
   // particleAlive.distanceSq = 0.0f;
    //particleAlive.padding = float2(0, 0);
   // aliveList[DTid.x] = particleAlive;

    // Append all IDs to dead list so we can emit on first update
    ParticleDead particleDead;
    particleDead.index = DTid.x;
    particleDead.padding = float3(0, 0, 0);
    deadList.Append(particleDead);
}