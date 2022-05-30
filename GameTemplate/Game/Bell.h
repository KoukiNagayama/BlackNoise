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
	/// デバッグ用文字表示
	/// </summary>
	void Font();
	/// <summary>
	/// 音が鳴っているか取得
	/// </summary>
	bool IsRing() const
	{
		return m_isRing;
	}
	/// <summary>
	/// 一度も鳴らしていないか取得
	/// </summary>
	bool IsNeverRung() const
	{
		return m_isNeverRung;
	}

private:
	/// <summary>
	/// 音の生成
	/// </summary>
	/// <param name="number">0->開、1->閉</param>
	void MakeSound();
	/// <summary>
	/// 影響率を調べる
	/// </summary>
	void CheckRate();


	//enum EnAnimationClip //アニメーション。
	//{
	//	enAnimationClip_Idle,		//待機アニメーション。
	//	enAnimationClip_Ring,		//音を鳴らしたときのアニメーション。
	//	enAnimationClip_Num,		//アニメーション数。
	//};
	ModelRender m_modelRender;
	Vector3 m_position = Vector3::Zero;						//座標
	Vector3 m_soundPos = Vector3::Zero;						//実際に音が鳴る座標
	EnBellState m_bellState = enBellState_Idle;				//ステート
	//AnimationClip animationClips[enAnimationClip_Num];		//アニメーションクリップ。
	SoundSource* m_bellSound = nullptr;						//ベルの音
	Player* m_player = nullptr;								//プレイヤー
	GameCamera* m_gameCam = nullptr;
	FontRender m_font;
	FontRender m_font1;

	float m_timer = 0.0f;
	float m_rateByTime = 0.0f;
	bool m_isRing = false;
	bool m_isNeverRung = true;
};

