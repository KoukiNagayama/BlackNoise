#pragma once
class Bell;
class BellSprite : public IGameObject
{
public:
	BellSprite() {};
	~BellSprite() {};

	bool Start();
	void Update();
	void Render(RenderContext& rc);
private:
	SpriteRender	m_bellSprite;
	Vector3			m_bellSpritePos;
	Vector3			m_bellSpriteSca;
	Bell*			m_bell = nullptr;
	float			m_currentAlpha = 0.0f;
};

