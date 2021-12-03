#include"LambertCB.hlsli"

struct IN {
    float4 pos     : POSITION;
	float3 normal  : NORMAL;
    float2 tex     : TEXCOORD;
	int4 worldIdx  : BLENDINDICES;
	float4 weights : BLENDWEIGHT;
};

struct OUT {
    float4 pos     : SV_POSITION;
	float4 diffuse : COLOR;
    float2 tex     : TEXCOORD;
};

OUT main( IN i ) {    
    OUT o = (OUT)0;

    float4 pos;
    float3 normal;
    pos     = i.weights.x * mul( WorldArray[ i.worldIdx.x ], i.pos );
    normal  = i.weights.x * mul( (float3x3)WorldArray[ i.worldIdx.x ], i.normal );
    pos    += i.weights.y * mul( WorldArray[ i.worldIdx.y ], i.pos );
    normal += i.weights.y * mul( (float3x3)WorldArray[ i.worldIdx.y ], i.normal );
    pos    += i.weights.z * mul( WorldArray[ i.worldIdx.z ], i.pos );
    normal += i.weights.z * mul( (float3x3)WorldArray[ i.worldIdx.z ], i.normal );
    pos    += i.weights.w * mul( WorldArray[ i.worldIdx.w ], i.pos );
    normal += i.weights.w * mul( (float3x3)WorldArray[ i.worldIdx.w ], i.normal );

    o.pos = mul( ProjView, pos );
	
    normal = normalize( normal );
    o.diffuse.rgb = Diffuse.rgb * max( dot( normal, LightPos ), Ambient );
    //o.diffuse.rgb = Diffuse.rgb *  max( ( dot( normal, LightPos ) * 0.5f + 0.5f ), Ambient );
    o.diffuse.a = Diffuse.a;

	o.tex = i.tex;
    
    return o;
}
