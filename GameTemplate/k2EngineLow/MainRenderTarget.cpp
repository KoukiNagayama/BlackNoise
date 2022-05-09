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
		// �X�v���C�g�̏������I�u�W�F�N�g
		SpriteInitData spriteInitData;
		// �e�N�X�`��
		spriteInitData.m_textures[0] = &m_mainRenderTarget.GetRenderTargetTexture();
		// �𑜓x
		spriteInitData.m_width = g_graphicsEngine->GetFrameBufferWidth();
		spriteInitData.m_height = g_graphicsEngine->GetFrameBufferHeight();
		// �V�F�[�_�[�̃t�@�C���p�X���w��
		spriteInitData.m_fxFilePath = "Assets/shader/2D.fx";

		// �������I�u�W�F�N�g�����ƂɃX�v���C�g��������
		m_copyMainRtToFrameBufferSprite.Init(spriteInitData);
	}

	void MainRenderTarget::CopyMainRenderTargetToFrameBuffer(RenderContext& rc)
	{
		rc.SetRenderTarget(
			g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
			g_graphicsEngine->GetCurrentFrameBuffuerDSV()
		);

		// �r���[�|�[�g���w�肷��
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
