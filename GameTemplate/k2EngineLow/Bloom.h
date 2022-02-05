#pragma once
namespace nsK2EngineLow {
	class Bloom
	{
	public:
		Bloom() {};
		~Bloom() {};
		void InitLuminanceRenderTarget(RenderTarget& mainRenderTarget);
		void InitSprite(RenderTarget& mainRenderTarget);
		void InitBlur();
		void InitFinalSprite(RenderTarget& mainRenderTarget);
		void Render1(RenderContext& rc, RenderTarget& mainRenderTarget);
		void Render2(RenderContext& rc, RenderTarget& mainRenderTarget);

	private:
		RenderTarget m_luminanceRenderTarget;
		GaussianBlur m_gaussianBlur[4];
		Sprite m_luminanceSprite;
		Sprite m_finalSprite;
		Sprite m_copyToFrameBufferSprite;

	};

	extern Bloom g_bloom;
}
 
