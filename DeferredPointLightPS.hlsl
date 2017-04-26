// Lighting information to sample from
Texture2D colorTexture : register(t0);
Texture2D worldPosTexture : register(t1);
Texture2D normalsTexture : register(t2);
Texture2D emissionTexture : register(t3);
SamplerState deferredSampler : register(s0);

// Point light information
cbuffer externalData : register(b0)
{
    float4 DiffuseColor; // 16
    float3 Position; // 12 (16)
    float Intensity;
}

struct DLVStoPS
{ 
    float4 position : SV_Position;
    float3 worldPos : POSITION;
};

float4 main(DLVStoPS input) : SV_TARGET
{
    // Sample based on 
    //float4 col = colorTexture.Sample(deferredSampler, input.uv);
   // float3 pos = worldPosTexture.Sample(deferredSampler, input.uv).xyz;

	// revert normals to -1 to 1
   // float3 n = (normalsTexture.Sample(deferredSampler, input.uv).xyz * 2.0f) - 1.0f;
    // Inside sphere...
	return float4(1.0f, 0.0f, 0.0f, 1.0f);
}