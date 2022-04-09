#include "stdafx.h"
#include "Key.h"
Key::Key()
{
	m_modelRender.Init("Assets/modelData/item/key.tkm");
	m_modelRender.Update();

}

Key::~Key()
{

}

void Key::Update()
{
	m_modelRender.Update();
}
void Key::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}