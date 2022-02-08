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
		/// <param name="filePath">ファイルパス</param>
		void Init(const char* filePath);
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
		/// <summary>
		/// 描画
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト</param>
		void Render(RenderContext& rc);
	private:
		RenderTarget		m_shadowMap;
		Camera				m_lightCamera;
		Model				m_shadowModel;

	};
	extern Shadow g_shadow;
}

