#include "stdafx.h"
#include "FloorGimmick.h"
#include "GameCamera.h"
#include "Item.h"

namespace
{
	const float DISTANCE = 100.0f;
}

//FloorGimmick::~FloorGimmick()
//{
//
//}

bool FloorGimmick::Start()
{
	//���f����ǂݍ��ށB
	m_modelRender.Init("Assets/modelData/stage/wood1.tkm");
	//���W��ݒ肷��B
	m_modelRender.SetTRS(m_position, m_rotation, m_scale);
	//���f���̍X�V�B
	m_modelRender.Update();
	//�����蔻����쐬����B
	m_physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());
	//�Q�[���J����
	m_gamecam = FindGO<GameCamera>("gamecamera");
	//�A�C�e��
	m_item = FindGO<Item>("item");

	return true;
}

void FloorGimmick::CreatePhysicsObject()
{
	m_physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());
}

void FloorGimmick::ReleasePhysicsObject()
{
	m_physicsStaticObject.Release();
}

void FloorGimmick::Update()
{
	NearGimmick();

	m_modelRender.Update();
}

void FloorGimmick::NearGimmick()
{
	Vector3 cameraPos = m_gamecam->GetPosition();
	cameraPos.y = m_position.y;
	Vector3 disToPlayer = m_position - cameraPos;
	//�o�[���������Ă��āA�M�~�b�N�ɋ߂���
	if (m_item->GetHaveCrowbar() != false && disToPlayer.Length() <= DISTANCE)
	{
		switch (m_attack)
		{
		case 2:
			if (g_pad[0]->IsTrigger(enButtonA))
			{
				ReleasePhysicsObject();
				//���f���̐؂�ւ�
				m_modelRender.Init("Assets/modelData/stage/wood2.tkm");
				m_attack -= 1;
				CreatePhysicsObject();
			}
			break;
		case 1:
			if (g_pad[0]->IsTrigger(enButtonA))
			{
				ReleasePhysicsObject();
				//���f���̐؂�ւ�
				m_modelRender.Init("Assets/modelData/stage/wood.tkm");
				m_attack -= 1;
				CreatePhysicsObject();
			}
			break;
		case 0:
			break;
		default:
			break;
		}
	}
}

void FloorGimmick::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}