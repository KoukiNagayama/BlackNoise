#include "k2EngineLowPreCompile.h"
#include "MainRenderTarget.h"

namespace nsK2EngineLow {
	void MainRenderTarget::Init()
	{
		InitMainRenderTarget();

		InitCopyMainRtToFrameBufferSprite();
	}

	void MainRenderTarget::InitMainRenderTarget()
	{
		m_mainRenderTarget.Create(
			g_graphicsEngine->GetFrameBufferWidth(),
			g_graphicsEngine->GetFrameBufferHeight(),
			1,
			1,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_D32_FLOAT
		);
	}

	void MainRenderTarget::InitCopyMainRtToFrameBufferSprite()
	{
		// スプライトの初期化オブジェクト
		SpriteInitData spriteInitData;
		// テクスチャ
		spriteInitData.m_textures[0] = &m_mainRenderTarget.GetRenderTargetTexture();
		// 解像度
		spriteInitData.m_width = g_graphicsEngine->GetFrameBufferWidth();
		spriteInitData.m_height = g_graphicsEngine->GetFrameBufferHeight();
		// シェーダーのファイルパスを指定
		spriteInitData.m_fxFilePath = "Assets/shader/2D.fx";

		// 初期化オブジェクトをもとにスプライトを初期化
		m_copyMainRtToFrameBufferSprite.Init(spriteInitData);
	}

	void MainRenderTarget::CopyMainRenderTargetToFrameBuffer(RenderContext& rc)
	{
		rc.SetRenderTarget(
			g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
			g_graphicsEngine->GetCurrentFrameBuffuerDSV()
		);

		// ビューポートを指定する
		D3D12_VIEWPORT viewport;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = static_cast<FLOAT>(g_graphicsEngine->GetFrameBufferWidth());
		viewport.Height = static_cast<FLOAT>(g_graphicsEngine->GetFrameBufferHeight());
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;

		rc.SetViewportAndScissor(viewport);
		m_copyMainRtToFrameBufferSprite.Draw(rc);
	}

	MainRenderTarget g_mainRenderTarget;
}
