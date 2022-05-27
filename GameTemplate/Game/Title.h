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
		enTitleState_Idle,				// 黒画面(待機画面)
		enTitleState_RingBell,			// ベルにより景色が可視化
		enTitleState_FadeOut			// 輪郭線が徐々に消えていく
	};

	Title() {};
	~Title();
	/// <summary>
	/// 初期化処理
	/// </summary>
	bool Start();
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();
	/// <summary>
	/// 描画処理
	/// </summary>
	void Render(RenderContext& rc);
	/// <summary>
	/// 現在のステートが待機中か
	/// </summary>
	/// <returns>待機中ならばtrue そうでなければfalse</returns>
	bool IsIdleStateNow() const
	{
		return m_titleState == enTitleState_Idle;
	}
private:
	/// <summary>
	/// ステート管理
	/// </summary>
	void ManageState();
	/// <summary>
	/// 待機中のステート遷移
	/// </summary>
	void ProcessIdleStateTransition();
	/// <summary>
	/// ベルを鳴らした状態のステート遷移
	/// </summary>
	void ProcessRingBellStateTransition();
	/// <summary>
	/// フェードアウトが始まってからゲームが始まるまでの処理
	/// </summary>
	void ProcessFromFadeOutStateToStartingGame();
	/// <summary>
	/// 音を鳴らす
	/// </summary>
	void MakeSound();
	/// <summary>
	/// 影響率を変化させる
	/// </summary>
	void ChangeRate();
	/// <summary>
	/// 影響率を上げる
	/// </summary>
	void IncreaseRate();
	/// <summary>
	/// 影響率を下げる
	/// </summary>
	void ReduceRate();
private:
	TitleCamera*			m_titleCamera = nullptr;				// タイトルカメラ
	TitleText*				m_titleText = nullptr;					// タイトルの文字
	BackGround*				m_backGround = nullptr;					// 背景
	TitleSprite*			m_titleSprite;							// タイトルの画像
	FontRender				m_fontRender;							// フォントレンダー
	LevelRender				m_levelRender1;							// レベルレンダー
	enTitleState			m_titleState = enTitleState_Idle;		// タイトルのステート
	Vector3					m_position;								// ベルの座標
	SoundSource*			m_bellSound = nullptr;					// 音源クラス
	float					m_rateByTime = 0.0f;					// 時間による影響率
	int						m_isRateChange = 0;						// 影響率の変化
	bool					m_isEndStanging = false;				// タイトルの演出が終わったか
	GameStart*				m_gameStart = nullptr;					// ゲームクラス




};

