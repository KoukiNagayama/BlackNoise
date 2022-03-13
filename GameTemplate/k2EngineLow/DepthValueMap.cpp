#include "k2EngineLowPreCompile.h"
#include "DepthValueMap.h"

namespace nsK2EngineLow {
	void DepthValueMap::Init()
	{
		InitRenderTargetForDepthValueMap();

		InitCamera();
	}

	void DepthValueMap::InitRenderTargetForDepthValueMap()
	{
		m_depthValueMap.Create(
			1600,
			900,
			1,
			1,
			DXGI_FORMAT_R32_FLOAT,
			DXGI_FORMAT_D32_FLOAT
		);
	}

	void DepthValueMap::InitCamera()
	{
		// �J�����̈ʒu(���C�g�̈ʒu)
		m_camera.SetPosition(g_camera3D->GetPosition());
		// �J�����̒����_
		m_camera.SetTarget(g_camera3D->GetTarget());
		// �J�����̏����
		m_camera.SetUp(g_camera3D->GetUp());
		// ��p
		m_camera.SetViewAngle(g_camera3D->GetViewAngle());
		// ������
		m_camera.SetFar(1000.0f);
		// �ߕ���
		m_camera.SetNear(5.0f);
	}

	void DepthValueMap::Update()
	{
		// �J�����̈ʒu(���C�g�̈ʒu)
		m_camera.SetPosition(g_camera3D->GetPosition());
		// �J�����̒����_
		m_camera.SetTarget(g_camera3D->GetTarget());
		// ���C�g�r���[�v���W�F�N�V�����s��̌v�Z
		m_camera.Update();
	}

	void DepthValueMap::RenderToDepthValueMap(RenderContext& rc)
	{
		// �����_�����O�^�[�Q�b�g��[�x�l�}�b�v�ɕύX����B
		rc.WaitUntilToPossibleSetRenderTarget(m_depthValueMap);
		rc.SetRenderTargetAndViewport(m_depthValueMap);
		rc.ClearRenderTargetView(m_depthValueMap);

		for (auto& model : m_modelArray) {
			// ���f����`��B
			model->Draw(rc, m_camera);
		}

		m_modelArray.clear();

		// �������݊����҂��B
		rc.WaitUntilFinishDrawingToRenderTarget(m_depthValueMap);

		// �����_�����O�^�[�Q�b�g���t���[���o�b�t�@�ɕύX����B
		rc.SetRenderTarget(
			g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
			g_graphicsEngine->GetCurrentFrameBuffuerDSV()
		);
		rc.SetViewportAndScissor(g_graphicsEngine->GetFrameBufferViewport());
	}

	DepthValueMap g_depthValueMap;
}