Texture2D				g_txDiffuse: register (t0);
Texture2D				g_txBlendA	: register(t1);
Texture2D				g_txBlendB	: register(t2);
SamplerState g_samLinear: register (s0);

cbuffer cb0
{
    float4x4	g_matWorld	: packoffset(c0);
    float4x4	g_matView	: packoffset(c4);
    float4x4	g_matProj	: packoffset(c8);
    float4  g_MeshColor     : packoffset(c12);
};
//--------------------------------------------------------------------------------------
struct VS_INPUT
{
    float4 p : POSITION;
    float3 n : NORMAL;
    float4 c : COLOR;
    float2 t : TEXCOORD;
};
struct VS_OUTPUT
{
    float4 p : SV_POSITION;
    float3 n : NORMAL;
    float4 c : COLOR0;
    float2 t : TEXCOORD0;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT)0;
    output.p = mul(input.p, g_matWorld);
    output.p = mul(output.p, g_matView);
    output.p = mul(output.p, g_matProj);
    output.n = input.n;
    output.c = input.c;
    output.t = input.t;

    return output;
}
//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(VS_OUTPUT input) : SV_Target
{
    float4 txColor0 = g_txDiffuse.Sample(g_samLinear, input.t);
    float4 txColor1 = g_txBlendA.Sample(g_samLinear, input.t);
    float4 FinalColor = (txColor0 * (1.0f - txColor1.a) +
                         txColor1 * txColor1.a);// *input.c;
    return FinalColor;
}
float4 LinearBlendAddPS(VS_OUTPUT input) : SV_Target
{
    float4 txColor0 = g_txDiffuse.Sample(g_samLinear, input.t);
    float4 txColor1 = g_txBlendA.Sample(g_samLinear, input.t);
    float4 txColor2 = g_txBlendB.Sample(g_samLinear, input.t);

    float4 AnimColor = lerp(txColor1, txColor2, g_MeshColor.a);
    float4 FinalColor = (txColor0 * (1.0f - AnimColor.a) + AnimColor * AnimColor.a);// * input.c;	
    return FinalColor;
}
float4 LinearBlendPS(VS_OUTPUT input) : SV_Target
{
    float4 txColor0 = g_txDiffuse.Sample(g_samLinear, input.t);
    float4 txColor1 = g_txBlendA.Sample(g_samLinear, input.t);
    float4 txColor2 = g_txBlendB.Sample(g_samLinear, input.t);

    float4 AnimColor = lerp(txColor1, txColor2, g_MeshColor.a);
    float4 FinalColor = (input.c * AnimColor.a + AnimColor * (1.0f - AnimColor.a));// * input.c;	
    FinalColor = (txColor0 * (1.0f - FinalColor.a) + FinalColor * AnimColor.a);// * input.c;	
    return FinalColor;
}
