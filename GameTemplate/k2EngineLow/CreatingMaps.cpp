#include "k2EngineLowPreCompile.h"
#include "CreatingMaps.h"

namespace nsK2EngineLow {
	void CreatingMaps::Init()
	{
		InitRenderTargetForDepthValueMap();

		InitRenderTargetForWorldCoordinateMap();

		InitRenderTargetForNormalMap();

		InitCamera();
	}

	void CreatingMaps::InitRenderTargetForDepthValueMap()
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

	void CreatingMaps::InitRenderTargetForWorldCoordinateMap()
	{
		float clearColor[4] = { 3000.0f, 3000.0f, 3000.0f, 1.0f };
		m_worldCoordinateMap.Create(
			1600,
			900,
			1,
			1,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_D32_FLOAT,
			clearColor
		);
	}

	void CreatingMaps::InitRenderTargetForNormalMap()
	{
		m_normalMap.Create(
			1600,
			900,
			1,
			1,
			DXGI_FORMAT_R16G16B16A16_FLOAT,
			DXGI_FORMAT_D32_FLOAT
		);
	}

	void CreatingMaps::InitCamera()
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
		m_camera.SetFar(10000.0f);
		// �ߕ���
		m_camera.SetNear(5.0f);
		// ���C�g�r���[�v���W�F�N�V�����s��̌v�Z
		m_camera.Update();
	}

	void CreatingMaps::Update()
	{
		// �J�����̈ʒu(���C�g�̈ʒu)
		m_camera.SetPosition(g_camera3D->GetPosition());
		// �J�����̒����_
		m_camera.SetTarget(g_camera3D->GetTarget());
		// ���C�g�r���[�v���W�F�N�V�����s��̌v�Z
		m_camera.Update();
	}

	void CreatingMaps::RenderToDepthValueMap(RenderContext& rc)
	{
		RenderTarget* rts[] = {
			&m_depthValueMap,
			&m_worldCoordinateMap,
			&m_normalMap
		};

		// �����_�����O�^�[�Q�b�g��ύX����B
		rc.WaitUntilToPossibleSetRenderTargets(3, rts);
		rc.SetRenderTargetsAndViewport(3, rts);
		rc.ClearRenderTargetViews(3, rts);

		for (auto& model : m_modelArray) {
			// ���f����`��B
			model->Draw(rc, m_camera);
		}

		m_modelArray.clear();

		// �������݊����҂��B
		rc.WaitUntilFinishDrawingToRenderTargets(3, rts);

		// �����_�����O�^�[�Q�b�g���t���[���o�b�t�@�ɕύX����B
		rc.SetRenderTarget(
			g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
			g_graphicsEngine->GetCurrentFrameBuffuerDSV()
		);
		rc.SetViewportAndScissor(g_graphicsEngine->GetFrameBufferViewport());
	}

	CreatingMaps g_creatingMaps;
}
