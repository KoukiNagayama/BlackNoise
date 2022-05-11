#include "stdafx.h"
#include "TitleCamera.h"
#include "TitleText.h"

bool TitleCamera::Start()
{
	m_tText = FindGO<TitleText>("titleText");
	Vector3	target = m_tText->GetPosition();
	// �J�����̏����ݒ�
	// �J�����̋ߕ��ʂƉ����ʂ�ݒ�
	g_camera3D->SetNear(3.0f);
	g_camera3D->SetFar(10000.0f);

	// �����_��ݒ�
	g_camera3D->SetTarget(target);

	// ���_��ݒ�
	g_camera3D->SetPosition(m_cameraPos);

	return true;
}

void TitleCamera::Update()
{
	g_camera3D->Update();
}


