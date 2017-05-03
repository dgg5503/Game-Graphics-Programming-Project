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
    float radius;
    float3 Attenuation;
}

cbuffer screenInfo : register(b1)
{
    float2 screenSize;
}

struct DLVStoPS
{
    float4 position : SV_Position;
    float3 worldPos : POSITION;
};

float4 main(DLVStoPS input) : SV_TARGET0
{
    // Sample based on vertex xy in screen space
    float2 uv = input.position.xy / screenSize;
    float3 pos = worldPosTexture.Sample(deferredSampler, uv).xyz;

    // See if we should discard
    float3 centerToPos = Position - pos;
    float distSq = dot(centerToPos, centerToPos);
    clip(radius * radius - distSq);

    float4 col = colorTexture.Sample(deferredSampler, uv);
    

	// revert normals to -1 to 1
    float3 n = (normalsTexture.Sample(deferredSampler, uv).xyz * 2.0f) - 1.0f;

    // dir from object to light
    float3 dir = normalize(centerToPos);

    // normal dot light direction
    float lightAmt = saturate(dot(n, dir));

    // attenuation
    float att = 1.0f / (Attenuation.x + Attenuation.y * sqrt(distSq) + Attenuation.z * distSq);

    // add on
    return DiffuseColor * att * lightAmt;
}