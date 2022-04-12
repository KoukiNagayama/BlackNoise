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
	/// �A�j���[�V�����Đ��B
	/// </summary>
	void PlayAnimation();

	void Font();

private:
	enum EnAnimationClip //�A�j���[�V�����B
	{
		enAnimationClip_Idle,		//�ҋ@�A�j���[�V�����B
		enAnimationClip_Ring,		//����炵���Ƃ��̃A�j���[�V�����B
		enAnimationClip_Num,		//�A�j���[�V�������B
	};
	ModelRender m_modelRender;
	Vector3 m_position;
	Vector3 m_soundPos;
	EnBellState m_bellState = enBellState_Idle;
	AnimationClip animationClips[enAnimationClip_Num];		//�A�j���[�V�����N���b�v�B
	SoundSource* m_bellSound;
	Player* m_player = nullptr;
	GameCamera* m_gameCam = nullptr;
	FontRender m_font;
	FontRender m_font1;

	float m_timer = 0.0f;
	float rate = 0.0f;
	float beforeRate;
	bool m_isRing = false;
};

