#pragma once
class Player;

class Game : public IGameObject
{
public:
	Game();
	~Game();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
private:
	// アニメーションクリップ
	enum EnAnimationClip {
		enAnimClip_Idle,					// 待機アニメーション
		enAnimClip_Run,						// 走りアニメーション
		enAnimClip_Num,						// アニメーションクリップの数
	};

	Light m_light;
	ModelRender m_modelRender;				// モデルレンダー
	ModelRender m_modelRender2;
	ModelRender m_bgModelRender;		
	Vector3		m_position;					// 座標
	Vector3     m_position2;
	Vector3		m_scale = g_vec3One;		// 拡大率
	Quaternion	m_rotation;					// 回転
	Animation   m_animation;				// アニメーション
	AnimationClip m_animationClipArray[enAnimClip_Num];	// アニメーションクリップ
	Vector3		m_spPosition = {0.0f,100.0f,-10.0f};
	Quaternion	m_qRotY;
	Quaternion  m_qRotX;
	SpriteRender m_spriteRender1;
	SpriteRender m_spriteRender2;
};

