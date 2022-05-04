#pragma once
class TitleText : public IGameObject
{
public:
	TitleText() {};
	~TitleText() {};
	bool Start();
	void Update();
	void Render(RenderContext& rc);
private:
	ModelRender			m_modelRender;			// モデルレンダー
	Vector3				m_position;				// 座標
	Vector3				m_scale;				// 拡大率
	Quaternion			m_rotation = Quaternion::Identity;				// 回転
};

