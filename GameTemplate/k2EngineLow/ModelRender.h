#pragma once
namespace nsK2EngineLow {
	class Shadow;
	class ModelRender
	{
	public:
		ModelRender();
		~ModelRender();
		/// <summary>
		/// 初期化処理
		/// </summary>
		/// <param name="filePath">ファイルパス</param>
		/// <param name="animationClips">アニメーションクリップ</param>
		/// <param name="numAnimationClips">アニメーションクリップの数</param>
		/// <param name="isShadowReceiver">影を受けるかどうか</param>
		/// <param name="enModelUpAxis">モデルの上方向</param>
		void Init(const char* filePath,
			AnimationClip* animationClips = nullptr,
			int numAnimationClips = 0,
			bool isShadowReceiver = true,
			EnModelUpAxis enModelUpAxis = enModelUpAxisZ
		);
		/// <summary>
		/// 更新処理
		/// </summary>
		void Update();
		/// <summary>
		/// 描画処理
		/// </summary>
		/// <param name="rc"></param>
		/// <param name="filePath"></param>
		void Draw(RenderContext& rc);
		/// <summary>
		/// アニメーション再生。
		/// </summary>
		/// <param name="animNo">アニメーションクリップの番号。</param>
		/// <param name="interpolateTime">補完時間(単位：秒。)</param>
		void PlayAnimation(int animNo, float interpolateTime = 0.0f)
		{
			m_animation.Play(animNo, interpolateTime);
		}
		/// <summary>
		/// 座標を設定
		/// </summary>
		/// <param name="pos">座標</param>
		void SetPosition(const Vector3& pos)
		{
			m_position = pos;
		}
		/// <summary>
		/// 回転を設定
		/// </summary>
		/// <param name="rotation">回転</param>
		void SetRotation(const Quaternion& rotation)
		{
			m_rotation = rotation;
		}
		/// <summary>
		/// 拡大率を設定
		/// </summary>
		/// <param name="scale">拡大率</param>
		void SetScale(const Vector3& scale)
		{
			m_scale = scale;
		}

	private:
		/// <summary>
		/// モデルの初期化
		/// </summary>
		/// <param name="filePath">ファイルパス</param>
		/// <param name="enModelUpAxis">モデルの上方向</param>
		void InitModel(const char* filePath, 
			EnModelUpAxis enModelUpAxis = enModelUpAxisZ
		);
		/// <summary>
		/// スケルトンの初期化
		/// </summary>
		/// <param name="filePath">ファイルパス</param>
		void InitSkeleton(const char* filePath);
		/// <summary>
		/// アニメーションの初期化
		/// </summary>
		/// <param name="animationClips">アニメーションクリップ。</param>
		/// <param name="numAnimationClips">アニメーションクリップの数。</param>
		void InitAnimation(AnimationClip* animationClips,
			int numAnimationClips);
		/// <summary>
		/// シャドウマップ描画用のモデルを初期化
		/// </summary>
		/// <param name="filePath">ファイルパス</param>
		void InitModelOnShadowMap(const char* filePath);


		Model				m_model;							// モデル
		Skeleton			m_skeleton;							// スケルトン
		//ModelInitData		m_initData;							// モデルを初期化するための情報を設定。
		Animation			m_animation;						// アニメーション
		AnimationClip*		m_animationClips;					// アニメーションクリップ
		int					m_numAnimationClips;				// アニメーションクリップの数
		Vector3				m_position = Vector3::Zero;			// 座標
		Vector3				m_scale = Vector3::One;				// 拡大率
		Quaternion			m_rotation = Quaternion::Identity;	// 回転
		std::vector<Model>  m_shadowModelArray;				// 影モデル

	};
}


