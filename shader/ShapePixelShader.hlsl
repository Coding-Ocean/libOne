cbuffer cbColor : register(b3)
{
    float4 MeshColor;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
};

float4 main(PS_INPUT input) : SV_Target
{
    return MeshColor;
}
