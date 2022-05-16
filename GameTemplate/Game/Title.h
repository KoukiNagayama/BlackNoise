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
	/// タイトルのステートを取得
	/// </summary>
	/// <returns>タイトルステート</returns>
	int GetTitleState() const
	{
		return m_titleState;
	}
private:
	TitleCamera*			m_tCamera = nullptr;			// タイトルカメラ
	TitleText*				m_tText = nullptr;				// タイトルの文字
	BackGround*				m_backGround = nullptr;			// 背景
	TitleGramophone*		m_tGramophone = nullptr;		// 蓄音機
	FontRender				m_fontRender;					// フォントレンダー
	LevelRender				m_levelRender1;					// レベルレンダー
	LevelRender				m_levelRender2;
	enTitleState			m_titleState = enTitleState_Idle;

};

