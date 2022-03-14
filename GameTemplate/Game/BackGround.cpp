#include "stdafx.h"
#include "BackGround.h"

BackGround::BackGround()
{
	//コメントアウトする。
	//PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();

	m_modelRender.Init("Assets/modelData/stage/Sample.tkm");

	m_modelRender.Update();
}

BackGround::~BackGround()
{

}

void BackGround::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}