/*!
 * @brief エッジ検出
 */

////////////////////////////////////////////////
// 定数
////////////////////////////////////////////////
// 音源の最大個数
#define MAX_DATA 100

////////////////////////////////////////////////
// 構造体
////////////////////////////////////////////////
// 頂点シェーダーへの入力
struct SVSIn
{
    float4 pos : POSITION;
    float2 uv : TEXCOORD0;
};

// ピクセルシェーダーへの入力
struct SPSIn
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
    float4 posInProj : TEXCOORD1;
};

// 音源
struct SoundSourceData
{
    float3 pos;
    int isSound; 
    float range; 
    float3 pad;
};

///////////////////////////////////////////
// 定数バッファー
///////////////////////////////////////////
// モデル用の定数バッファー
cbuffer ModelCb : register(b0)
{
    float4x4 mWorld;
    float4x4 mView;
    float4x4 mProj;
};
// 音源用の定数バッファー
cbuffer SoundSourceCb : register(b1)
{
    SoundSourceData soundSourceData[MAX_DATA];
    int numSoundSource;
};


///////////////////////////////////////////
// シェーダーリソース
///////////////////////////////////////////
Texture2D<float4> g_texture : register(t0); // モデルテクスチャ
Texture2D<float4> g_normalTexture : register(t1); // 法線テクスチャ
Texture2D<float4> g_depthValueTexture : register(t10); // 深度テクスチャ

///////////////////////////////////////////
// サンプラーステート
///////////////////////////////////////////
sampler g_sampler : register(s0); // サンプラー

/// <summary>
/// モデル用の頂点シェーダーのエントリーポイント
/// </summary>
SPSIn VSMain(SVSIn vsIn)
{
    SPSIn psIn;

    psIn.pos = mul(mWorld, vsIn.pos); // モデルの頂点をワールド座標系に変換
    psIn.pos = mul(mView, psIn.pos); // ワールド座標系からカメラ座標系に変換
    psIn.pos = mul(mProj, psIn.pos); // カメラ座標系からスクリーン座標系に変換
    psIn.uv = vsIn.uv;
    psIn.posInProj = psIn.pos; // 頂点の正規化スクリーン座標系の座標をピクセルシェーダーに渡す
    
    return psIn;
}

/// <summary>
/// モデル用のピクセルシェーダーのエントリーポイント
/// </summary>
float4 PSMain(SPSIn psIn) : SV_Target0
{
    // 正規化スクリーン座標系からUV座標系に変換する
    float2 uv = ( psIn.posInProj.xy / psIn.posInProj.w ) * float2( 0.5f, -0.5f ) + 0.5f;
    
    // 近傍8テクセルへのUVオフセット
    float2 uvOffset[8] =
    {
        float2( 0.0f, 1.0f / 900.0f ), //上
        float2( 0.0f, -1.0f / 900.0f ), //下
        float2( 1.0f / 1600.0f, 0.0f ), //右
        float2(-1.0f / 1600.0f, 0.0f ), //左
        float2( 1.0f / 1600.0f, 1.0f / 900.0f ), //右上
        float2(-1.0f / 1600.0f, 1.0f / 900.0f ), //左上
        float2( 1.0f / 1600.0f, -1.0f / 900.0f ), //右下
        float2(-1.0f / 1600.0f, -1.0f / 900.0f ) //左下
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
        // 深度値
        // このピクセルの深度値を取得
        float depth = g_depthValueTexture.Sample(g_sampler, uv).x;
  
        // 近傍8テクセルの深度値の平均値を計算する
        float depth2 = 0.0f;
        for (int i = 0; i < 8; i++)
        {
            depth2 += g_depthValueTexture.Sample(g_sampler, uv + uvOffset[i]).x;
        }
        depth2 /= 8.0f;

        // 自身の深度値と近傍8テクセルの深度値の差を調べる
        if (abs(depth - depth2) > 0.000045f)
        {
            // 深度値が結構違う場合はピクセルカラーを白にする
            return float4(1.0f, 1.0f, 1.0f, 1.0f);
        }
    }
    // 普通にテクスチャを
    //return g_texture.Sample(g_sampler, psIn.uv);
    // ピクセルカラーを黒にする
    return float4(0.0f, 0.0f, 0.0f, 1.0f);
}