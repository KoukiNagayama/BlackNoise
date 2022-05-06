#pragma once
namespace nsK2EngineLow {
	class MainRenderTarget
	{
	public:
		MainRenderTarget() {};
		~MainRenderTarget() {};

		/// <summary>
		/// 初期化
		/// </summary>
		void Init();
		/// <summary>
		/// メインレンダリングターゲットをフレームバッファにコピー
		/// </summary>
		/// <param name="rc"></param>
		void CopyMainRenderTargetToFrameBuffer(RenderContext& rc);
		/// <summary>
		/// メインレンダリングターゲットを取得
		/// </summary>
		/// <returns>メインレンダリングターゲット</returns>
		RenderTarget& GetMainRenderTarget() 
		{
			return m_mainRenderTarget;
		}
	private:
		/// <summary>
		/// メインレンダリングターゲットを初期化
		/// </summary>
		void InitMainRenderTarget();
		/// <summary>
		/// メインレンダリングターゲットをフレームバッファにコピーする用のスプライトを初期化
		/// </summary>
		void InitCopyMainRtToFrameBufferSprite();
	private:
		RenderTarget		m_mainRenderTarget;					// メインレンダリングターゲット		
		Sprite				m_copyMainRtToFrameBufferSprite;	// メインレンダリングターゲットをフレームバッファにコピーする用のスプライト

	};
	extern MainRenderTarget g_mainRenderTarget;
}


