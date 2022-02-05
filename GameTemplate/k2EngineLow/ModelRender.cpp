#include "k2EngineLowPreCompile.h"
#include "ModelRender.h"
#include "DirectionLight.h"
#include "PointLight.h"
#include "Light.h"

namespace nsK2EngineLow {
	ModelRender::ModelRender()
	{

	}

	ModelRender::~ModelRender()
	{

	}

	void ModelRender::Init(const char* filePath,
		AnimationClip* animationClips,
		int numAnimationClips)
	{
		// アニメーションを代入(アニメーションの有無判定のため)
		m_animationClips = animationClips;
		// スケルトンの初期化
		InitSkeleton(filePath);
		// モデルの初期化
		InitModel(filePath);
		// アニメーションの初期化
		InitAnimation(animationClips, numAnimationClips);


	}

	void ModelRender::InitModel(const char* filePath)
	{
		// tkmファイルのファイルパスを指定する。
		m_initData.m_tkmFilePath = filePath;
		// シェーダーファイルのファイルパスを指定する。
		m_initData.m_fxFilePath = "Assets/shader/model.fx";

		if (m_animationClips != nullptr) {
			//スキンメッシュ用の頂点シェーダーのエントリーポイントを指定。
			m_initData.m_vsSkinEntryPointFunc = "VSSkinMain";
			//スケルトンを指定する。
			m_initData.m_skeleton = &m_skeleton;
		}
		else {
			//ノンスキンメッシュ用の頂点シェーダーのエントリーポイントを指定する。
			m_initData.m_vsEntryPointFunc = "VSMain";
		}

		//モデルの上方向を指定する。
		m_initData.m_modelUpAxis = enModelUpAxisY;
		//ライトの情報を呈すバッファとしてディスクリプタヒープに登録するために
		//モデルの初期化情報として渡す。
		m_initData.m_expandConstantBuffer = g_light.GetLightData();
		m_initData.m_expandConstantBufferSize = sizeof(*g_light.GetLightData());
	

		//作成した初期化データをもとにモデルを初期化する。
		m_model.Init(m_initData);
	}

	void ModelRender::InitSkeleton(const char* filePath)
	{
		m_skeleton.Init("Assets/modelData/unityChan.tks");
	}

	void ModelRender::InitAnimation(AnimationClip* animationClips, int numAnimationClips)
	{
		m_numAnimationClips = numAnimationClips;
		
		// アニメーションを初期化
		m_animation.Init(
			m_skeleton,					// アニメーションを流し込むスケルトン
			m_animationClips,			// アニメーションクリップ
			m_numAnimationClips			// アニメーションの数
		);
	}
	void ModelRender::Update()
	{
		// スケルトンを更新
		m_skeleton.Update(m_model.GetWorldMatrix());
		// アニメーションを進める
		m_animation.Progress(g_gameTime->GetFrameDeltaTime());
		// ワールド行列を更新
		m_model.UpdateWorldMatrix(m_position, m_rotation, m_scale);
	}

	void ModelRender::Draw(RenderContext& rc)
	{
		m_model.Draw(rc);
	}
}

