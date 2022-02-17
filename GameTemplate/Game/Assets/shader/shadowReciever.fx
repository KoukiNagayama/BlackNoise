/*!
 * @brief �e�����Ƃ���郂�f���p�̃V�F�[�_�[
 */

// ���f���p�̒萔�o�b�t�@�[
cbuffer ModelCb : register(b0)
{
    float4x4 mWorld;
    float4x4 mView;
    float4x4 mProj;
};

cbuffer ShadowCb : register(b1)
{
    float4x4 mLVP;
};

// ���_�V�F�[�_�[�ւ̓���
struct SVSIn
{
    float4 pos : POSITION; // �X�N���[����Ԃł̃s�N�Z���̍��W
    float3 normal : NORMAL; // �@��
    float2 uv : TEXCOORD0; // UV���W
};

// �s�N�Z���V�F�[�_�[�ւ̓���
struct SPSIn
{
    float4 pos : SV_POSITION; // �X�N���[����Ԃł̃s�N�Z���̍��W
    float3 normal : NORMAL; // �@��
    float2 uv : TEXCOORD0; // UV���W


    float4 posInLVP : TEXCOORD1; //���C�g�r���[�X�N���[����Ԃł̃s�N�Z���̍��W
};

///////////////////////////////////////////////////
// �O���[�o���ϐ�
///////////////////////////////////////////////////

Texture2D<float4> g_albedo : register(t0); // �A���x�h�}�b�v
Texture2D<float4> g_shadowMap : register(t10); // �V���h�E�}�b�v
sampler g_sampler : register(s0); // �T���v���[�X�e�[�g

/// <summary>
/// �e�����Ƃ����3D���f���p�̒��_�V�F�[�_�[
/// </summary>
SPSIn VSMain(SVSIn vsIn)
{
    // �ʏ�̍��W�ϊ�
    SPSIn psIn;
    float4 worldPos = mul(mWorld, vsIn.pos);
    psIn.pos = mul(mView, worldPos);
    psIn.pos = mul(mProj, psIn.pos);
    psIn.uv = vsIn.uv;
    psIn.normal = mul(mWorld, vsIn.normal);

    psIn.posInLVP = mul(mLVP, worldPos);
    return psIn;
}

/// <summary>
/// �e�����Ƃ����3D���f���p�̃s�N�Z���V�F�[�_�[
/// </summary>
float4 PSMain(SPSIn psIn) : SV_Target0
{
    float4 color = g_albedo.Sample(g_sampler, psIn.uv);


    float2 shadowMapUV = psIn.posInLVP.xy * psIn.posInLVP.w;
    shadowMapUV *= float2(0.5f, -0.5f);
    shadowMapUV += 0.5f;

    float3 shadowMap = 1.0f;
    if (shadowMapUV.x > 0.0f && shadowMapUV.x < 1.0f
	&& shadowMapUV.y > 0.0f && shadowMapUV.y < 1.0f
)
    {
        shadowMap = g_shadowMap.Sample(g_sampler, shadowMapUV);
    }
    color.xyz *= shadowMap;
    return color;
}
