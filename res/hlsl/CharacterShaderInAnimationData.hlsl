#define MAX_BONE_MATRICES 100 
#define MAX_INSTANCING_MATRICES 100 

Texture2D g_txDiffuse1 : register(t0);
Texture2D g_txAnimationArray : register(t2);
SamplerState sample0 : register(s0);

struct VS_INPUT
{
    float3 p : POSITION;
    float3 n : NORMAL;
    float4 c : COLOR;    // COLOR0 ~ COLOR1
    float2 t : TEXCOORD0; // TEXCOORD0 ~ TEXCOORD15

    float4 i : INDEX;
    float4 w : WEIGHT;
    float3 tan : TANGENT;
};

struct VS_OUTPUT
{
    float4 p : SV_POSITION;
    float3 n : NORMAL;
    float4 c : COLOR0;
    float2 t : TEXCOORD0;
};

// 상수버퍼(레지스터 단위로 저장되어야 한다.)
// 레지스터 단위란, float4(x,y,z,w) 
cbuffer cb0 : register (b0)
{
    matrix g_matWorld  : packoffset(c0);// float4x4 // 4개    
    matrix g_matView  : packoffset(c4);
    matrix g_matProj  : packoffset(c8);
};

cbuffer cbAnimMatrices : register (b1)
{
    float4x4 g_matBoneWorld[MAX_BONE_MATRICES];
};

cbuffer cbCurrentFrame : register (b2)
{
    int g_CurrentFrame[4];
};

float4x4 ReadMatrix(int x, int y) {
    float4x4 mat;
    mat[0] = g_txAnimationArray.Load(int3(x * 4, y, 0));
    mat[1] = g_txAnimationArray.Load(int3(x * 4 + 1, y, 0));
    mat[2] = g_txAnimationArray.Load(int3(x * 4 + 2, y, 0));
    mat[3] = g_txAnimationArray.Load(int3(x * 4 + 3, y, 0));
    return mat;
}

VS_OUTPUT VS(VS_INPUT vIn)
{
    VS_OUTPUT vOut = (VS_OUTPUT)0;
    
    /*for (int i = 0; i < MAX_INSTANCING_MATRICES; i++)
    {
        g_matBoneWorld[i] = ReadMatrix(i, g_CurrentFrame[0]);
    }*/

    float4 vLocal = float4(vIn.p.xyz, 1.0f);
    float4 vWorld = 0;
    float3 vNormal = 0;

 /*   for (int iBone = 0; iBone < 4; iBone++)
    {
        uint iBoneIndex = vIn.i[iBone];
        vWorld += mul(vLocal, g_matBoneWorld[iBoneIndex]) * vIn.w[iBone];
        vNormal += mul(vIn.n, (float3x3)g_matBoneWorld[iBoneIndex]) * vIn.w[iBone];
    }*/

    for (int iBone = 0; iBone < 4; iBone++)
    {
        uint iBoneIndex = vIn.i[iBone];
        vWorld += mul(vLocal, ReadMatrix(iBoneIndex, g_CurrentFrame[0])) * vIn.w[iBone];
        vNormal += mul(vIn.n, (float3x3)ReadMatrix(iBoneIndex, g_CurrentFrame[0])) * vIn.w[iBone];
    }

    vWorld = mul(vWorld, g_matWorld);
    float4 vView = mul(vWorld, g_matView);
    float4 vProj = mul(vView, g_matProj);
    vOut.p = vProj;
    vOut.n = vNormal;
    vOut.t = vIn.t;
    vOut.c = vIn.c;
    return vOut;
}

struct PS_IN
{
    float4 p : SV_POSITION;
    float2 t : TEXTURE;
};
float4 PS(VS_OUTPUT vIn) : SV_Target
{
    //float depthValue = vIn.p.z / vIn.p.w;

    //            r,g,b,a(1)=불투명, a(0)=완전투명, a(0.0< 1.0f)= 반투명
    return g_txDiffuse1.Sample(sample0, vIn.t) *vIn.c;
    //return vIn.c;
}