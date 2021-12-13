cbuffer cbWorld: register(b0)
{
    row_major matrix World;
};
cbuffer cbProj : register(b2)
{
    row_major matrix Proj;
};


struct VS_INPUT
{
    float4 Pos : POSITION;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
};


PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output = (PS_INPUT)0;
    output.Pos = mul(World, input.Pos);
    output.Pos = mul(Proj, output.Pos);

    return output;
}
