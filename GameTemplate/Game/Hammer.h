#pragma once

class GameCamera;
class BrokenDoor;

class Hammer:public IGameObject
{
public:
	Hammer() {};
	~Hammer() {};
	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <returns>true</returns>
	bool Start();
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();
	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="rc">レンダーコンテキスト</param>
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
	/// 回転を設定
	/// </summary>
	/// <param name="rot">クォータニオン</param>
	void SetRotation(Quaternion& rot)
	{
		m_rotation = rot;
	}

private:
	Vector3			m_disToPlayer;			//プレイヤーとの距離
	Vector3			m_position;				//座標
	Quaternion		m_rotation;				//回転
	ModelRender		m_modelRender;			//モデル
	GameCamera*		m_gamecam;				//ゲームカメラ
	BrokenDoor*		m_b_door;				//ブロークンドア
};

