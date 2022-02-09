#pragma once
namespace nsK2EngineLow {
	class Bloom
	{
	public:
		Bloom() {};
		~Bloom() {};
		/// <summary>
		/// ブルーム実行に必要な全て要素の初期化
		/// </summary>
		/// <param name="mainRenderTarget">メインレンダリングターゲット</param>
		void InitBloom(RenderTarget& mainRenderTarget);
		/// <summary>
		/// 輝度抽出用のレンダリングターゲットの初期化
		/// </summary>
		/// <param name="mainRenderTarget">メインレンダーターゲット</param>
		void InitLuminanceRenderTarget(RenderTarget& mainRenderTarget);
		/// <summary>
		/// 輝度抽出用のスプライトの初期化
		/// </summary>
		/// <param name="mainRenderTarget">メインレンダリングターゲット</param>
		void InitSprite(RenderTarget& mainRenderTarget);
		/// <summary>
		/// ガウシアンブラーを初期化
		/// </summary>
		void InitBlur();
		/// <summary>
		/// ボケ画像を加算合成するスプライトを初期化
		/// </summary>
		/// <param name="mainRenderTarget">メインレンダリングターゲット</param>
		void InitFinalSprite(RenderTarget& mainRenderTarget);
		/// <summary>
		/// レンダリングターゲットをmainRenderTargetに変更
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト</param>
		/// <param name="mainRenderTarget">メインレンダリングターゲット</param>
		void ChangeRenderTarget(RenderContext& rc, RenderTarget& mainRenderTarget);
		/// <summary>
		/// 輝度抽出
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト</param>
		/// <param name="mainRenderTarget">メインレンダリングターゲット</param>
		void LuminanceExtraction(RenderContext& rc, RenderTarget& mainRenderTarget);
		/// <summary>
		/// ブラーを実行
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト</param>
		void RunBlur(RenderContext& rc);
		/// <summary>
		/// ボケ画像をメインレンダリングターゲットに加算合成
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト</param>
		/// <param name="mainRenderTarget">メインレンダリングターゲット</param>
		void CompositeImageToMainRenderTarget(RenderContext& rc, RenderTarget& mainRenderTarget);
		/// <summary>
		/// 描画
		/// </summary>
		/// <param name="rc">レンダリングコンテキスト</param>
		/// <param name="mainRenderTarget">メインレンダリングターゲット</param>
		void Render(RenderContext& rc, RenderTarget& mainRenderTarget);


	private:
		RenderTarget		m_luminanceRenderTarget;
		GaussianBlur		m_gaussianBlur[4];
		Sprite				m_luminanceSprite;
		Sprite				m_finalSprite;
		Sprite				m_copyToFrameBufferSprite;

	};

	extern Bloom g_bloom;
}
 
