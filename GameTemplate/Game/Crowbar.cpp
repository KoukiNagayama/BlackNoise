#include "stdafx.h"
#include "Crowbar.h"
#include "GameCamera.h"
#include "Item.h"

namespace
{
	const float DISTANCE = 100.0f;	//�v���C���[�܂ł̋���
}

bool Crowbar::Start()
{
	//���f���̓ǂݍ���
	m_modelRender.Init("Assets/modelData/item/crowbar.tkm");
	//���W�̐ݒ�
	m_modelRender.SetTRS(m_position, m_rotation, m_scale);
	//���f���̍X�V
	m_modelRender.Update();

	//�u���[�N���h�A
	m_item = FindGO<Item>("item");
	//�Q�[���J����
	m_gamecam = FindGO<GameCamera>("gamecamera");

	return true;
}

void Crowbar::Update()
{
	Vector3 cameraPos = m_gamecam->GetPosition();
	cameraPos.y = m_position.y;
	Vector3 disToPlayer = m_position - cameraPos;
	//�E����͈͂�������
	if (disToPlayer.Length() <= DISTANCE)
	{
		if (g_pad[0]->IsTrigger(enButtonA))
		{
			m_item->SetCrowbarToPendingItem();
			DeleteGO(this);
		}
	}
	m_modelRender.Update();
}

void Crowbar::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}
