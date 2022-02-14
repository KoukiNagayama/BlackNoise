/*!
 * @brief	シンプルなモデルシェーダー。
 */


////////////////////////////////////////////////
// 定数バッファ。
////////////////////////////////////////////////
//モデル用の定数バッファ
cbuffer ModelCb : register(b0){
	float4x4 mWorld;
	float4x4 mView;
	float4x4 mProj;
};

//各種ライト用の定数バッファ
cbuffer LightCb : register(b1) {
    // ディレクションライト
	float3 dirDirection;    // ディレクションライトの方向
	float3 dirColor;        // ディレクションライトのカラー
    // ポイントライト
    float3 ptPosition;      // ポイントライトの座標
    float3 ptColor;         // ポイントライトのカラー
    float ptRange;          // ポイントライトの影響範囲
    // スポットライト
    float3 spPosition;      // スポットライトの座標
    float3 spColor;         // スポットライトのカラー
    float spRange;          // スポットライトの射出範囲
    float3 spDirection;     // スポットライトの射出方向
    float spAngle;          // スポットライトの射出角度
    float3 eyePos;          // 視点の位置
    float3 ambientLight;    // アンビエントライト
    // リムライト
    float3 limDirection;    // リムライトの射出方向
    float3 limColor;        // リムライトのカラー
}

////////////////////////////////////////////////
// 構造体
////////////////////////////////////////////////
//スキニング用の頂点データをひとまとめ。
struct SSkinVSIn{
	int4  Indices  	: BLENDINDICES0;
    float4 Weights  : BLENDWEIGHT0;
};
//頂点シェーダーへの入力。
struct SVSIn{
	float4 pos 		: POSITION;		//モデルの頂点座標。
	float2 uv 		: TEXCOORD0;	//UV座標。
	SSkinVSIn skinVert;				//スキン用のデータ。
	float3 normal	: NORMAL;
};
//ピクセルシェーダーへの入力。
struct SPSIn{
	float4 pos 			: SV_POSITION;	//スクリーン空間でのピクセルの座標。
	float2 uv 			: TEXCOORD0;	//uv座標。
	float3 normal		: NORMAL;
    float3 worldPos		: TEXCOORD1;
    float3 normalInView : TEXCOORD2;
    float4 posInLVP     : TEXCOORD3;    //ライトビュースクリーン空間でのピクセルの座標

};

////////////////////////////////////////////////
// グローバル変数。
////////////////////////////////////////////////
Texture2D<float4> g_albedo : register(t0);				//アルベドマップ
Texture2D<float4> g_specularMap : register(t2);         //スペキュラマップ
StructuredBuffer<float4x4> g_boneMatrix : register(t3);	//ボーン行列。
Texture2D<float4> g_shadowMap : register(t10);          // シャドウマップ
sampler g_sampler : register(s0);	//サンプラステート。

////////////////////////////////////////////////
// 関数定義。
////////////////////////////////////////////////
// Lambert拡散反射光の計算
float3 CalcLambertDiffuse(float3 lightDirection, float3 lightColor, float3 normal);
// Phong鏡面反射光の計算
float3 CalcPhongSpecular(float3 lightDirection, float3 lightColor, float3 worldPos, float3 normal, float2 uv);
// ポイントライトの計算
float3 CalcLigFromPointLight(SPSIn psIn);
// ディレクションライトの計算
float3 CalcLigFromDirectionLight(SPSIn psIn);
// スポットライトの計算
float3 CalcLigFromSpotLight(SPSIn psIn);
// リムライトの計算
float3 CalcLigFromLimLight(SPSIn psIn);

/// <summary>
//スキン行列を計算する。
/// </summary>
float4x4 CalcSkinMatrix(SSkinVSIn skinVert)
{
	float4x4 skinning = 0;	
	float w = 0.0f;
	[unroll]
    for (int i = 0; i < 3; i++)
    {
        skinning += g_boneMatrix[skinVert.Indices[i]] * skinVert.Weights[i];
        w += skinVert.Weights[i];
    }
    
    skinning += g_boneMatrix[skinVert.Indices[3]] * (1.0f - w);
	
    return skinning;
}

/// <summary>
/// 頂点シェーダーのコア関数。
/// </summary>
SPSIn VSMainCore(SVSIn vsIn, uniform bool hasSkin)
{
	SPSIn psIn;
	float4x4 m;
	if( hasSkin ){
		m = CalcSkinMatrix(vsIn.skinVert);
	}else{
		m = mWorld;
	}
	psIn.pos = mul(m, vsIn.pos);
    psIn.worldPos = psIn.pos;
	psIn.pos = mul(mView, psIn.pos);
	psIn.pos = mul(mProj, psIn.pos);

	psIn.normal = mul(m, vsIn.normal);
	psIn.uv = vsIn.uv;
    
    psIn.normalInView = mul(mView, psIn.normal);
    //psIn.posInLVP = mul(mLVP, psIn.worldPos);
	return psIn;
}

