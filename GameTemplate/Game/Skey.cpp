#include "stdafx.h"
#include "Skey.h"
Skey::Skey()
{
	m_modelRender.Init("Assets/modelData/item/key.tkm");
	m_modelRender.Update();

}

Skey::~Skey()
{

}

void Skey::Update()
{
	m_modelRender.Update();
}
void Skey::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}