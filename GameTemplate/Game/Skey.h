#pragma once
class Skey : public IGameObject
{
public:
	Skey();
	~Skey();
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}
	void Render(RenderContext& rc);
	void Update();
private:
	ModelRender m_modelRender;
	Vector3 m_position;
};

