#pragma once
#include "sound/SoundSource.h"

class Player;
class Enemy2;
//�Q�[�����̃J�����𐧌䂷��B
class GameCamera : public IGameObject
{
public:
	enum EnMoveState
	{
		enMoveState_Idle,
		enMoveState_Walk,
	};

public:
	GameCamera();
	~GameCamera();

	/// <summary>
	/// ���W�̐ݒ�B
	/// </summary>
	/// <param name="position">���W�B</param>
	void SetPosition(Vector3& position)
	{
		m_position = position;
	}

	void SetRotation(Quaternion& rot)
	{
		m_rotation = rot;
	}
	/// <summary>
	/// ��K�̐؂�ւ��n�_�̐ݒ�B
	/// </summary>
	/// <param name="position"></param>
	void SetSwitchPosition(Vector3& position)
	{
		m_switchPos = position;
	}
	/// <summary>
	/// �J�����̍��W�̎擾
	/// </summary>
	/// <returns>�J�����̍��W</returns>
	const Vector3 GetPosition()
	{
		return m_position;
	}
	/// <summary>
	/// �J�����̍�����0�ɂ������W�̎擾
	/// </summary>
	/// <returns>�J�����̍��W</returns>
	const Vector3 GetYaxisZeroPosition()
	{
		Vector3 pos;
		pos = m_position;
		pos.y = 0.0f;
		return pos;
	}
	/// <summary>
	/// ��]�̎擾�B
	/// </summary>
	/// <returns></returns>
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
	/// <summary>
	/// ���������Ă��邩
	/// </summary>
	bool IsSound() const
	{
		return m_isSound;
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
	/// �`�揈��
	/// </summary>
	/// <param name="rc">�����_�[�R���e�L�X�g</param>
	void Render(RenderContext& rc);
	/// <summary>
	/// �e�����𒲂ׂ�
	/// </summary>
	void CheckRate();
	/// <summary>
	/// ����炷
	/// </summary>
	void MakeSound();


	/////////////////////////////////////
	//�����o�ϐ�
	/////////////////////////////////////
	Player* m_player = nullptr;										//�v���C���[�N���X�̃|�C���^�B
	Vector3 m_toCameraPos = Vector3::Zero;							//�����_���王�_�Ɍ������x�N�g���B
	Vector3 m_moveSpeed = Vector3::Zero;							//�ړ����x�B
	float m_multiplier = 0.0f;										//�ړ����x�ɏ�Z����l�B
	Vector3 m_position = Vector3::Zero;								//���W�B
	Vector3 m_stickL = Vector3::Zero;								//���X�e�B�b�N
	Vector3 m_switchPos = Vector3::Zero;							//��K�ւ̐؂�ւ��n�_�B
	EnMoveState m_moveState = enMoveState_Idle;						//�X�e�[�g�B
	Vector3 m_target = Vector3::Zero;								//�^�[�Q�b�g�B
	CharacterController m_charaCon;									//�L�����N�^�[�R���g���[���[�B
	Quaternion m_rotation = Quaternion::Identity;					//��]�B
	SoundSource* m_sound = nullptr;									//����
	ModelRender m_modelRender;										//���f��
	Vector3 m_up = Vector3::Zero;									//�J���������
	float m_rateByTime = 0.0f;										//�e����
	Enemy2* m_enemy = nullptr;										//�G�l�~�[
	bool m_isSound = false;											//���������Ă��邩
};
