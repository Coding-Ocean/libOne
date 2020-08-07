Texture2D Texture : register(t0);
SamplerState LinearSampler : register(s0);
cbuffer cbColor : register(b3)
{
    float4 MeshColor;
};

struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float2 tex : TEXCOORD;
};

float4 main(PS_INPUT input) : SV_Target
{
    return Texture.Sample(LinearSampler, input.tex) * MeshColor;
}
