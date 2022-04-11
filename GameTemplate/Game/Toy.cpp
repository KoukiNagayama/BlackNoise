#include "stdafx.h"
#include "Toy.h"
#include "GameCamera.h"

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
	// �X�e�[�g���ʏ�Ȃ�
	if (m_toyState == enToyState_usual) {
		// �v���C���[�Ƃ̋���
		// �J�����̍��W
		m_gameCameraPos = m_gameCamera->GetPosition();
		m_gameCameraPos.y = 0.0f;
		// ���W�̍�
		Vector3	distToPlayer = m_position - m_gameCameraPos;
		if (distToPlayer.Length() <= 10.0f) {
			// ���f����ύX
			m_modelRender.Init("Assets/modelData/item/clash_toy.tkm");
			// �X�e�[�g��ύX
			m_toyState = enToyState_collapse;
		}
	}

	m_modelRender.Update();
}
void Toy::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}