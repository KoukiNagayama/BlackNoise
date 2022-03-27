#pragma once
class Player;
class GameCamera;
#include "sound/SoundSource.h"

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

	ModelRender m_modelRender;				// モデルレンダー
	ModelRender m_modelRender2;
	ModelRender m_bgModelRender;		
	Vector3		m_position;					// 座標
	Vector3     m_position2;
	Vector3		m_scale = g_vec3One;		// 拡大率
	Quaternion	m_rotation;					// 回転
	Animation   m_animation;				// アニメーション
	AnimationClip m_animationClipArray[enAnimClip_Num];	// アニメーションクリップ

	Player*		m_player;
	GameCamera* m_gamecam;
	SoundSource* m_sound1;
	SoundSource* m_sound2 = nullptr;
	float rateByTime;
};

