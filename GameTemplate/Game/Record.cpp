#include "stdafx.h"
#include "Record.h"

bool Record::Start()
{
	m_modelRender.Init("Assets/modelData/item/record_item.tkm");
	m_modelRender.SetTRS(m_position, m_rotation, m_scale);

	if (m_number == 1) {
		m_position = Vector3(0.0f, 50.0f, 0.0f);
	}
	else if (m_number == 2) {
		m_position = Vector3(0.0f, 0.0f, 0.0f);
	}
	
	return true;
}

void Record::Update()
{
	m_modelRender.Update();
}

void Record::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}
