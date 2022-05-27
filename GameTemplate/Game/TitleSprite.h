#pragma once
class Title;
class TitleSprite : public IGameObject
{
public:
	TitleSprite() {};
	~TitleSprite() {};
	bool Start();
	void Update();
	void Render(RenderContext& rc);
private:
	SpriteRender			m_spriteRender1;
	SpriteRender			m_spriteRender2;
	Vector3					m_spritePos1;
	Vector3					m_spritePos2;
	Vector3					m_spriteScale;
	Title*					m_title;
};

