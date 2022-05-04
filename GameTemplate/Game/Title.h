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
	TitleCamera*			m_tCamera;				// タイトルカメラ
	TitleText*				m_tText;				// タイトルの文字
	BackGround*				m_backGround;			// 背景
	Gramophone*				m_gramophone;			// 蓄音機
	FontRender				m_fontRender;			// フォントレンダー
};