/// <summary>
/// スキンなしメッシュ用の頂点シェーダーのエントリー関数。
/// </summary>
SPSIn VSMain(SVSIn vsIn)
{
	return VSMainCore(vsIn, false);
}
/// <summary>
/// スキンありメッシュの頂点シェーダーのエントリー関数。
/// </summary>
SPSIn VSSkinMain( SVSIn vsIn ) 
{
	return VSMainCore(vsIn, true);
}
/// <summary>
/// ピクセルシェーダーのエントリー関数。
/// </summary>
float4 PSMain(SPSIn psIn) : SV_Target0
{
    // ディレクションライト
    float3 directionLig = CalcLigFromDirectionLight(psIn);
    // ポイントライト
    float3 pointLig = CalcLigFromPointLight(psIn);
    // スポットライト
    float3 spotLig = CalcLigFromSpotLight(psIn);
    // リムライト
    float3 limLig = CalcLigFromLimLight(psIn);
    
    // ライトの合算により最終的な光を求める
    float3 lig = directionLig + ambientLight;
    
    
  /*  float2 shadowMapUV = psIn.posInLVP.xy / psIn.posInLVP.w;
    shadowMapUV *= float2(0.5f, -0.5f);
    shadowMapUV += 0.5f;
    
    float3 shadowMap = 1.0f;
    if (shadowMapUV.x > 0.0f && shadowMapUV.x < 1.0f
	&& shadowMapUV.y > 0.0f && shadowMapUV.y < 1.0f
)
    {
        shadowMap = g_shadowMap.Sample(g_sampler, shadowMapUV);
    }*/
	
    float4 albedoColor = g_albedo.Sample(g_sampler, psIn.uv);
    albedoColor.xyz *= lig;
    //albedoColor.xyz *= lig * shadowMap;
	return albedoColor;
}

///<summary>
/// Lambert拡散反射光を計算する
///</summary>
float3 CalcLambertDiffuse(float3 lightDirection, float3 lightColor, float3 normal)
{
    // ピクセルの法線とライトの方向の内積を計算する
    float t = dot(normal, lightDirection) * -1.0f;

    // 内積の値を0以上の値にする
    t = max(0.0f, t);

    // 拡散反射光を計算する
    return lightColor * t;
}

/// <summary>
/// Phong鏡面反射光を計算する
/// </summary>
float3 CalcPhongSpecular(float3 lightDirection, float3 lightColor, float3 worldPos, float3 normal, float2 uv)
{
    // 反射ベクトルを求める
    float3 refVec = reflect(lightDirection, normal);

    // 光が当たったサーフェイスから視点に伸びるベクトルを求める
    float3 toEye = eyePos - worldPos;
    toEye = normalize(toEye);

    // 鏡面反射の強さを求める
    float t = dot(refVec, toEye);

    // 鏡面反射の強さを0以上の数値にする
    t = max(0.0f, t);

    // 鏡面反射の強さを絞る
    t = pow(t, 5.0f);

    // 鏡面反射光を求める
    float3 specularLig = lightColor * t;
    
    // スペキュラマップからスペキュラ反射の強さをサンプリング
    float specPower = g_specularMap.Sample(g_sampler, uv).r;
    
    // 鏡面反射の強さを鏡面反射光に乗算する
    specularLig *= specPower * 10.0f;
    
    return specularLig;
}

/// <summary>
/// ポイントライトによる反射光を計算
/// </summary>
/// <param name="psIn">ピクセルシェーダーに渡されている引数</param>
float3 CalcLigFromPointLight(SPSIn psIn)
{
    // このサーフェイスに入射しているポイントライトの光の向きを計算する
    float3 ligDir = psIn.worldPos - ptPosition;
    ligDir = normalize(ligDir);

    // 減衰なしのLambert拡散反射光を計算する
    float3 diffPoint = CalcLambertDiffuse(
        ligDir, 
        ptColor, 
        psIn.normal
    );

    // 減衰なしのPhong鏡面反射光を計算する
    float3 specPoint = CalcPhongSpecular(
        ligDir, 
        ptColor, 
        psIn.worldPos, 
        psIn.normal,
        psIn.uv
    );

    // 距離による影響率を計算する
    // ポイントライトとの距離を計算する
    float3 distance = length(psIn.worldPos - ptPosition);

    // 影響率は距離に比例して小さくなっていく
    float affect = 1.0f - 1.0f / ptRange * distance;

    // 影響力がマイナスにならないように補正をかける
    if (affect < 0.0f)
    {
        affect = 0.0f;
    }

    // 影響の仕方を指数関数的にする。今回のサンプルでは3乗している
    affect = pow(affect, 3.0f);

    // 拡散反射光と鏡面反射光に減衰率を乗算して影響を弱める
    diffPoint *= affect;
    specPoint *= affect;

    return diffPoint + specPoint;
}

