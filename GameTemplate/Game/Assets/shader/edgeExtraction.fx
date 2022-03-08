/*!
 * @brief エッジ検出
 */

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
    float2 uv = (psIn.posInProj.xy / psIn.posInProj.w) * float2(0.5f, -0.5f) + 0.5f;
    
    // 近傍8テクセルへのUVオフセット
    float2 uvOffset[8] =
    {
        float2(0.0f, 1.0f / 900.0f), //上
        float2(0.0f, -1.0f / 900.0f), //下
        float2(1.0f / 1600.0f, 0.0f), //右
        float2(-1.0f / 1600.0f, 0.0f), //左
        float2(1.0f / 1600.0f, 1.0f / 900.0f), //右上
        float2(-1.0f / 1600.0f, 1.0f / 900.0f), //左上
        float2(1.0f / 1600.0f, -1.0f / 900.0f), //右下
        float2(-1.0f / 1600.0f, -1.0f / 900.0f) //左下
    };

    
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

    // 普通にテクスチャを
    //return g_texture.Sample(g_sampler, psIn.uv);
    // ピクセルカラーを白にしなかったピクセルは黒にする
    return float4(0.0f, 0.0f, 0.0f, 1.0f);
}