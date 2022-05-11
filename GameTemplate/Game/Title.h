#pragma once
#include "LevelRender.h"

class TitleCamera;
class TitleText;
class BackGround;
class Gramophone;


class Title : public IGameObject
{
public:
	Title() {};
	~Title();

	bool Start();
	void Update();
	void Render(RenderContext& rc);
private:
	TitleCamera*			m_tCamera = nullptr;			// �^�C�g���J����
	TitleText*				m_tText = nullptr;				// �^�C�g���̕���
	BackGround*				m_backGround = nullptr;			// �w�i
	Gramophone*				m_gramophone = nullptr;			// �~���@
	FontRender				m_fontRender;					// �t�H���g�����_�[
	LevelRender				m_levelRender1;					// ���x�������_�[
	LevelRender				m_levelRender2;
};

