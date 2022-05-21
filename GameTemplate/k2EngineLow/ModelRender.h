#pragma once
namespace nsK2EngineLow {
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
		/// <param name="isShadowCaster">影をキャストするか</param>
		/// <param name="outlineType">輪郭線の種類 0:通常 1:アイテム 2:エネミー</param>
		void Init(const char* filePath,
			AnimationClip* animationClips = nullptr,
			int numAnimationClips = 0,
			bool isShadowReceiver = true,
			EnModelUpAxis enModelUpAxis = enModelUpAxisZ,
			bool isShadowCaster = true,
			int maxInstance = 0,
			int outlineType = 0
		);
		/// <summary>
		/// 更新処理
		/// </summary>
		void Update();
		/// <summary>
		/// インスタンシングデータの更新。
		/// </summary>
		/// <param name="pos">座標</param>
		/// <param name="rot">回転</param>
		/// <param name="scale">拡大率</param>
		void UpdateInstancingData(const Vector3& pos, const Quaternion& rot, const Vector3& scale);
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
		/// モデルを取得
		/// </summary>
		/// <returns>モデル</returns>
		Model& GetModel()
		{
			return m_model;
		}
		/// <summary>
		/// アニメーションの再生中？
		/// </summary>
		bool IsPlayingAnimation() const
		{
			return m_animation.IsPlaying();
		}
		/// <summary>
		/// 座標、回転、拡大を全て設定。
		/// </summary>
		/// <remark>
		/// インスタンシング描画が有効の場合は、この設定は無視されます。
		/// </remark>
		/// <param name="pos">座標。</param>
		/// <param name="rotation">回転。</param>
		/// <param name="scale">拡大。</param>
		void SetTRS(const Vector3& pos, const Quaternion& rotation, const Vector3& scale)
		{
			SetPosition(pos);
			SetRotation(rotation);
			SetScale(scale);
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
		/// <summary>
		/// アニメーションイベントの追加
		/// </summary>
		/// <param name="eventListener"></param>
		void AddAnimationEvent(AnimationEventListener eventListener)
		{
			m_animation.AddAnimationEventListener(eventListener);
		}
		/// <summary>
		/// アニメーション再生の速度を設定する。
		/// </summary>
		/// <param name="animationSpeed">数値の分だけ倍にする。</param>
		void SetAnimationSpeed(const float animationSpeed)
		{
			m_animationSpeed = animationSpeed;
		}
	private:
		/// <summary>
		/// モデルの初期化
		/// </summary>
		/// <param name="filePath">ファイルパス</param>
		/// <param name="enModelUpAxis">モデルの上方向</param>
		/// <param name="isShadowReceiver">影を受けるか</param>
		void InitModel(const char* filePath, 
			EnModelUpAxis enModelUpAxis = enModelUpAxisZ,
			bool isShadowReceiver = true
		);
		/// <summary>
		/// 輪郭線描画モデルの初期化
		/// </summary>
		/// <param name="filePath">ファイルパス</param>
		/// <param name="enModelUpAxis">モデルの上方向</param>
		void InitModelWithContours(const char* filePath,
			EnModelUpAxis enModelUpAxis = enModelUpAxisZ
		);
		/// <summary>
		/// フォワードレンダリング用のモデルの初期化
		/// </summary>
		/// <param name="filePath"></param>
		/// <param name="enModelUpAxis"></param>
		void InitForwardRenderingModel(const char* filePath,
			int outlineType,
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
		/// <param name="enModelUpAxis">モデルの上方向</param>
		void InitModelOnShadowMap(const char* filePath,
			EnModelUpAxis enModelUpAxis = enModelUpAxisZ
		);
		/// <summary>
		/// 深度値マップ描画用のモデルを初期化
		/// </summary>
		/// <param name="filePath">ファイルパス</param>
		/// <param name="enModelUpAxis">モデルの上方向</param>
		void InitModelOnDepthValueMap(const char* filePath,
			EnModelUpAxis enModelUpAxis = enModelUpAxisZ
		);


	private:
		Model						m_model;							// モデル
		Skeleton					m_skeleton;							// スケルトン
		Animation					m_animation;						// アニメーション
		AnimationClip*				m_animationClips;					// アニメーションクリップ
		int							m_numAnimationClips;				// アニメーションクリップの数
		Vector3						m_position = Vector3::Zero;			// 座標
		Vector3						m_scale = Vector3::One;				// 拡大率
		Quaternion					m_rotation = Quaternion::Identity;	// 回転
		std::vector<Model>			m_shadowModelArray;					// 影モデル配列
		Model						m_shadowMapModel;					// シャドウマップ描画用モデル
		Model						m_shadowModel;						// 影モデル
		bool						m_isShadowCaster;					// 影をキャストするか
		std::vector<Model>			m_modelArray;						// モデル配列
		Model						m_depthValueMapModel;				// 深度値マップ描画用モデル
		int							m_numInstance = 0;					// インスタンスの数。
		int							m_maxInstance = 1;					// 最大インスタンス数。
		int							m_fixNumInstanceOnFrame = 0;		// このフレームに描画するインスタンスの数の確定数。。
		bool						m_isEnableInstancingDraw = false;	// インスタンシング描画が有効？
		std::unique_ptr<Matrix[]>	m_worldMatrixArray;					// ワールド行列の配列。
		StructuredBuffer			m_worldMatrixArraySB;				// ワールド行列の配列のストラクチャードバッファ。
		int							m_outlineType;						// 輪郭線の種類
		float						m_animationSpeed = 1.0f;			// アニメーションの速さの倍率
	};
}


