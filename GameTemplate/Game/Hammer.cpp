#include "stdafx.h"
#include "Hammer.h"
#include "BrokenDoor.h"
#include "GameCamera.h"

namespace
{
	const float DISTANCE = 100.0f;	//�v���C���[�܂ł̋���
}

bool Hammer::Start()
{
	//���f���̓ǂݍ���
	m_modelRender.Init("Assets/modelData/item/hammer.tkm", nullptr, 0, false, enModelUpAxisZ, false, 0, 1);
	//���W�̐ݒ�
	m_modelRender.SetPosition(m_position);
	//���f���̍X�V
	m_modelRender.Update();

	//�u���[�N���h�A
	m_b_door = FindGO<BrokenDoor>("brokendoor");
	//�Q�[���J����
	m_gamecam = FindGO<GameCamera>("gamecamera");

	return true;
}

void Hammer::Update()
{
	Vector3 cameraPos = m_gamecam->GetPosition();
	cameraPos.y = m_position.y;
	m_disToPlayer = m_position - cameraPos;
	//�E����͈͂�������
	if (m_disToPlayer.Length() <= DISTANCE)
	{
		m_b_door->PickHammer();
		DeleteGO(this);
	}
	m_modelRender.Update();
}

void Hammer::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}