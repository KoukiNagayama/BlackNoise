#include "stdafx.h"
#include "Record.h"

bool Record::Start()
{
	m_modelRender.Init("Assets/modelData/toy.tkm");
	m_scale = Vector3{ 100.0f, 100.0f, 100.0f };
	m_modelRender.SetTRS(m_position, m_rotation, m_scale);
	return true;
}

void Record::Update()
{

}

void Record::Render(RenderContext& rc)
{

}
