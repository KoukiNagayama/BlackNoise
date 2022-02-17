#pragma once
namespace nsK2EngineLow {
	class Shadow
	{
	public:
		Shadow() {};
		~Shadow() {};
		/// <summary>
		/// 初期化
		/// </summary>
		void Init();
		/// <summary>
		/// 描画
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト</param>
		void RenderToShadowMap(RenderContext& rc);
		/// <summary>
		/// ライトカメラを取得
		/// </summary>
		/// <returns>ライトカメラ</returns>
		Camera& GetLightCamera() 
		{
			return m_lightCamera;
		}
		/// <summary>
		/// シャドウマップ描画用のレンダリングターゲットを取得
		/// </summary>
		/// <returns>シャドウマップ描画用のレンダリングターゲット</returns>
		RenderTarget& GetShadowMap()
		{
			return m_shadowMap;
		}
		/// <summary>
		/// シャドウマップ描画用にモデルを設定
		/// </summary>
		/// <param name="model">シャドウマップ描画用モデル</param>
		void SetShadowModel(Model* model)
		{
			//m_shadowModelArray.push_back(model);
			m_shadowModel = model;
		}
	private:
		/// <summary>
		/// シャドウマップ描画用のレンダリングターゲットの初期化
		/// </summary>
		void InitRenderTargetForShadowMap();
		/// <summary>
		/// 影描画用のライトカメラの初期化
		/// </summary>
		void InitLightCamera();


	private:
		RenderTarget		m_shadowMap;			// シャドウマップ描画用のレンダリングターゲット
		Camera				m_lightCamera;			// ライトカメラ
		std::vector<Model*>  m_shadowModelArray;		// 影モデル 
		Model*				m_shadowModel;
	};
	extern Shadow g_shadow;
}

