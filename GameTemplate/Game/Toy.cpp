#include "stdafx.h"
#include "Toy.h"
#include "GameCamera.h"

Toy::Toy()
{
}

Toy::~Toy()
{
	
}

bool Toy::Start()
{
	m_modelRender.Init("Assets/modelData/item/toy.tkm");
	m_modelRender.SetPosition(m_position);
	m_modelRender.Update();
	m_gameCamera = FindGO<GameCamera>("gamecamera");

	return true;
}

void Toy::Update()
{
	// �v���C���[�Ƃ̋���
	// �J�����̍��W
	m_gameCameraPos = m_gameCamera->GetPosition();
	m_gameCameraPos.y = 0.0f;
	// ���W�̍�
	Vector3	distToPlayer = m_position - m_gameCameraPos;
	if (distToPlayer.Length() <= 10.0f) {
		if (m_toyState == enToyState_usual) {
			m_modelRender.Init("Assets/modelData/item/clash_toy.tkm");
			m_toyState = enToyState_collapse;
		}
	}

	m_modelRender.Update();
}
void Toy::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}