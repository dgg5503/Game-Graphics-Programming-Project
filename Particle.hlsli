// ALL BASED ON http://twvideo01.ubm-us.net/o1/vault/GDC2014/Presentations/Gareth_Thomas_Compute-based_GPU_Particle.pdf
// WITH HELP FROM CHRIS Cascioli

#ifndef PARTICLE_H
#define PARTICLE_H

struct Particle
{
	float4 color;		// Color of particle (solid colors for now)
	float3 worldPos;	// Current WORLD position
	float age;			// Age in seconds (?)
	float3 velocity;	// Current velocity
};

struct ParticleAlive
{
	uint index;			// Index of alive particle in particlePool
	float distanceSq;	// Distance squared of particlePool[index] from camera
};

struct ParticleDead
{
	uint index;			// Index of dead particle in particlePool
};

#endif