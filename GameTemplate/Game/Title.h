#pragma once
class TitleCamera;
class TitleText;
class BackGround;
class Gramophone;

class Title : public IGameObject
{
public:
	Title() {};
	~Title() {};

	bool Start();
	void Update();
	void Render(RenderContext& rc);
private:
	TitleCamera*			m_tCamera;				// �^�C�g���J����
	TitleText*				m_tText;				// �^�C�g���̕���
	BackGround*				m_backGround;			// �w�i
	Gramophone*				m_gramophone;			// �~���@
	FontRender				m_fontRender;			// �t�H���g�����_�[
};

