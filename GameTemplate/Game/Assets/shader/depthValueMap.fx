/*!
 * @brief   �[�x�l�}�b�v
 */

///////////////////////////////////////////////////
// �\����
///////////////////////////////////////////////////

// ���_�V�F�[�_�[�ւ̓���
struct SVSIn
{
    float4 pos : POSITION;          // ���f���̒��_���W
    float3 normal : NORMAL;         // �@��
    float3 tangent : TANGENT;
    float3 biNormal : BINORMAL;
    float2 uv : TEXCOORD0;          // UV���W
};

// �s�N�Z���V�F�[�_�[�ւ̓���
struct SPSIn
{
    float4 pos : SV_POSITION;       // �X�N���[����Ԃł̃s�N�Z���̍��W
    float3 normal : NORMAL;         // �@��
    float3 tangent : TANGENT;       // �ڃx�N�g��
    float3 biNormal : BINORMAL;     // �]�@���x�N�g��
    float2 uv : TEXCOORD0;          // UV���W
    float3 worldPos : TEXCOORD1;    // ���[���h��Ԃł̃s�N�Z���̍��W
    float3 depthInView : TEXCOORD2; // �J������Ԃł̃s�N�Z����Z�l
};

// �s�N�Z���V�F�[�_�[����̏o��
struct SPSOut
{
    float depth : SV_TARGET0;   // �����_�����O�^�[�Q�b�g0�ɕ`��
};

///////////////////////////////////////////
// �萔�o�b�t�@�[
///////////////////////////////////////////
// ���f���p�̒萔�o�b�t�@�[
cbuffer ModelCb : register(b0)
{
    float4x4 mWorld;
    float4x4 mView;
    float4x4 mProj;
};

///////////////////////////////////////////
// �T���v���[�X�e�[�g
///////////////////////////////////////////
sampler g_sampler : register(s0); // �T���v���[

/// <summary>
/// ���_�V�F�[�_�[
/// <summary>
SPSIn VSMain(SVSIn vsIn)
{
    SPSIn psIn;
    psIn.pos = mul(mWorld, vsIn.pos);
    psIn.worldPos = psIn.pos;
    psIn.pos = mul(mView, psIn.pos);
    
    psIn.depthInView = psIn.pos.z;
    psIn.pos = mul(mProj, psIn.pos);
    psIn.normal = normalize(mul(mWorld, vsIn.normal));
    psIn.tangent = normalize(mul(mWorld, vsIn.tangent));
    psIn.biNormal = normalize(mul(mWorld, vsIn.biNormal));
    psIn.uv = vsIn.uv;
    
    return psIn;
}

/// <summary>
/// �s�N�Z���V�F�[�_�[
/// <summary>
SPSOut PSMain(SPSIn psIn)
{
    SPSOut psOut;
    
    // �J������Ԃł̐[�x�l��ݒ�
    psOut.depth = psIn.depthInView;
    
    return psOut;
}