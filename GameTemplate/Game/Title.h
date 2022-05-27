#pragma once
#include "LevelRender.h"
#include "sound/SoundSource.h"

class TitleCamera;
class TitleText;
class BackGround;
class TitleSprite;
class GameStart;

class Title : public IGameObject
{
public:
	enum enTitleState {
		enTitleState_Idle,				// �����(�ҋ@���)
		enTitleState_RingBell,			// �x���ɂ��i�F������
		enTitleState_FadeOut			// �֊s�������X�ɏ����Ă���
	};

	Title() {};
	~Title();
	/// <summary>
	/// ����������
	/// </summary>
	bool Start();
	/// <summary>
	/// �X�V����
	/// </summary>
	void Update();
	/// <summary>
	/// �`�揈��
	/// </summary>
	void Render(RenderContext& rc);
	/// <summary>
	/// ���݂̃X�e�[�g���ҋ@����
	/// </summary>
	/// <returns>�ҋ@���Ȃ��true �����łȂ����false</returns>
	bool IsIdleStateNow() const
	{
		return m_titleState == enTitleState_Idle;
	}
private:
	/// <summary>
	/// �X�e�[�g�Ǘ�
	/// </summary>
	void ManageState();
	/// <summary>
	/// �ҋ@���̃X�e�[�g�J��
	/// </summary>
	void ProcessIdleStateTransition();
	/// <summary>
	/// �x����炵����Ԃ̃X�e�[�g�J��
	/// </summary>
	void ProcessRingBellStateTransition();
	/// <summary>
	/// �t�F�[�h�A�E�g���n�܂��Ă���Q�[�����n�܂�܂ł̏���
	/// </summary>
	void ProcessFromFadeOutStateToStartingGame();
	/// <summary>
	/// ����炷
	/// </summary>
	void MakeSound();
	/// <summary>
	/// �e������ω�������
	/// </summary>
	void ChangeRate();
	/// <summary>
	/// �e�������グ��
	/// </summary>
	void IncreaseRate();
	/// <summary>
	/// �e������������
	/// </summary>
	void ReduceRate();
private:
	TitleCamera*			m_titleCamera = nullptr;				// �^�C�g���J����
	TitleText*				m_titleText = nullptr;					// �^�C�g���̕���
	BackGround*				m_backGround = nullptr;					// �w�i
	TitleSprite*			m_titleSprite;							// �^�C�g���̉摜
	FontRender				m_fontRender;							// �t�H���g�����_�[
	LevelRender				m_levelRender1;							// ���x�������_�[
	enTitleState			m_titleState = enTitleState_Idle;		// �^�C�g���̃X�e�[�g
	Vector3					m_position;								// �x���̍��W
	SoundSource*			m_bellSound = nullptr;					// �����N���X
	float					m_rateByTime = 0.0f;					// ���Ԃɂ��e����
	int						m_isRateChange = 0;						// �e�����̕ω�
	bool					m_isEndStanging = false;				// �^�C�g���̉��o���I�������
	GameStart*				m_gameStart = nullptr;					// �Q�[���N���X




};

