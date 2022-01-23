#pragma once

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
	ModelRender m_bgRender;		
	Vector3		m_position;					// 座標
	Vector3		m_scale = g_vec3One;		// 拡大率
	Quaternion	m_rotation;					// 回転
	Animation   m_animation;				// アニメーション
	AnimationClip m_animationClipArray[enAnimClip_Num];	// アニメーションクリップ
	Vector3		m_ptPosition = {0.0f,50.0f,50.0f};
};

