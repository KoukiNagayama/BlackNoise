#pragma once
class Enemy2;
class GameStart;
class GroundFloor;
class Title;
class GameOver : public IGameObject
{
public:
	GameOver() {};
	~GameOver();
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
private:
	/// <summary>
	/// オブジェクトを削除
	/// </summary>
	void DeleteObject();
	/// <summary>
	/// フェードイン
	/// </summary>
	void FadeIn();
	/// <summary>
	/// フェードアウト
	/// </summary>
	void FadeOut();
private:
	Enemy2*					m_enemy;						// エネミー
	SpriteRender			m_gameOverSprite;					// ゲームオーバー時のスプライト
	float					m_currentAlpha = 0.0f;			// 現在のα値
	float					m_timerToFadeOut = 0.0f;
	Vector3					m_gameOverPosition;
	Vector3					m_gameOverScale;
	GameStart*				m_gameStart = nullptr;
	GroundFloor*			m_groundFloor = nullptr;
	Title*					m_title = nullptr;
	bool					m_isFadeOut = false;
};

