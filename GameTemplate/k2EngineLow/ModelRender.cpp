#include "k2EngineLowPreCompile.h"
#include "ModelRender.h"
#include "DirectionLight.h"
#include "PointLight.h"
#include "Light.h"
#include "Shadow.h"

namespace nsK2EngineLow {
	ModelRender::ModelRender()
	{

	}

	ModelRender::~ModelRender()
	{

	}

	void ModelRender::Init(const char* filePath,
		AnimationClip* animationClips,
		int numAnimationClips,
		bool isShadowReceiver,
		EnModelUpAxis enModelUpAxis,
		bool isShadowCaster
	)
	{
		// アニメーションを代入(アニメーションの有無判定のため)
		m_animationClips = animationClips;
		// スケルトンの初期化
		InitSkeleton(filePath);
		// アニメーションの初期化
		InitAnimation(animationClips, numAnimationClips);
		if (isShadowCaster == true) {
			// シャドウマップ描画用モデルの初期化
			InitModelOnShadowMap(filePath);
		}
		// モデルの初期化
		InitModel(filePath, enModelUpAxis, isShadowReceiver);

		m_isShadowCaster = isShadowCaster;
	}

	void ModelRender::InitSkeleton(const char* filePath)
	{
		std::string skeletonFilePath = filePath;
		int pos = (int)skeletonFilePath.find(".tkm");
		skeletonFilePath.replace(pos, 4, ".tks");
		m_skeleton.Init(skeletonFilePath.c_str());
	}

	void ModelRender::InitAnimation(AnimationClip* animationClips, int numAnimationClips)
	{
		m_numAnimationClips = numAnimationClips;
		m_animationClips = animationClips;
		
		if (m_animationClips != nullptr) {
			// アニメーションを初期化
			m_animation.Init(
				m_skeleton,					// アニメーションを流し込むスケルトン
				m_animationClips,			// アニメーションクリップ
				m_numAnimationClips			// アニメーションの数
			);
		}
	}

	/*void ModelRender::InitModel(const char* filePath,
		EnModelUpAxis enModelUpAxis,
		bool isShadowReceiver
	)
	{
		ModelInitData modelInitData;
		// tkmファイルのファイルパスを指定する。
		modelInitData.m_tkmFilePath = filePath;

		// シェーダーファイルのファイルパスを指定する。
		if (isShadowReceiver == false) {
			modelInitData.m_fxFilePath = "Assets/shader/model.fx";
			if (m_animationClips != nullptr) {
				//スケルトンを指定する。
				modelInitData.m_skeleton = &m_skeleton;
				//スキンメッシュ用の頂点シェーダーのエントリーポイントを指定。
				modelInitData.m_vsSkinEntryPointFunc = "VSSkinMain";
				//モデルの上方向を指定する。
				modelInitData.m_modelUpAxis = enModelUpAxis;
			}
			else {
				//ノンスキンメッシュ用の頂点シェーダーのエントリーポイントを指定する。
				modelInitData.m_vsEntryPointFunc = "VSMain";
			}
			//ライトの情報を呈すバッファとしてディスクリプタヒープに登録するために
			//モデルの初期化情報として渡す。
			modelInitData.m_expandConstantBuffer = g_light.GetLightData();
			modelInitData.m_expandConstantBufferSize = sizeof(*g_light.GetLightData());
			//作成した初期化データをもとにモデルを初期化する。
			m_model.Init(modelInitData);
		}
		else {
			modelInitData.m_fxFilePath = "Assets/shader/shadowReciever.fx";
			modelInitData.m_modelUpAxis = enModelUpAxis;
			// シャドウマップを拡張SRVに設定する。
			modelInitData.m_expandShaderResoruceView[0] = &g_shadow.GetShadowMap().GetRenderTargetTexture();
			// ライトビュープロジェクション行列を拡張定数バッファに設定する
			modelInitData.m_expandConstantBuffer = (void*)&g_shadow.GetLightCamera().GetViewProjectionMatrix();
			modelInitData.m_expandConstantBufferSize = sizeof(g_shadow.GetLightCamera().GetViewProjectionMatrix());
			m_model.Init(modelInitData);
		}

		
		
	}*/

	void ModelRender::InitModel(const char* filePath,
		EnModelUpAxis enModelUpAxis,
		bool isShadowReceiver
	)
	{
		ModelInitData modelInitData;
		// tkmファイルのファイルパスを指定する。
		modelInitData.m_tkmFilePath = filePath;
		modelInitData.m_fxFilePath = "Assets/shader/model.fx";
		if (m_animationClips != nullptr) {
			//スケルトンを指定する。
			modelInitData.m_skeleton = &m_skeleton;
			//スキンメッシュ用の頂点シェーダーのエントリーポイントを指定。
			modelInitData.m_vsSkinEntryPointFunc = "VSSkinMain";
		}
		else {
			//ノンスキンメッシュ用の頂点シェーダーのエントリーポイントを指定する。
			modelInitData.m_vsEntryPointFunc = "VSMain";
		}
		//モデルの上方向を指定する。
		modelInitData.m_modelUpAxis = enModelUpAxis;
		//ライトの情報を呈すバッファとしてディスクリプタヒープに登録するために
		//モデルの初期化情報として渡す。
		modelInitData.m_expandConstantBuffer = g_light.GetLightData();
		modelInitData.m_expandConstantBufferSize = sizeof(*g_light.GetLightData());
		//作成した初期化データをもとにモデルを初期化する。
		m_model.Init(modelInitData);
	}
	
	void ModelRender::InitModelOnShadowMap(const char* filePath,
		EnModelUpAxis enModelUpAxis
	)
	{
		ModelInitData shadowModelInitData;
		//モデルの上方向を指定する。
		shadowModelInitData.m_modelUpAxis = enModelUpAxis;
		// tkmファイルのファイルパスを指定する。
		shadowModelInitData.m_tkmFilePath = filePath;
		// シェーダーファイルのファイルパスを指定する。
		shadowModelInitData.m_fxFilePath = "Assets/shader/shadowMap.fx";
		// 初期化データをもとにモデルを初期化
		m_shadowMapModel.Init(shadowModelInitData);

	}

	
	void ModelRender::Update()
	{
		if (m_skeleton.IsInited()) {
			// スケルトンを更新
			m_skeleton.Update(m_model.GetWorldMatrix());
		}
		// アニメーションを進める
		m_animation.Progress(g_gameTime->GetFrameDeltaTime());
		// 通常レンダリング用モデルのワールド行列を更新
		m_model.UpdateWorldMatrix(
			m_position, 
			g_quatIdentity,
			g_vec3One
		);
		// シャドウマップ描画用モデルのワールド行列を更新
		m_shadowMapModel.UpdateWorldMatrix(
			m_position,
			g_quatIdentity,
			g_vec3One
		);

		if (m_isShadowCaster == true) {
			// モデルの情報の受け渡し
			g_shadow.SetShadowModel(&m_shadowMapModel);
		}

	}

	void ModelRender::Draw(RenderContext& rc)
	{
		m_model.Draw(rc);
	}
}

