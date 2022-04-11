#pragma once
class GameCamera;
class Toy : public IGameObject
{
public:
	enum enToyState {
		enToyState_usual,
		enToyState_collapse
	};
	Toy();
	~Toy();
	/// <summary>
	/// 初期化関数。
	/// </summary>
	/// <returns>true</returns>
	bool Start();
	/// <summary>
	/// 座標を設定
	/// </summary>
	/// <param name="position">座標</param>
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}
	void Render(RenderContext& rc);
	void Update();
private:
	ModelRender			m_modelRender;		// モデルレンダー
	Vector3				m_position;			// 座標
	GameCamera*			m_gameCamera;		// ゲームカメラ
	Vector3				m_gameCameraPos;	// ゲームカメラの座標
	enToyState			m_toyState = enToyState_usual;
};

