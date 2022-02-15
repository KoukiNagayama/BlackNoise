#pragma once
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
	/// �_���[�W�X�e�[�g���̏����B
	/// </summary>
	void DamageState();
	/// <summary>
	/// �_�E���X�e�[�g���̏����B
	/// </summary>
	void DownState();

	//�����o�ϐ�
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

	Vector3 m_moveSpeed;
	Vector3 m_forward;
	Vector3 m_position;
	Quaternion m_rotation;
	EnPlayerState m_playerState = enPlayerState_Idle;
	ModelRender m_modelRender;
	AnimationClip animationClips[enAnimationClip_Num];
	CharacterController m_charaCon;
};

