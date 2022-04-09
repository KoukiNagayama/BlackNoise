#pragma once
#include "sound/SoundSource.h"


class Player;
//�Q�[�����̃J�����𐧌䂷��B
class GameCamera : public IGameObject
{
public:
	enum EnMoveState
	{
		enMoveState_Idle,
		enMoveState_Walk,
		enMoveState_Run,
		enMoveState_Sneak,
		enMoveState_SneakIdle
	};

public:
	GameCamera();
	~GameCamera();

	/// <summary>
	/// ���݂̃X�e�[�g�����Ⴊ�ݏ�ԂłȂ���
	/// </summary>
	/// <returns>���Ⴊ�ݏ�Ԃ̎���false</returns>
	bool IsSneak() const
	{
		return m_moveState != enMoveState_Sneak &&
			m_moveState != enMoveState_SneakIdle;
	}

	void SetPosition(Vector3& position)
	{
		m_position = position;
	}
	/// <summary>
	/// �J�����̍��W�̎擾
	/// </summary>
	/// <returns>�J�����̍��W</returns>
	const Vector3 GetPosition()
	{
		return m_position;
	}

	const Quaternion GetRotation()
	{
		return m_rotation;
	}
	/// <summary>
	/// �����_���王�_�Ɍ������x�N�g���̎擾
	/// </summary>
	/// <returns>�����_���王�_�Ɍ������x�N�g��</returns>
	const Vector3 GetToCameraPos()
	{
		return m_toCameraPos;
	}
	/// <summary>
	/// ���X�e�B�b�N�����͒����B
	/// </summary>
	/// <returns>���͒� -> true</returns>
	bool IsStickMove() const
	{
		return m_stickL.x > 0.0f || m_stickL.y > 0.0f;
	}
	/// <summary>
	/// �ړ������B
	/// </summary>
	/// <returns>�ړ��� -> true</returns>
	bool IsMoveNow() const
	{
		return fabsf(m_moveSpeed.x) >= 0.001f && fabsf(m_moveSpeed.z) >= 0.001f;
	}

private:
	/////////////////////////////////////
	//�����o�֐�
	/////////////////////////////////////

	/// <summary>
	/// ����������
	/// </summary>
	/// <returns>true</returns>
	bool Start();
	/// <summary>
	/// �X�V����
	/// </summary>
	void Update();
	/// <summary>
	/// �X�e�[�g�J�ڏ����B
	/// </summary>
	void TransitionState();
	/// <summary>
	/// �ړ������B
	/// </summary>
	void Move();
	/// <summary>
	/// �����_�����B
	/// </summary>
	void ViewPoint();
	/// <summary>
	/// �X�e�[�g�Ǘ�
	/// </summary>
	void ManageState();
	/// <summary>
	/// �ҋ@�X�e�[�g�B
	/// </summary>
	void IdleState();
	/// <summary>
	/// �����X�e�[�g�B
	/// </summary>
	void WalkState();
	/// <summary>
	/// ����X�e�[�g�B
	/// </summary>
	void RunState();
	/// <summary>
	/// ���Ⴊ�݃X�e�[�g�B
	/// </summary>
	void SneakState();
	/// <summary>
	/// ���Ⴊ�ݑҋ@�X�e�[�g�B
	/// </summary>
	void SneakIdleState();

	void Render(RenderContext& rc);


	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);


	/////////////////////////////////////
	//�����o�ϐ�
	/////////////////////////////////////
	Player* m_player;								//�v���C���[�N���X�̃|�C���^�B
	Vector3 m_toCameraPos;							//�����_���王�_�Ɍ������x�N�g���B
	Vector3 m_moveSpeed;							//�ړ����x�B
	float m_multiplier;								//�ړ����x�ɏ�Z����l�B
	Vector3 m_position;								//���W�B
	Vector3 m_stickL;								//���X�e�B�b�N
	EnMoveState m_moveState = enMoveState_Idle;		//�X�e�[�g�B
	Vector3 m_target;								//�^�[�Q�b�g�B
	CharacterController m_charaCon;					//�L�����N�^�[�R���g���[���[�B
	Quaternion m_rotation;							//��]�B
	SoundSource* m_walkSound;
	ModelRender m_modelRender;
	Vector3 m_up;
	float rate = 0.0f;
	float beforeRate;
};
