cbuffer b0 : register( b0 ) {
	float3 LightPos;
	float Ambient;
};

cbuffer b1 : register( b1 ) {
    float4 Diffuse;
};

cbuffer b6 : register( b6 ) {
    row_major matrix ProjView;
};

cbuffer b7 : register( b7 ) {
    row_major matrix World;
};

cbuffer b8 : register( b8 ){ 
	row_major matrix WorldArray[2]       : packoffset(c0);
	row_major matrix WorldArrayDmmy[253] : packoffset(c8);//4*2
	row_major matrix WorldArrayEnd       : packoffset(c1020);//4*(2+253)
}