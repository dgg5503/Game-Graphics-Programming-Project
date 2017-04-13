// ALL BASED ON http://twvideo01.ubm-us.net/o1/vault/GDC2014/Presentations/Gareth_Thomas_Compute-based_GPU_Particle.pdf
// WITH HELP FROM CHRIS Cascioli
// RESET COUNTERS EACH DRAW HERE https://www.gamedev.net/topic/662293-reset-hidden-counter-of-structuredappendconsume-buffers/
// CLEAR ALIVE LIST EACH DRAW
#include "Particle.hlsli"
#include "Vertex.hlsli"

// World information
cbuffer externalData : register(b0)
{
	matrix view;
	matrix projection;
};

// Particle information to take from
RWStructuredBuffer<Particle> particlePool	: register(u0); // All particles
RWStructuredBuffer<ParticleAlive> aliveList : register(u1); // Alive particles (will be used for drawing) // THIS IS PROBABLY WRONG

VertexToPixel main(uint i_id : SV_InstanceID, uint v_id : SV_VertexID)
{
	// Each particle has 4 verts with 6 indices so in order to draw 2 triangles
	// we need to have 0,1,2,
	uint particleVertexID = i_id % 4; // 0, 1, 2, 3, // THIS IS PROBABLY WRONG
	uint particleIndexID = i_id / 4; // 0, 0, 0, 0, // THIS IS PROBABLY WRONG

	// When billboarding, normal always facing directly towards camera
	// Obtain view matrix Up and Right
	float3 halfRight = normalize(view._11_21_31) * 0.5f;
	float3 halfUp = normalize(view._12_22_32) * 0.5f;
	float3 n = normalize(view._13_23_33);
	float3 verts[4] =
	{
		-halfRight - halfUp,
		halfRight - halfUp,
		-halfRight + halfUp,
		halfRight + halfUp
	};

	// Inidices would be 
	// 0, 1, 2, 1, 3, 2
	// 0, 1, 2, 2, 1, 3
	// 0, 1, 2, 3, 2, 1
	// Grab current particle
	Particle particle = particlePool[aliveList[i_id].index];
	
	// Get ready to output to PS
	VertexToPixel output;

	// Calculate WVP for each particle
	// Because normal always faces camera, its just the opposite of the forward. 
	output.normal = -n;
	output.tangent = halfRight * 2.0f;
	output.worldPos = particle.worldPos + verts[particleIndexID]; // THIS IS PROBABLY WRONG CUZ INDEX
	output.position = mul(mul(float4(output.worldPos, 1.0f), view), projection);
	output.uv = particle.color.xy; // TEMPORARY, ACTUALLY SUPPLY UV COORDS HERE LATER!

	// Move on to pixel shader (deferred pixel shader)
	return output;
}