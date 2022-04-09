#pragma once
class Toy : public IGameObject
{
public:
	Toy();
	~Toy();
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

