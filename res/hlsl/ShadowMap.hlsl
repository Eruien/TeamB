Texture2D g_txDiffuse : register (t0);
Texture2D g_txShadow1 : register (t1);
Texture2D g_txShadow2 : register (t2);
SamplerState g_SamLinear : register (s0);
SamplerState g_samShadowMap : register (s1);

cbuffer cb0 : register (b0)
{
    matrix g_matWorld  : packoffset(c0);    
    matrix g_matView  : packoffset(c4);
    matrix g_matProj  : packoffset(c8);
};

cbuffer cb1 : register (b1)
{
    matrix g_matShadow1;
    matrix g_matShadow2;
};

struct VS_INPUT
{
    float4 p : POSITION;
    float3 n : NORMAL;
    float4 c : COLOR0;
    float2 t : TEXCOORD0;
};

struct VS_OUTPUT
{
    float4 p : SV_POSITION;
    float4 c : COLOR0;
    float2 t : TEXCOORD0;
    float4 TexShadow1 : TEXCOORD1;
    float4 TexShadow2 : TEXCOORD2;
};


VS_OUTPUT VS(VS_INPUT vIn)
{
    VS_OUTPUT vOut = (VS_OUTPUT)0;
    // v * m ->(행->c0,c1,c2,c3)
    // v dot c0 = v.x
    // v dot c1 = v.y
    // v dot c2 = v.z
    // v dot c3 = v.w     
    vOut.p = mul(vIn.p, g_matWorld);
    vOut.p = mul(vOut.p, g_matView);
    vOut.p = mul(vOut.p, g_matProj);
    vOut.t = vIn.t;
    vOut.c = vIn.c;
    // 텍스처좌표
    vOut.TexShadow1 = mul(vIn.p, g_matShadow1);
    vOut.TexShadow2 = mul(vIn.p, g_matShadow2);
    return vOut;
}


struct PS_IN
{
    float4 p : SV_POSITION;
    float2 t : TEXTURE;
};
float4 PS(VS_OUTPUT vIn) : SV_Target
{
    //            r,g,b,a(1)=불투명, a(0)=완전투명, a(0.0< 1.0f)= 반투명
    float4 shadow1 = g_txShadow1.Sample(g_samShadowMap, vIn.TexShadow1.xy / vIn.TexShadow1.w);
    float4 shadow2 = g_txShadow2.Sample(g_samShadowMap, vIn.TexShadow2.xy / vIn.TexShadow2.w);
    float4 FinalColor = g_txDiffuse.Sample(g_SamLinear, vIn.t);

    if (shadow1.r > 0.01f || shadow2.r > 0.01f)
    {
        FinalColor = FinalColor * float4(0.5f, 0.5f, 0.5f, 1.0f);
        FinalColor.w = 1.0f;
    }
    return FinalColor;
}
