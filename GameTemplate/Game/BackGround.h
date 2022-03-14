#pragma once
class BackGround : public IGameObject
{
public:
	BackGround();
	~BackGround();
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}
	void Render(RenderContext& rc);

private:
	ModelRender m_modelRender;
	PhysicsStaticObject m_physicsStaticObject;
	Vector3 m_position = Vector3::Zero;
};
