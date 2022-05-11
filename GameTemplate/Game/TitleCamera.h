#pragma once
class TitleText;
class TitleCamera : public IGameObject
{
public:
	TitleCamera() {};
	~TitleCamera() {};

	bool Start();
	void Update();
	/// <summary>
	/// 視点を設定
	/// </summary>
	/// <param name="position">座標</param>
	void SetPosition(const Vector3& position)
	{
		m_cameraPos = position;
	}
private:
	Vector3				m_cameraPos;			// 座標(視点)	
	Vector3				m_toCameraPos;			// 注視点から視点に向かうベクトル
	TitleText*			m_tText;
};

