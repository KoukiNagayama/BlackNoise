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
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0;
    float3 tangent : TANGENT;
    float3 biNormal : BINORMAL;
};

// �s�N�Z���V�F�[�_�[�ւ̓���
struct SPSIn
{
    float4 pos : SV_POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0;
    float4 posInProj : TEXCOORD1;
    float3 tangent : TANGENT;
    float3 biNormal : BINORMAL;                             
};

// ����
/*
struct SoundSourceData
{
    float3 pos;
    int isSound; 
    float range;
    float rate;
    float currentRate;
    int pad;
};*/

struct SoundSourceData
{
    float3 pos;
    int isSound;
    float range;
    float rateByTime;
    float2 pad;
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
Texture2D<float4> g_texture : register(t0);                 // ���f���e�N�X�`��
Texture2D<float4> g_depthValueTexture : register(t10);      // �[�x�e�N�X�`��
Texture2D<float4> g_worldCoordinateTexture : register(t11); // ���[���h���W�e�N�X�`��
Texture2D<float4> g_normalTexture : register(t12);          // �@���e�N�X�`��

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

    float4 tex = g_texture.Sample(g_sampler, psIn.uv);
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
    
    int num = 0;
    
    float color = 1.0f;
    
    //float rate;
    
    // ���[���h���W
    float3 worldPos = g_worldCoordinateTexture.Sample(g_sampler, uv);

    for (int i = 0; i < numSoundSource ; i++)
    {
        if (soundSourceData[i].isSound == 1 || soundSourceData[i].rateByTime > 0.00f)
        {
            // ��������̋����ɂ���ė֊s����`�悷�邩���f
            float dist = length(worldPos - soundSourceData[i].pos);
            if(dist < soundSourceData[i].range)
            {
                drawEdge = 1;
                num = i;
            }
        }
    }
    
    if(drawEdge == 1)
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
        
        
        // �@��
        // ���̃s�N�Z���̖@�����擾
        float3 normal = g_normalTexture.Sample(g_sampler, uv).xyz * -8.0f;
        
        // �ߖT8�e�N�Z���̖@���̕��ϒl���v�Z����
        for (i = 0; i < 8; i++)
        {
            normal += g_normalTexture.Sample(g_sampler, uv + uvOffset[i]).xyz;
        }
        
        // ���g�̐[�x�l�E�@���ƋߖT8�e�N�Z���̐[�x�l�̍��E�@���̍��𒲂ׂ�
        if (abs(depth - depth2) > 0.000045f || length(normal) >= 0.35f)
        {
            // ��������s�N�Z���܂ł̋���
            float dist = length(worldPos - soundSourceData[num].pos);
            // �����ɂ��e����
            float rateByDist = 1.0 - pow((dist / soundSourceData[num].range), 2.5f);
            // �֊s���̐F���v�Z
            color = 1.0 * soundSourceData[num].rateByTime * rateByDist;
            // �[�x�l�܂��͖@�����傫���Ⴄ�ꍇ�̓s�N�Z����֊s���Ƃ��ēh��Ԃ�
            return float4(color, color, color, 1.0f);
            
        }
    }

    // �s�N�Z�������F�ɓh��Ԃ�
    return float4(0.0f, 0.0f, 0.0f, 1.0f);
    //xxxxxxxxxxxxreturn tex;

}