/// <summary>
/// ディレクションライトによる反射光を計算
/// </summary
/// <param name="psIn">ピクセルシェーダーからの入力。</param>
float3 CalcLigFromDirectionLight(SPSIn psIn)
{
    // ディレクションライトによるLambert拡散反射光を計算する
    float3 diffDirection = CalcLambertDiffuse(dirDirection, dirColor, psIn.normal);

    // ディレクションライトによるPhong鏡面反射光を計算する
    float3 specDirection = CalcPhongSpecular(
            dirDirection, dirColor, psIn.worldPos, psIn.normal, psIn.uv);
    return diffDirection + specDirection;
}

/// <summary>
/// スポットライトによる反射光を計算
/// </summary
/// <param name="psIn">ピクセルシェーダーからの入力。</param>
float3 CalcLigFromSpotLight(SPSIn psIn)
{
    float3 ligDir = psIn.worldPos - spPosition;
    ligDir = normalize(ligDir);
    // 減衰なしのLambert拡散反射光を計算する
    float3 diffSpot = CalcLambertDiffuse(
        ligDir, 
        spColor, 
        psIn.normal		
    );
    // 減衰なしのPhong鏡面反射光を計算する
    float3 specSpot = CalcPhongSpecular(
        ligDir, 
        spColor, 
        psIn.worldPos, 
        psIn.normal,
        psIn.uv
    );
    // スポットライトとの距離を計算する。
    float3 distance = length(psIn.worldPos - spPosition);

    // 影響率は距離に比例して小さくなっていく。
    float affect = 1.0f - 1.0f / spRange * distance;
    // 影響力がマイナスにならないように補正をかける。
    if (affect < 0.0f)
    {
        affect = 0.0f;
    }
    // 影響の仕方を指数関数的にする。
    affect = pow(affect, 3.0f);
    // 影響率を乗算して影響を弱める
    diffSpot *= affect;
    specSpot *= affect;
    // dot()を利用して内積を求める。
    float angle = dot(ligDir, spDirection);
    // dot()で求めた値をacos()に渡して角度を求める。
    angle = acos(angle);
    // 角度に比例して小さくなっていく影響率を計算する。
    affect = 1.0f - 1.0f / spAngle * angle;
    // 影響力がマイナスにならないように補正をかける。
    if (affect < 0.0f){
        affect = 0.0f;
    }
    // 影響の仕方を指数関数的にする。
    affect = pow(affect, 0.5f);
    // 角度による影響率を反射光に乗算して、影響を弱める
    diffSpot *= affect;
    specSpot *= affect;
   
    return diffSpot + specSpot;
}

/// <summary>
/// リムライトによる反射光を計算(ディレクションライト)
/// </summary
/// <param name="psIn">ピクセルシェーダーからの入力。</param>
float3 CalcLigFromLimLight(SPSIn psIn)
{
    // ディレクションライトによるLambert拡散反射光を計算する
    float3 diffLim = CalcLambertDiffuse(
        limDirection, 
        limColor, 
        psIn.normal
    );

    // ディレクションライトによるPhong鏡面反射光を計算する
    float3 specLim = CalcPhongSpecular(
        limDirection, 
        limColor, 
        psIn.worldPos, 
        psIn.normal,
        psIn.uv
    );
    
    float3 limLig = diffLim + specLim;
    
    // リムライトの強さ
    // サーフェイスの法線と光の入射方向に依存するリムの強さの計算
    float power1 = 1.0f - max(0.0f, dot(limDirection, psIn.normal));
    // サーフェイスの法線と視線の方向に依存するリムの強さの計算
    float power2 = 1.0f - max(0.0f, psIn.normalInView.z * -1.0f);
    // 最終的なリムの強さを求める
    float limPower = power1 * power2;
    // 強さの変化を指数関数的にする
    limPower = pow(limPower, 1.3f);
    // リムライトのカラー
    float3 LimColor = limColor * limPower;
    
    return limLig + LimColor;
    
}