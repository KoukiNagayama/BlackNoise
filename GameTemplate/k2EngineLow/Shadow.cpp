#include "k2EngineLowPreCompile.h"
#include "Shadow.h"

namespace nsK2EngineLow {
	void Shadow::Init()
	{
		InitRenderTargetForShadowMap();

		InitLightCamera();
	}

	void Shadow::InitRenderTargetForShadowMap()
	{
		float clearColor[4] = { 1.0f,1.0f,1.0f,1.0f };
		m_shadowMap.Create(
			1024,
			1024,
			1,
			1,
			DXGI_FORMAT_R32_FLOAT,
			DXGI_FORMAT_D32_FLOAT,
			clearColor
		);
	}

	void Shadow::InitLightCamera()
	{
		// �J�����̈ʒu(���C�g�̈ʒu)
		m_lightCamera.SetPosition(0, 600, 0);
		// �J�����̒����_
		m_lightCamera.SetTarget(0, 0, 0);
		// �J�����̏����
		m_lightCamera.SetUp(1, 0, 0);
	    // ��p
		m_lightCamera.SetViewAngle(Math::DegToRad(20.0f));
		// ���C�g�r���[�v���W�F�N�V�����s��̌v�Z
		m_lightCamera.Update();
	}

	void Shadow::RenderToShadowMap(RenderContext& rc)
	{
		
		//�����_�����O�^�[�Q�b�g���V���h�E�}�b�v�ɕύX����B
		rc.WaitUntilToPossibleSetRenderTarget(m_shadowMap);
		rc.SetRenderTargetAndViewport(m_shadowMap);
		rc.ClearRenderTargetView(m_shadowMap);

		for (auto& model : m_shadowModelArray) {
			//�e���f����`��B
			model->Draw(rc, m_lightCamera);
		}

		m_shadowModelArray.clear();

		//�������݊����҂��B
		rc.WaitUntilFinishDrawingToRenderTarget(m_shadowMap);

		// �����_�����O�^�[�Q�b�g���t���[���o�b�t�@�ɕύX����B
		rc.SetRenderTarget(
			g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
			g_graphicsEngine->GetCurrentFrameBuffuerDSV()
		);
		rc.SetViewportAndScissor(g_graphicsEngine->GetFrameBufferViewport());
		
	}
	Shadow g_shadow;
}

