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
	SpriteRender			m_bellSprite;						// ベルのスプライト
	SpriteRender			m_textSprite;						// テキストのスプライト
	SpriteRender			m_bButtonSprite;					// Bボタンのスプライト
	SpriteRender			m_pressSprite;						// PRESSのスプライト
	Vector3					m_bellPos;							// ベルの座標
	Vector3					m_textPos;							// テキストの座標
	Vector3					m_bButtonPos;						// Bボタンの座標
	Vector3					m_pressPos;							// PRESSの座標
	Vector3					m_bellScale;						// ベルの拡大率
	Vector3					m_textScale;						// テキストの拡大率
	Vector3					m_bButtonScale;						// Bボタンの拡大率
	Vector3					m_pressScale;						// PRESSの拡大率
	Title*					m_title = nullptr;					// タイトル
	float					m_currentAlpha = 1.0f;				// 現在のα値
};

