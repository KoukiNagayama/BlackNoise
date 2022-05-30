#pragma once
#include "sound/SoundSource.h"

class Player;
class GameCamera;

class Bell : public IGameObject
{
public:
	//�X�e�[�g
	enum EnBellState {
		enBellState_Idle,				//�ҋ@�B
		enBellState_Ring				//����炵���B
	};

public:
	Bell();
	~Bell();
	/// <summary>
	/// �����������B
	/// </summary>
	/// <returns>true</returns>
	bool Start();
	/// <summary>
	/// �X�V�����B
	/// </summary>
	void Update();
	/// <summary>
	/// ���W�B
	/// </summary>
	void Position();
	/// <summary>
	/// �`�揈���B
	/// </summary>
	/// <param name="rc">�����_�[�R���e�L�X�g�B</param>
	void Render(RenderContext& rc);
	/// <summary>
	/// �X�e�[�g�Ǘ��B
	/// </summary>
	void ManageState();
	/// <summary>
	/// �X�e�[�g�J�ڏ����B
	/// </summary>
	void TransitionState();
	/// <summary>
	/// �ҋ@���̏����B
	/// </summary>
	void Idle();
	/// <summary>
	/// �����������̏����B
	/// </summary>
	void Ring();
	/// <summary>
	/// �f�o�b�O�p�����\��
	/// </summary>
	void Font();
	/// <summary>
	/// �������Ă��邩�擾
	/// </summary>
	bool IsRing() const
	{
		return m_isRing;
	}
	/// <summary>
	/// ��x���炵�Ă��Ȃ����擾
	/// </summary>
	bool IsNeverRung() const
	{
		return m_isNeverRung;
	}

private:
	/// <summary>
	/// ���̐���
	/// </summary>
	/// <param name="number">0->�J�A1->��</param>
	void MakeSound();
	/// <summary>
	/// �e�����𒲂ׂ�
	/// </summary>
	void CheckRate();


	//enum EnAnimationClip //�A�j���[�V�����B
	//{
	//	enAnimationClip_Idle,		//�ҋ@�A�j���[�V�����B
	//	enAnimationClip_Ring,		//����炵���Ƃ��̃A�j���[�V�����B
	//	enAnimationClip_Num,		//�A�j���[�V�������B
	//};
	ModelRender m_modelRender;
	Vector3 m_position = Vector3::Zero;						//���W
	Vector3 m_soundPos = Vector3::Zero;						//���ۂɉ�������W
	EnBellState m_bellState = enBellState_Idle;				//�X�e�[�g
	//AnimationClip animationClips[enAnimationClip_Num];		//�A�j���[�V�����N���b�v�B
	SoundSource* m_bellSound = nullptr;						//�x���̉�
	Player* m_player = nullptr;								//�v���C���[
	GameCamera* m_gameCam = nullptr;
	FontRender m_font;
	FontRender m_font1;

	float m_timer = 0.0f;
	float m_rateByTime = 0.0f;
	bool m_isRing = false;
	bool m_isNeverRung = true;
};

