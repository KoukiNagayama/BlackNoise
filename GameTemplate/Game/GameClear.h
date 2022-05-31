#pragma once
class GroundFloor;
class Title;

class GameClear : public IGameObject
{
public:
	GameClear() {};
	~GameClear();
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
	/// ゲームクリアしているか
	/// </summary>
	/// <returns></returns>
	bool IsGameClear() const
	{
		return m_isGameClear;
	}
private:
	SpriteRender				m_gameClearSprite;					// ゲームクリア時のスプライト
	GroundFloor*				m_groundFloor = nullptr;
	float						m_timerToDrawSprite;
	bool						m_isDraw = false;
	bool						m_isGameClear = true;
	Title* m_title = nullptr;
};

