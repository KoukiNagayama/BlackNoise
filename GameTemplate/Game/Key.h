#pragma once
class GameCamera;
class GroundFloor;
class Key : public IGameObject
{
public:
	Key() {};
	~Key() {};
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
	/// <summary>
	/// ピースを拾った数
	/// </summary>
	void PickPiece()
	{
		m_havePiece += 1;
	}
private:
	/// <summary>
	/// 鍵を拾ったか
	/// </summary>
	//void PickKey();

	GameCamera* m_gamecam = nullptr;	//ゲームカメラ
	GroundFloor* m_groundfloor;			//一階
	Vector3 m_position;					//座標
	Vector3 m_scale;					//拡大率
	Quaternion m_rotation;				//回転
	ModelRender m_modelRender;			//モデル
	int m_havePiece = 0;				//ピースをいくつ集めたか
};

