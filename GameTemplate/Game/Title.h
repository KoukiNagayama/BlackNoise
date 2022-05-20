#pragma once
#include "LevelRender.h"

class TitleCamera;
class TitleText;
class BackGround;
class TitleGramophone;
class TitleSprite;
class TitlePiano;

class Title : public IGameObject
{
public:
	enum enTitleState {
		enTitleState_Idle,				// �����(�ҋ@���)
		enTitleState_PlayPiano,			// �s�A�m���t��
		enTitleState_PlayGramophone		// �~���@���t��
	};

	Title() {};
	~Title();

	bool Start();
	void Update();
	void Render(RenderContext& rc);
	/// <summary>
	/// �^�C�g���̃X�e�[�g���擾
	/// </summary>
	/// <returns>�^�C�g���X�e�[�g</returns>
	int GetTitleState() const
	{
		return m_titleState;
	}
	/// <summary>
	/// �s�A�m�����t����
	/// </summary>
	const bool IsPlayPiano() const
	{
		return m_titleState == enTitleState_PlayPiano;
	}
	/// <summary>
	/// �~���@�̉��t�J�n
	/// </summary>
	void StartPlayGramophone()
	{
		m_titleState = enTitleState_PlayGramophone;
	}
private:
	TitleCamera*			m_titleCamera = nullptr;		// �^�C�g���J����
	TitleText*				m_titleText = nullptr;			// �^�C�g���̕���
	BackGround*				m_backGround = nullptr;			// �w�i
	TitleGramophone*		m_titleGramophone = nullptr;	// �~���@
	TitleSprite*			m_titleSprite;					// �^�C�g���̉摜
	TitlePiano*				m_titlePiano;
	FontRender				m_fontRender;					// �t�H���g�����_�[
	LevelRender				m_levelRender1;					// ���x�������_�[
	LevelRender				m_levelRender2;
	enTitleState			m_titleState = enTitleState_Idle;


};

