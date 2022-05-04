#include "stdafx.h"
#include "Piece.h"
#include "GameCamera.h"
#include "Key.h"

namespace
{
	const float DISTANCE = 100.0f;
}

bool Piece::Start()
{
	//���f���̓ǂݍ���
	m_modelRender.Init("Assets/modelData/item/hammer.tkm");
	//���W�̐ݒ�
	m_modelRender.SetTRS(m_position,m_rotation,m_scale);
	//���f���̍X�V
	m_modelRender.Update();

	//�Q�[���J����
	m_gamecam = FindGO<GameCamera>("gamecamera");
	//�Q�[���J����
	m_key = FindGO<Key>("key");

	return true;
}
void Piece::Update()
{
	Vector3 cameraPos = m_gamecam->GetPosition();
	cameraPos.y = m_position.y;
	Vector3 disToPlayer = m_position - cameraPos;
	//�E����͈͂�������
	if (disToPlayer.Length() <= DISTANCE)
	{
		m_key->PickPiece();
		DeleteGO(this);
	}
	m_modelRender.Update();
}

void Piece::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}