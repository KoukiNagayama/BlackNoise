#include "stdafx.h"
#include "Key.h"
Key::Key()
{


}

Key::~Key()
{

}

bool Key::Start()
{
	m_modelRender.Init("Assets/modelData/item/key.tkm");
	m_modelRender.SetPosition(m_position);
	m_modelRender.Update();

	return true;
}

void Key::Update()
{
	m_modelRender.SetPosition(m_position);

	m_modelRender.Update();
}
void Key::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}