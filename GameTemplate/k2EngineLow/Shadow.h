#pragma once
namespace nsK2EngineLow {
	class Shadow
	{
	public:
		Shadow();
		~Shadow() {};
		/// <summary>
		/// 初期化
		/// </summary>
		/// <param name="filePath">ファイルパス</param>
		void Init(const char* filePath);
		/// <summary>
		/// 描画
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト</param>
		void Render(RenderContext& rc);
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
		void SetShadowModel(Model* model)
		{
			m_shadowModelArray.push_back(model); 
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
		/// <summary>
		/// シャドウマップ描画用のモデルの初期化
		/// </summary>
		/// <param name="filePath">ファイルパス</param>
		void InitModelForShadowMap(const char* filePath);

	private:
		RenderTarget		m_shadowMap;			// シャドウマップ描画用のレンダリングターゲット
		Camera				m_lightCamera;			// ライトカメラ
		std::vector<Model*>  m_shadowModelArray;		// 影モデル
		//Sprite				m_copyToFrameBufferSprite;           
	};
	extern Shadow g_shadow;
}

