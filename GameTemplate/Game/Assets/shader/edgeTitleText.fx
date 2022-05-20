/*!
 * @brief フォワードレンダリング輪郭線描画
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
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0;
    float3 tangent : TANGENT;
    float3 biNormal : BINORMAL;
};

// ピクセルシェーダーへの入力
struct SPSIn
{
    float4 pos : SV_POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0;
    float4 posInProj : TEXCOORD1;
    float3 tangent : TANGENT;
    float3 biNormal : BINORMAL;
};

// 音源データ
struct SoundSourceData
{
    float3 pos;
    int isSound;
    float range;
    float rateByTime;
    int color;
    float pad;
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
    float colorRatio;
};



///////////////////////////////////////////
// シェーダーリソース
///////////////////////////////////////////
Texture2D<float4> g_texture : register(t0); // モデルテクスチャ
Texture2D<float4> g_depthValueTexture : register(t10); // 深度テクスチャ
Texture2D<float4> g_worldCoordinateTexture : register(t11); // ワールド座標テクスチャ
Texture2D<float4> g_normalTexture : register(t12); // 法線テクスチャ

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

    float4 tex = g_texture.Sample(g_sampler, psIn.uv);
    
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
    
    // 輪郭線を描画するか
    int drawEdge = 0;
    // 配列の番号
    int num = 0;
    // 色の最大値
    float color = 1.0f;
    
    // 同じピクセルに被っている輪郭線の範囲用配列
    int edgeArray[3];
    // 計算後の色の値の最大値格納用
    float maxColor = 0.0000f;
    
    int colorNum = 0;
    
    // ワールド座標
    float3 worldPos = g_worldCoordinateTexture.Sample(g_sampler, uv);

    for (int i = 0; i < numSoundSource; i++)
    {
        if (soundSourceData[i].isSound == 1 || soundSourceData[i].rateByTime > 0.00f)
        {
            // 音源からの距離によって輪郭線を描画するか判断
            float dist = length(worldPos - soundSourceData[i].pos);
            if (dist < soundSourceData[i].range)
            {
                drawEdge++;
                edgeArray[drawEdge - 1] = i;
            }
        }
    }
    
    // ここから輪郭線描画
    if (drawEdge >= 1)
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
        
        
        // 法線
        // このピクセルの法線を取得
        float3 normal = g_normalTexture.Sample(g_sampler, uv).xyz * -8.0f;
        
        // 近傍8テクセルの法線の平均値を計算する
        for (i = 0; i < 8; i++)
        {
            normal += g_normalTexture.Sample(g_sampler, uv + uvOffset[i]).xyz;
        }
        
        // 自身の深度値・法線と近傍8テクセルの深度値の差・法線の差を調べる
        if (abs(depth - depth2) > 0.0000455f || length(normal) >= 0.4f)
        {
            for (int i = 0; i < drawEdge; i++)
            {
                // 音源からピクセルまでの距離
                float dist = length(worldPos - soundSourceData[edgeArray[i]].pos);
                // 距離による影響率
                float rateByDist = 1.0 - pow((dist / soundSourceData[edgeArray[i]].range), 2.5f);
                // 輪郭線の色を計算
                color = 1.0 * soundSourceData[edgeArray[i]].rateByTime * rateByDist;
                // 計算後の色が一番明るいか
                if (color > maxColor)
                {
                    maxColor = color;
                }
            }
            // ピクセルを輪郭線として塗りつぶす
            return float4(maxColor, 0.0f, 0.0f, 1.0f);
        }

    }
    // ピクセルを黒色に塗りつぶす
    return float4(0.0f, 0.0f, 0.0f, 1.0f);
    // デバッグ用 
    //return tex;
}