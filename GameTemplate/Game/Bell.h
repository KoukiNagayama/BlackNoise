#pragma once
#include "sound/SoundSource.h"

class Player;
class GameCamera;

class Bell : public IGameObject
{
public:
	//ステート
	enum EnBellState {
		enBellState_Idle,				//待機。
		enBellState_Ring				//音を鳴らした。
	};

public:
	Bell();
	~Bell();
	/// <summary>
	/// 初期化処理。
	/// </summary>
	/// <returns>true</returns>
	bool Start();
	/// <summary>
	/// 更新処理。
	/// </summary>
	void Update();
	/// <summary>
	/// 座標。
	/// </summary>
	void Position();
	/// <summary>
	/// 描画処理。
	/// </summary>
	/// <param name="rc">レンダーコンテキスト。</param>
	void Render(RenderContext& rc);
	/// <summary>
	/// ステート管理。
	/// </summary>
	void ManageState();
	/// <summary>
	/// ステート遷移処理。
	/// </summary>
	void TransitionState();
	/// <summary>
	/// 待機時の処理。
	/// </summary>
	void Idle();
	/// <summary>
	/// 音が鳴った時の処理。
	/// </summary>
	void Ring();
	/// <summary>
	/// アニメーション再生。
	/// </summary>
	void PlayAnimation();

	void Font();

private:
	enum EnAnimationClip //アニメーション。
	{
		enAnimationClip_Idle,		//待機アニメーション。
		enAnimationClip_Ring,		//音を鳴らしたときのアニメーション。
		enAnimationClip_Num,		//アニメーション数。
	};
	ModelRender m_modelRender;
	Vector3 m_position;
	Vector3 m_soundPos;
	EnBellState m_bellState = enBellState_Idle;
	AnimationClip animationClips[enAnimationClip_Num];		//アニメーションクリップ。
	SoundSource* m_bellSound;
	Player* m_player = nullptr;
	GameCamera* m_gameCam = nullptr;
	FontRender m_font;
	FontRender m_font1;

	float m_timer = 0.0f;
	float rate = 0.0f;
	float beforeRate;
	bool m_isRing = false;
};

