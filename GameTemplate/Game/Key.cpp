#include "stdafx.h"
#include "Key.h"
#include "GameCamera.h"
#include "GroundFloor.h"
namespace
{
	const float DISTANCE = 100.0f;
}

bool Key::Start()
{
	//���f���̓ǂݍ���
	m_modelRender.Init("Assets/modelData/item/hammer.tkm");
	//���W�̐ݒ�
	m_modelRender.SetTRS(m_position, m_rotation, m_scale);
	//���f���̍X�V
	m_modelRender.Update();

	//�Q�[���J����
	m_gamecam = FindGO<GameCamera>("gamecamera");
	//��K
	m_groundfloor = FindGO<GroundFloor>("groundfloor");

	return true;
}

void Key::Update()
{
	if (m_havePiece < 2)
	{
		return;
	}
	Vector3 cameraPos = m_gamecam->GetPosition();
	cameraPos.y = m_position.y;
	Vector3 disToPlayer = m_position - cameraPos;
	//�E����͈͂�������
	if (disToPlayer.Length() <= DISTANCE)
	{
		m_groundfloor->PickKey();
		DeleteGO(this);
	}
	m_modelRender.Update();
}

void Key::Render(RenderContext& rc)
{
	if (m_havePiece < 2)
	{
		return;
	}
	m_modelRender.Draw(rc);
}