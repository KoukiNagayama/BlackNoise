#pragma once

class GameCamera;

class Player : public IGameObject
{
public:
	/// <summary>
	/// �X�e�[�g
	/// </summary>
	enum EnPlayerState {
		enPlayerState_Idle,			//�ҋ@�X�e�[�g
		enPlayerState_Walk,			//�����X�e�[�g
		enPlayerState_Run,			//����X�e�[�g
		enPlayerState_Sneak,		//���Ⴊ�݁A�B��X�e�[�g
		enPlayerState_SneakIdle,	//���Ⴊ�݁A�B��ҋ@�X�e�[�g
		enPlayerState_Damage,		//�_���[�W�X�e�[�g
		enPlayerState_Down			//�_�E���X�e�[�g
	};
public:
	Player() {};
	~Player();
	/// <summary>
	/// �������֐�
	/// </summary>
	/// <returns>true</returns>
	bool Start();
	/// <summary>
	/// �ړ������̐����B
	/// </summary>
	/// <returns></returns>
	bool IsEnableMove() const
	{
		return m_playerState != enPlayerState_Damage &&
			m_playerState != enPlayerState_Down;
	}
	/// <summary>
	/// �_���[�W�X�e�[�g�̐ݒ�
	/// </summary>
	void ReceiveDamege()
	{
		m_playerState = enPlayerState_Damage;
	}
	/// <summary>
	/// ���W��n���B
	/// </summary>
	/// <returns>m_positon</returns>
	const Vector3 GetPosition()
	{
		return m_position;
	}

private:
	/// <summary>
	/// �X�V����
	/// </summary>
	void Update();
	/// <summary>
	/// �ړ�����
	/// </summary>
	void Move();
	/// <summary>
	/// ��]����
	/// </summary>
	void Rotation();
	/// <summary>
	/// �`�揈��
	/// </summary>
	/// <param name="rc">�����_�[�R���e�L�X�g</param>
	void Render(RenderContext& rc);
	/// <summary>
	/// �X�e�[�g�J�ڏ����B
	/// </summary>
	void TransitionState();
	/// <summary>
	/// �X�e�[�g�Ǘ��B
	/// </summary>
	void ManageState();
	/// <summary>
	/// �ҋ@�X�e�[�g���̏����B
	/// </summary>
	void IdleState();
	/// <summary>
	/// �����X�e�[�g���̏����B
	/// </summary>
	void WalkState();
	/// <summary>
	/// ����X�e�[�g���̏����B
	/// </summary>
	void RunState();
	/// <summary>
	/// ���Ⴊ�݁E�B��X�e�[�g���̏����B
	/// </summary>
	void SneakState();
	/// <summary>
	/// ���Ⴊ�ݑҋ@�X�e�[�g���̏����B
	/// </summary>
	void SneakIdleState();
	/// <summary>
	/// �_���[�W�X�e�[�g���̏����B
	/// </summary>
	void DamageState();
	/// <summary>
	/// �_�E���X�e�[�g���̏����B
	/// </summary>
	void DownState();

	//�����o�ϐ�
	//�A�j���[�V�����N���b�v
	enum EnAnimationClip
	{
		enAnimationClip_Idle,			//�ҋ@�A�j���[�V����
		enAnimationClip_Walk,			//�����A�j���[�V����
		enAnimationClip_Run,			//����A�j���[�V����
		enAnimationClip_Sneak,			//���Ⴊ�݁E�B��A�j���[�V����
		enAnimationClip_SneakIdle,		//���Ⴊ�݁E�B��A�C�h���A�j���[�V����
		enAnimationClip_Damage,			//�_���[�W�A�j���[�V����
		enAnimationClip_Down,			//�_�E���A�j���[�V����
		enAnimationClip_Num				//�A�j���[�V�����̐�
	};

	int m_hp = 3;											//HP

	Vector3 m_moveSpeed;									//�ړ����x
	Vector3 m_toCameraPos;									//�J�����̒����_���王�_�Ɍ������x�N�g��
	Vector3 m_position ;									//���W
	Quaternion m_rotation = Quaternion::Identity;			//��]
	EnPlayerState m_playerState = enPlayerState_Idle;		//�X�e�[�g
	ModelRender m_modelRender;								//���f�������_�[
	AnimationClip animationClips[enAnimationClip_Num];		//�A�j���[�V�����N���b�v
	CharacterController m_charaCon;							//�L�����N�^�[�R���g���[���[
	GameCamera* m_gamecam = nullptr;						//�Q�[���J�����̃|�C���^
};

