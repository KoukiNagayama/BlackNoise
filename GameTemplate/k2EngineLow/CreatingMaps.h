#pragma once
namespace nsK2EngineLow {
	/// <summary>
	/// 深度値マップ (＋ワールド座標用マップ)
	/// </summary>
	class CreatingMaps
	{
	public:
		CreatingMaps() {};
		~CreatingMaps() {};
		/// <summary>
		/// 初期化
		/// </summary>
		void Init();
		/// <summary>
		/// 深度値マップ描画用にモデルを設定
		/// </summary>
		void SetModel(Model* model)
		{
			m_modelArray.push_back(model);
		}
		/// <summary>
		/// 更新
		/// </summary>
		void Update();
		/// <summary>
		/// 深度値マップへの描画
		/// </summary>
		void RenderToDepthValueMap(RenderContext& rc);
		/// <summary>
		/// 深度値マップ書き込み用のレンダリングターゲットを取得
		/// </summary>
		/// <returns>深度値マップ用のレンダリングターゲット</returns>
		RenderTarget& GetDepthValueMap()
		{
			return m_depthValueMap;
		}
		/// <summary>
		/// ワールド座標書き込み用のレンダリングターゲットを取得
		/// </summary>
		/// <returns>ワールド座標書き込み用のレンダリングターゲット</returns>
		RenderTarget& GetWorldCoordinateMap()
		{
			return m_worldCoordinateMap;
		}
		/// <summary>
		/// 法線マップ描画用のレンダリングターゲットを取得
		/// </summary>
		/// <returns>法線マップ描画用のレンダリングターゲット</returns>
		RenderTarget& GetNormalMap()
		{
			return m_normalMap;
		}
	private:
		/// <summary>
		/// 深度値マップ描画用のレンダリングターゲットの初期化
		/// </summary>
		void InitRenderTargetForDepthValueMap();
		/// <summary>
		/// ワールド座標記録用のレンダリングターゲットの初期化
		/// </summary>
		void InitRenderTargetForWorldCoordinateMap();
		/// <summary>
		/// 法線マップ描画用のレンダリングターゲットの初期化
		/// </summary>
		void InitRenderTargetForNormalMap();
		/// <summary>
		/// カメラの初期化
		/// </summary>
		void InitCamera();
	private:
		std::vector<Model*> m_modelArray;			// モデル配列
		Camera				m_camera;				// カメラ
		RenderTarget		m_depthValueMap;		// 深度値マップのレンダリングターゲット
		RenderTarget		m_worldCoordinateMap;	// ワールド座標マップ
		RenderTarget		m_normalMap;			// 法線マップ
	};

	extern CreatingMaps g_creatingMaps;
}

