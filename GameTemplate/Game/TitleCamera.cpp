#include "stdafx.h"
#include "TitleCamera.h"
#include "TitleText.h"

bool TitleCamera::Start()
{
	m_tText = FindGO<TitleText>("titleText");
	Vector3	target = m_tText->GetPosition();
	// カメラの初期設定
	// カメラの近平面と遠平面を設定
	g_camera3D->SetNear(3.0f);
	g_camera3D->SetFar(10000.0f);

	// 注視点を設定
	g_camera3D->SetTarget(target);

	// 視点を設定
	g_camera3D->SetPosition(m_cameraPos);

	return true;
}

void TitleCamera::Update()
{
	g_camera3D->Update();
}


