#pragma once
namespace nsK2EngineLow {
	class DepthValueMap
	{
	public:
		DepthValueMap() {};
		~DepthValueMap() {};
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

	private:
		/// <summary>
		/// 深度値マップ描画用のレンダリングターゲットの初期化
		/// </summary>
		void InitRenderTargetForDepthValueMap();
		/// <summary>
		/// カメラの初期化
		/// </summary>
		void InitCamera();
	private:
		std::vector<Model*> m_modelArray;		// モデル配列
		Camera				m_camera;			// カメラ
		RenderTarget		m_depthValueMap;	// 深度値マップのレンダリングターゲット
	};

	extern DepthValueMap g_depthValueMap;
}

