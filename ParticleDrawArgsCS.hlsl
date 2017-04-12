// ALL BASED ON http://twvideo01.ubm-us.net/o1/vault/GDC2014/Presentations/Gareth_Thomas_Compute-based_GPU_Particle.pdf
// WITH HELP FROM CHRIS Cascioli
// RESET COUNTERS EACH DRAW HERE https://www.gamedev.net/topic/662293-reset-hidden-counter-of-structuredappendconsume-buffers/
// CLEAR ALIVE LIST EACH DRAW
#include "Particle.hlsli"

// Matching args found from https://msdn.microsoft.com/en-us/library/windows/desktop/ff476410(v=vs.85).aspx
// Need this since we are storing the arguments for the command above on the GPU in a buffer
struct DrawArgs
{
	uint indexCountPerInstance;
	uint instanceCount;
	uint startIndexLocation;
	int baseVertexLocation;
	uint startInstanceLocation;
};

RWStructuredBuffer<ParticleAlive> aliveList : register(u0); // Alive particles (will be used for drawing)

// Simple compute shader that runs one time which will store relevant info
// for the DrawIndexedInstancedIndirect command
[numthreads(1, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
	DrawArgs args;

	// Get number of particles that are alive
	uint instanceCount;
	uint stride;
	aliveList.GetDimensions(instanceCount, stride);

	// THIS ARG IS PROBABLY WRONG
	args.indexCountPerInstance = 6;	// Unsure if this is number of indices per particle OR the current particle to be drawn?
	args.instanceCount = instanceCount;
	args.startIndexLocation = 0;
	args.baseVertexLocation = 0;
	args.startInstanceLocation = 0;
}