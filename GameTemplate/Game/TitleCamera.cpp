#include "stdafx.h"
#include "TitleCamera.h"

bool TitleCamera::Start()
{
	// �J�����̏����ݒ�
	// �J�����̋ߕ��ʂƉ����ʂ�ݒ�
	g_camera3D->SetNear(3.0f);
	g_camera3D->SetFar(10000.0f);

	// �����_��ݒ�(��)
	g_camera3D->SetTarget(g_camera3D->GetForward());

	// ���_��ݒ�(��)
	m_cameraPos = Vector3::Zero;

	return true;
}

void TitleCamera::Update()
{
	g_camera3D->Update();
}


