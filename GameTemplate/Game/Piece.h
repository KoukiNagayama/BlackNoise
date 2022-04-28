#pragma once
class GameCamera;
class Key;
class Piece:public IGameObject
{
public:
	Piece() {};
	~Piece() {};
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	/// <summary>
	/// 座標の設定
	/// </summary>
	/// <param name="pos">座標</param>
	void SetPosition(Vector3& pos)
	{
		m_position = pos;
	}
	/// <summary>
	/// 拡大率の設定
	/// </summary>
	/// <param name="scale">拡大率</param>
	void SetScale(Vector3& scale)
	{
		m_scale = scale;
	}
	/// <summary>
	/// 回転の設定
	/// </summary>
	/// <param name="rot">クォータニオン</param>
	void SetRotation(Quaternion& rot)
	{
		m_rotation = rot;
	}

private:
	Vector3 m_position; //座標
	Vector3 m_scale; //拡大率
	Quaternion m_rotation; //回転
	GameCamera* m_gamecam = nullptr; //ゲームカメラ
	Key* m_key = nullptr; //鍵
	ModelRender m_modelRender; //モデル
};

