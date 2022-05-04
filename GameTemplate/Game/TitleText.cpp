#include "stdafx.h"
#include "TitleText.h"

namespace
{
	const float VALUE_OF_SCALE = 0.05f;						// 拡大率の値
	const float ADJUSTMENT_VALUE_OF_POSITION = 4.0f;		// 座標の調整値
	const float ADJUSTMENT_VALUE_OF_ROTATION = -26.0f;		// 回転の調整値
}
bool TitleText::Start()
{
	m_modelRender.Init("Assets/modelData/title/title.tkm");

	// 座標
	m_position = g_camera3D->GetPosition();
	m_position += g_camera3D->GetForward() * 10.0f;

	// 座標と回転の調整
	m_position.y -= ADJUSTMENT_VALUE_OF_POSITION;
	m_rotation.AddRotationDegX(ADJUSTMENT_VALUE_OF_ROTATION);

	// 拡大率
	m_scale = Vector3(VALUE_OF_SCALE, VALUE_OF_SCALE, VALUE_OF_SCALE);


	m_modelRender.SetTRS(m_position, m_rotation, m_scale);
	
	return true;
}

void TitleText::Update()
{
	m_modelRender.Update();
}

void TitleText::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}
