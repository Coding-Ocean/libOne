cbuffer cbWorld : register(b0)
{
    row_major matrix World;
};
cbuffer cbProj : register(b2)
{
    row_major matrix Proj;
};

//--------------------------------------------------------------------------------------
struct VS_INPUT {
    float4 pos : POSITION;
    float2 tex : TEXCOORD;
};

struct PS_INPUT {
    float4 pos : SV_POSITION;
    float2 tex : TEXCOORD;
};

PS_INPUT main(VS_INPUT input) {
    PS_INPUT output = (PS_INPUT)0;
    output.pos = mul(World, input.pos);
    output.pos = mul(Proj, output.pos);
    output.tex = input.tex;
    return output;
}
