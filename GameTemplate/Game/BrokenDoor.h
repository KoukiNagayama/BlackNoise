#pragma once
class GameCamera;
class BrokenDoor : public IGameObject
{

public:
	BrokenDoor() {};
	~BrokenDoor() {};
	/// <summary>
	/// ����������
	/// </summary>
	/// <returns>true</returns>
	bool Start();
	/// <summary>
	/// ��]�̐ݒ�
	/// </summary>
	/// <param name="rotation">�N�H�[�^�j�I��</param>
	void SetRotation(Quaternion rotation)
	{
		m_rotation = rotation;
	}
	/// <summary>
	/// ���W�̐ݒ�
	/// </summary>
	/// <param name="position">���W</param>
	void SetPosition(Vector3 position)
	{
		m_position = position;
	}
	/// <summary>
	/// �n���}�[���E����
	/// </summary>
	void PickHammer()
	{
		m_haveHammer = true;
	}
private:
	enum EnBreakDoorState
	{
		enDoorState_Before,			//�h�A���󂷑O
		enDoorState_CloseIdle,		//�N���[�Y�ҋ@
		enDoorState_Close,			//�N���[�Y
		enDoorState_OpenIdle,		//�I�[�v���ҋ@
		enDoorState_Open			//�I�[�v��
	};
private:
	/////�֐�/////
	/// <summary>
	/// �X�V����
	/// </summary>
	void Update();
	/// <summary>
	/// �X�e�[�g�Ǘ�����
	/// </summary>
	void ManageState();
	/// <summary>
	/// �`�揈��
	/// </summary>
	/// <param name="rc">�����_�[�R���e�L�X�g</param>
	void Render(RenderContext& rc);
	/// <summary>
	/// �t�B�W�b�N�X�I�u�W�F�N�g�̐���
	/// </summary>
	void CreatePhysicsObject();
	/// <summary>
	/// �t�B�W�b�N�X�I�u�W�F�N�g�̊J��
	/// </summary>
	void ReleasePhysicsObject();
	/// <summary>
	/// �X�e�[�g�J�ڏ���
	/// </summary>
	void TransitionState();
	/// <summary>
	/// �󂷑O�X�e�[�g
	/// </summary>
	void BeforeState();
	/// <summary>
	/// �N���[�Y�ҋ@�X�e�[�g
	/// </summary>
	void CloseIdleState();
	/// <summary>
	/// �N���[�Y�X�e�[�g
	/// </summary>
	void CloseState();
	/// <summary>
	/// �I�[�v���X�e�[�g
	/// </summary>
	void OpenState();
	/// <summary>
	/// �I�[�v���ҋ@�X�e�[�g
	/// </summary>
	void OpenIdleState();
	/// <summary>
	/// �A�j���[�V�����̍Đ�
	/// </summary>
	void PlayAnimation();
	/// <summary>
	/// �h�A�ɋ߂���
	/// </summary>
	/// <returns>�߂��Ƃ�->true</returns>
	bool NearDoor();
	/// <summary>
	/// �h�A���󂹂邩
	/// </summary>
	bool CanBreakDoor();

	/////�ϐ�/////
	enum EnAnimationClip {					//�A�j���[�V�����B
		enAnimationClip_Before,				//�󂷑O�B
		enAnimationClip_CloseIdle,			//�ҋ@�B	
		enAnimationClip_OpenIdle,			//�I�[�v���B
		enAnimationClip_Close,				//�N���[�Y�B
		enAnimationClip_Open,				//�I�[�v��
		enAnimationClip_Num,				//�A�j���[�V�����̐��B
	};
	AnimationClip				m_animationClips[enAnimationClip_Num];	//�A�j���[�V�����N���b�v�B
	ModelRender					m_modelRender;							//���f��
	Quaternion					m_rotation;								//��]
	PhysicsStaticObject			m_physicsStaticObject;					//�t�B�W�N�X�X�^�e�B�b�N�I�u�W�F�N�g�B	
	Vector3						m_position = Vector3::Zero;								//���W
	EnBreakDoorState			m_doorState = enDoorState_Before;		//�X�e�[�g
	GameCamera*					m_gamecam = nullptr;					//�Q�[���J����
	bool						m_haveHammer = false;					//�n���}�[�������Ă��邩
};

