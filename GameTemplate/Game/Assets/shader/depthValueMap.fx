/*!
 * @brief   深度値マップ
 */

///////////////////////////////////////////////////
// 構造体
///////////////////////////////////////////////////

// 頂点シェーダーへの入力
struct SVSIn
{
    float4 pos : POSITION;          // モデルの頂点座標
    float3 normal : NORMAL;         // 法線
    float3 tangent : TANGENT;
    float3 biNormal : BINORMAL;
    float2 uv : TEXCOORD0;          // UV座標
};

// ピクセルシェーダーへの入力
struct SPSIn
{
    float4 pos : SV_POSITION;       // スクリーン空間でのピクセルの座標
    float3 normal : NORMAL;         // 法線
    float3 tangent : TANGENT;       // 接ベクトル
    float3 biNormal : BINORMAL;     // 従法線ベクトル
    float2 uv : TEXCOORD0;          // UV座標
    float3 worldPos : TEXCOORD1;    // ワールド空間でのピクセルの座標
    float3 depthInView : TEXCOORD2; // カメラ空間でのピクセルのZ値
};

// ピクセルシェーダーからの出力
struct SPSOut
{
    float depth : SV_TARGET0;   // レンダリングターゲット0に描画
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
// サンプラーステート
///////////////////////////////////////////
sampler g_sampler : register(s0); // サンプラー

/// <summary>
/// 頂点シェーダー
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
/// ピクセルシェーダー
/// <summary>
SPSOut PSMain(SPSIn psIn)
{
    SPSOut psOut;
    
    // カメラ空間での深度値を設定
    psOut.depth = psIn.depthInView;
    
    return psOut;
}