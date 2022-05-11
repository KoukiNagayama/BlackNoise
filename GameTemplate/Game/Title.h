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
	TitleCamera*			m_tCamera = nullptr;			// タイトルカメラ
	TitleText*				m_tText = nullptr;				// タイトルの文字
	BackGround*				m_backGround = nullptr;			// 背景
	Gramophone*				m_gramophone = nullptr;			// 蓄音機
	FontRender				m_fontRender;					// フォントレンダー
	LevelRender				m_levelRender1;					// レベルレンダー
	LevelRender				m_levelRender2;
};

