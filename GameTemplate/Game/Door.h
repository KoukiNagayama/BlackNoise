#pragma once
#include "sound/SoundSource.h"
class GameCamera;
class Enemy;

class Door : public IGameObject
{
public:
	//�h�A�X�e�[�g�B
	enum EnDoorState {
		enDoorState_Open,				//�I�[�v���B
		enDoorState_OpenIdle,			//�I�[�v�����I������B
		enDoorState_Close,				//�N���[�Y�B
		enDoorState_CloseIdle			//�N���[�Y���I������B
	};
public:
	Door();
	~Door();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	/// <summary>
	/// �X�e�[�g�J�ڏ����B
	/// </summary>
	void TransitionState();
	/// <summary>
	/// ���W��ݒ肷��B
	/// </summary>
	/// <param name="position">���W�B</param>
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}
	/// <summary>
	/// ��]��ݒ肷��B
	/// </summary>
	/// <param name="rotation">��]�B</param>
	void SetRotation(const Quaternion& rotation)
	{
		m_rotation = rotation;
	}
	/// <summary>
	/// �傫����ݒ肷��B
	/// </summary>
	/// <param name="scale">�傫���B</param>
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
	}
	/// <summary>
	/// �h�A�̔ԍ���ݒ肷��B
	/// </summary>
	/// <param name="doorNumber">�h�A�̔ԍ��B</param>
	void SetDoorNumber(const int doorNumber)
	{
		m_doorNumber = doorNumber;
	}
private:
	/// <summary>
	/// �h�A���J������ʒu�ɂ��邩
	/// </summary>
	/// <returns>�v���C���[���߂����true</returns>
	bool NearPlayer();
	/// <summary>
	/// �G�l�~�[���h�A���J������ʒu�ɂ��邩
	/// </summary>
	/// <returns>Enemy���߂����true</returns>
	bool NearEnemy();
	/// <summary>
	/// �A�j���[�V�������Đ�����B
	/// </summary>
	void PlayAnimation();
	/// <summary>
	/// �t�B�W�b�N�X�I�u�W�F�N�g�̓����蔻����J������B
	/// </summary>
	void ReleasePhysicsObject();
	/// <summary>
	/// �t�B�W�b�N�X�I�u�W�F�N�g�̓����蔻����쐬����B
	/// </summary>
	void CreatePhysicsObject();
	/// <summary>
	/// �X�e�[�g�Ǘ��B
	/// </summary>
	void ManageState();
	/// <summary>
	/// �I�[�v���X�e�[�g�̃X�e�[�g�J�ڏ����B
	/// </summary>
	void OpenState();
	/// <summary>
	/// �I�[�v���ҋ@�X�e�[�g�̃X�e�[�g�J�ڏ����B
	/// </summary>
	void OpenIdleState();
	/// <summary>
	/// �N���[�Y�X�e�[�g�̃X�e�[�g�J�ڏ����B
	/// </summary>
	void CloseState();
	/// <summary>
	/// �N���[�Y�ҋ@�X�e�[�g�̃X�e�[�g�J�ڏ����B
	/// </summary>
	void CloseIdleState();
private:
	/// <summary>
	/// ���̐���
	/// </summary>
	/// <param name="number">0->�J�A1->��</param>
	void MakeSound(int number);
	/// <summary>
	/// �e�����𒲂ׂ�
	/// </summary>
	void CheckRate();

	ModelRender					m_modelRender;							//���f�������_�\�B
	Vector3						m_position = Vector3::Zero;				//���W�B
	Quaternion					m_rotation = Quaternion::Identity;		//��]�B
	Vector3						m_scale = Vector3::One;					//�傫���B
	enum EnAnimationClip {					//�A�j���[�V�����B
		enAnimationClip_CloseIdle,			//�ҋ@�B	
		enAnimationClip_OpenIdle,			//�I�[�v���B
		enAnimationClip_Close,				//�N���[�Y�B
		enAnimationClip_Open,				//�I�[�v��
		enAnimationClip_Num,				//�A�j���[�V�����̐��B
	};
	AnimationClip				m_animationClips[enAnimationClip_Num];	//�A�j���[�V�����N���b�v�B
	PhysicsStaticObject			m_physicsStaticObject;					//�t�B�W�N�X�X�^�e�B�b�N�I�u�W�F�N�g�B	
	EnDoorState					m_doorState = enDoorState_CloseIdle;	//�h�A�X�e�[�g�B
	int							m_doorNumber = 0;						//�h�A�̔ԍ��B
	GameCamera*					m_gamecam = nullptr;					//�Q�[���J�����B
	float						m_deg = 0.0f;							//�h�A�̊p�x
	SoundSource*				m_sound = nullptr;						//�J���T�E���h
	float						m_rateByTime = 0.0f;
	Enemy*						m_enemy = nullptr;						//�G�l�~�[
	bool						m_close = true;							//���Ă��邩
};

