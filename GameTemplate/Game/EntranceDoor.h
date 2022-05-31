#pragma once
class GroundFloor;
class EntranceDoor : public IGameObject
{
public:
	EntranceDoor() {};
	~EntranceDoor() {};

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
	/// 座標を設定
	/// </summary>
	/// <param name="position">座標</param>
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}
	/// <summary>
	/// 拡大率を設定
	/// </summary>
	/// <param name="scale">拡大率</param>
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
	}
	/// <summary>
	/// 回転を設定
	/// </summary>
	/// <param name="rotation">回転</param>
	void SetRotation(const Quaternion& rotation)
	{
		m_rotation = rotation;
	}
private:
	void OpenDoor();
private:
	// アニメーションクリップ
	enum EnAnimationClip {	
		enAnimationClip_Idle,
		enAnimationClip_Open,			// 開く
		enAnimationClip_Num				// 個数
	};
	ModelRender				m_modelRender;						// モデルレンダー
	Vector3					m_position;							// 座標
	Vector3					m_scale;							// 拡大率
	Quaternion				m_rotation;							// 回転
	GroundFloor*			m_groundFloor;
	AnimationClip			m_animationClips[enAnimationClip_Num];
	bool					m_isOpened = false;
};

