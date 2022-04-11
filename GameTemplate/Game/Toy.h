#pragma once
class GameCamera;
class Toy : public IGameObject
{
public:
	enum enToyState {
		enToyState_usual,		// 通常時
		enToyState_collapse		// 崩壊時
	};
	Toy();
	~Toy();
	/// <summary>
	/// 座標を設定
	/// </summary>
	/// <param name="position">座標</param>
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}
	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="rc">レンダリングコンテキスト</param>
	void Render(RenderContext& rc);
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();
private:
	ModelRender			m_modelRender;					// モデルレンダー
	Vector3				m_position;						// 座標
	GameCamera*			m_gameCamera;					// ゲームカメラ
	Vector3				m_gameCameraPos;				// ゲームカメラの座標
	enToyState			m_toyState = enToyState_usual;
};

