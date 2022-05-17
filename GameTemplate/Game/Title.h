#pragma once
#include "LevelRender.h"

class TitleCamera;
class TitleText;
class BackGround;
class TitleGramophone;

class Title : public IGameObject
{
public:
	enum enTitleState {
		enTitleState_Idle,
		enTitleState_Play
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
private:
	TitleCamera*			m_tCamera = nullptr;			// �^�C�g���J����
	TitleText*				m_tText = nullptr;				// �^�C�g���̕���
	BackGround*				m_backGround = nullptr;			// �w�i
	TitleGramophone*		m_tGramophone = nullptr;		// �~���@
	FontRender				m_fontRender;					// �t�H���g�����_�[
	LevelRender				m_levelRender1;					// ���x�������_�[
	LevelRender				m_levelRender2;
	enTitleState			m_titleState = enTitleState_Idle;

};

