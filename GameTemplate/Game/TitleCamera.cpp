#include "stdafx.h"
#include "TitleCamera.h"

bool TitleCamera::Start()
{
	// カメラの初期設定
	// カメラの近平面と遠平面を設定
	g_camera3D->SetNear(3.0f);
	g_camera3D->SetFar(10000.0f);

	// 注視点を設定(仮)
	g_camera3D->SetTarget(g_camera3D->GetForward());

	// 視点を設定(仮)
	m_cameraPos = Vector3::Zero;

	return true;
}

void TitleCamera::Update()
{
	g_camera3D->Update();
}


