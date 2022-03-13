/*!
 * @brief �G�b�W���o
 */

////////////////////////////////////////////////
// �萔
////////////////////////////////////////////////
// �����̍ő��
#define MAX_DATA 100

////////////////////////////////////////////////
// �\����
////////////////////////////////////////////////
// ���_�V�F�[�_�[�ւ̓���
struct SVSIn
{
    float4 pos : POSITION;
    float2 uv : TEXCOORD0;
};

// �s�N�Z���V�F�[�_�[�ւ̓���
struct SPSIn
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
    float4 posInProj : TEXCOORD1;
};

// ����
struct SoundSourceData
{
    float3 pos;
    int isSound; 
    float range; 
    float3 pad;
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
// �����p�̒萔�o�b�t�@�[
cbuffer SoundSourceCb : register(b1)
{
    SoundSourceData soundSourceData[MAX_DATA];
    int numSoundSource;
};


///////////////////////////////////////////
// �V�F�[�_�[���\�[�X
///////////////////////////////////////////
Texture2D<float4> g_texture : register(t0); // ���f���e�N�X�`��
Texture2D<float4> g_normalTexture : register(t1); // �@���e�N�X�`��
Texture2D<float4> g_depthValueTexture : register(t10); // �[�x�e�N�X�`��

///////////////////////////////////////////
// �T���v���[�X�e�[�g
///////////////////////////////////////////
sampler g_sampler : register(s0); // �T���v���[

/// <summary>
/// ���f���p�̒��_�V�F�[�_�[�̃G���g���[�|�C���g
/// </summary>
SPSIn VSMain(SVSIn vsIn)
{
    SPSIn psIn;

    psIn.pos = mul(mWorld, vsIn.pos); // ���f���̒��_�����[���h���W�n�ɕϊ�
    psIn.pos = mul(mView, psIn.pos); // ���[���h���W�n����J�������W�n�ɕϊ�
    psIn.pos = mul(mProj, psIn.pos); // �J�������W�n����X�N���[�����W�n�ɕϊ�
    psIn.uv = vsIn.uv;
    psIn.posInProj = psIn.pos; // ���_�̐��K���X�N���[�����W�n�̍��W���s�N�Z���V�F�[�_�[�ɓn��
    
    return psIn;
}

/// <summary>
/// ���f���p�̃s�N�Z���V�F�[�_�[�̃G���g���[�|�C���g
/// </summary>
float4 PSMain(SPSIn psIn) : SV_Target0
{
    // ���K���X�N���[�����W�n����UV���W�n�ɕϊ�����
    float2 uv = ( psIn.posInProj.xy / psIn.posInProj.w ) * float2( 0.5f, -0.5f ) + 0.5f;
    
    // �ߖT8�e�N�Z���ւ�UV�I�t�Z�b�g
    float2 uvOffset[8] =
    {
        float2( 0.0f, 1.0f / 900.0f ), //��
        float2( 0.0f, -1.0f / 900.0f ), //��
        float2( 1.0f / 1600.0f, 0.0f ), //�E
        float2(-1.0f / 1600.0f, 0.0f ), //��
        float2( 1.0f / 1600.0f, 1.0f / 900.0f ), //�E��
        float2(-1.0f / 1600.0f, 1.0f / 900.0f ), //����
        float2( 1.0f / 1600.0f, -1.0f / 900.0f ), //�E��
        float2(-1.0f / 1600.0f, -1.0f / 900.0f ) //����
    };
    
    int drawEdge = 0;
    for (int i = 0; i < numSoundSource ; i++)
    {
        if (soundSourceData[i].isSound == 1)
        {
            drawEdge = 1;
        }
    }
    if(drawEdge != 0)
    {
        // �[�x�l
        // ���̃s�N�Z���̐[�x�l���擾
        float depth = g_depthValueTexture.Sample(g_sampler, uv).x;
  
        // �ߖT8�e�N�Z���̐[�x�l�̕��ϒl���v�Z����
        float depth2 = 0.0f;
        for (int i = 0; i < 8; i++)
        {
            depth2 += g_depthValueTexture.Sample(g_sampler, uv + uvOffset[i]).x;
        }
        depth2 /= 8.0f;

        // ���g�̐[�x�l�ƋߖT8�e�N�Z���̐[�x�l�̍��𒲂ׂ�
        if (abs(depth - depth2) > 0.000045f)
        {
            // �[�x�l�����\�Ⴄ�ꍇ�̓s�N�Z���J���[�𔒂ɂ���
            return float4(1.0f, 1.0f, 1.0f, 1.0f);
        }
    }
    // ���ʂɃe�N�X�`����
    //return g_texture.Sample(g_sampler, psIn.uv);
    // �s�N�Z���J���[�����ɂ���
    return float4(0.0f, 0.0f, 0.0f, 1.0f);
}