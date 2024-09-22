
cbuffer cbPerObject : register(b0)
{
    float4 color;
};

struct VertexIn
{
    float3 PosL : POSITION;
    float3 NormalL : NORMAL;
    float2 TexcoordL : TEXCOORD;
};

struct VertexOut
{
    float4 PosH : SV_POSITION;
    float3 PosW : POSITION;
    float3 NormalW : NORMAL;
};


VertexOut VS( VertexIn vin )
{
    VertexOut vout;

    vout.PosW = vin.PosL.xyz;
    vout.PosH = float4(vin.PosL, 1.0f);
	return vout;
}

float4 PS (VertexOut pin) : SV_Target
{

    return color;
}