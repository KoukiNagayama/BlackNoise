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
		enTitleState_Idle,				// 黒画面(待機画面)
		enTitleState_PlayPiano,			// ピアノ演奏中
		enTitleState_PlayGramophone		// 蓄音機演奏中
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
	/// <summary>
	/// ピアノを演奏中か
	/// </summary>
	const bool IsPlayPiano() const
	{
		return m_titleState == enTitleState_PlayPiano;
	}
	/// <summary>
	/// 蓄音機の演奏開始
	/// </summary>
	void StartPlayGramophone()
	{
		m_titleState = enTitleState_PlayGramophone;
	}
private:
	TitleCamera*			m_titleCamera = nullptr;		// タイトルカメラ
	TitleText*				m_titleText = nullptr;			// タイトルの文字
	BackGround*				m_backGround = nullptr;			// 背景
	TitleGramophone*		m_titleGramophone = nullptr;	// 蓄音機
	TitleSprite*			m_titleSprite;					// タイトルの画像
	TitlePiano*				m_titlePiano;
	FontRender				m_fontRender;					// フォントレンダー
	LevelRender				m_levelRender1;					// レベルレンダー
	LevelRender				m_levelRender2;
	enTitleState			m_titleState = enTitleState_Idle;


};

