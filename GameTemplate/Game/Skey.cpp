#include "stdafx.h"
#include "Skey.h"
Skey::Skey()
{
	
}

Skey::~Skey()
{

}

bool Skey::Start()
{
	m_modelRender.Init("Assets/modelData/item/key.tkm");
	m_modelRender.SetPosition(m_position);
	m_modelRender.Update();

	return true;
}

void Skey::Update()
{
	m_modelRender.SetPosition(m_position);

	m_modelRender.Update();
}
void Skey::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}