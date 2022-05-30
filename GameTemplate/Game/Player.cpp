#include "stdafx.h"
#include "Player.h"
#include "GameCamera.h"
#include "Enemy2.h"

//�萔
namespace
{
	const float MOVESPEED = 1000.0f;
	const float MOVE_RUN = 1.4f;
	const float MOVE_SNEAK = 0.58f;
}

//����������
bool Player::Start()
{

	m_gamecam = FindGO<GameCamera>("gamecamera");

	m_position = m_gamecam->GetPosition();
	m_position.y = 0.0f;

	
	m_enemy = FindGO<Enemy2>("enemy");
	Vector3 position;
	position = m_position;
	position.y = 0.0f;
	return true;
}

Player::~Player()
{

}

//�X�V����
void Player::Update()
{
	//�ړ�����
	Move();
	//��]�����B
	Rotation();
	//�X�e�[�g�Ǘ��B
	ManageState();	
	
	m_modelRender.Update();
}

void Player::Move()
{
	//FPS���_�̂��߁A�ړ�����W�̏��̓J�������ێ����Ă���B
	//�J�����̍��W���擾����B
	m_position = m_gamecam->GetPosition();
	//Y���W��0�ɂ���B
	m_position.y = 0.0f;
	//���W��ݒ肷��B
	m_modelRender.SetPosition(m_position);
}

void Player::Rotation()
{
	//�J�����̌������擾
	Vector3 rotVec = m_gamecam->GetToCameraPos();
	//y�̒l�̏��͗v��Ȃ��̂ŁA0.0f�������Đ��K���B
	rotVec.y = 0.0f;
	rotVec.Normalize();

	//���݂ǂꂾ����]���Ă���̂��̊p�x�����߂�B
	float angle = atan2(rotVec.x, rotVec.z);
	//���̊p�x���g���āAY������Quaternion��ݒ肷��B
	m_rotation.SetRotation(Vector3::AxisY, angle);

	//Quaternion���g���āA���f���̉�]��ݒ�
	m_modelRender.SetRotation(m_rotation);
}

void Player::TransitionState()
{
	if (m_enemy->IsGameOver() == false) {
		return;
	}
	//�_�E����Ԃɂ���B
	m_playerState = enPlayerState_Stop;
}
//�X�e�[�g�Ǘ�
void Player::ManageState()
{
	switch (m_playerState)
	{
		//�m�[�n���h�X�e�[�g
	case enPlayerState_Nomal:
		NomalState();
		break;
		//�_���[�W���
	case enPlayerState_Damage:
		DamageState();
		break;
		//�_�E�����
	case enPlayerState_Stop:
		DownState();
		break;
	default:
		break;
	}
}
//
void Player::NomalState()
{
	TransitionState();
}
//�_���[�W�X�e�[�g���̏���
void Player::DamageState()
{
	TransitionState();
}
//�_�E���X�e�[�g���̏���
void Player::DownState()
{
	TransitionState();
}

//�`�揈��
void Player::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}