#include "stdafx.h"
#include "TitleText.h"

namespace
{
	const float VALUE_OF_SCALE = 0.90f;						// �g�嗦�̒l
	const float ADJUSTMENT_VALUE_OF_POSITION = 4.0f;		// ���W�̒����l
	const float ADJUSTMENT_VALUE_OF_ROTATION = -26.0f;		// ��]�̒����l
}
bool TitleText::Start()
{
	m_modelRender.Init("Assets/modelData/title/title.tkm", nullptr, 0, false, enModelUpAxisZ, false, 0, 3);

	// �g�嗦
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
