struct PS_INPUT {
    float4 pos : SV_POSITION;
    float4 color : COLOR0;
};

float4 main(PS_INPUT i) : SV_Target{
    return i.color;
}
