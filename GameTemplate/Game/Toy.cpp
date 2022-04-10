#include "stdafx.h"
#include "Toy.h"

Toy::Toy()
{
	m_modelRender.Init("Assets/modelData/item/key.tkm");
	m_modelRender.SetPosition(m_position);
	m_modelRender.Update();
}

Toy::~Toy()
{

}

void Toy::Update()
{
	m_modelRender.Update();
}
void Toy::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}