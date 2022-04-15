#pragma once
#include "sound/SoundSource.h"
class GameCamera;
class Gramophone : public IGameObject
{
public:

	enum enGramophoneState
	{
		enGramophoneState_Idle,
		enGramophoneState_Play
	};

	Gramophone() {};
	~Gramophone() {};
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	/// <summary>
	/// 座標を設定
	/// </summary>
	/// <param name="position">座標</param>
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}
	/// <summary>
	/// 拡大率を設定
	/// </summary>
	/// <param name="scale">拡大率</param>
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
	}
	/// <summary>
	/// 回転を設定
	/// </summary>
	/// <param name="rotation">回転</param>
	void SetRotation(const Quaternion& rotation)
	{
		m_rotation = rotation;
	}
	const int GetState() const
	{

	}
private:
	/// <summary>
	/// 音を鳴らす
	/// </summary>
	void MakeSound();
	/// <summary>
	/// プレイヤーと音源の距離により音の大きさを調整する
	/// </summary>
	float SoundLevelByDistance();
	/// <summary>
	/// 音量調整
	/// </summary>
	void VolumeControl();
	/// <summary>
	/// ステート管理
	/// </summary>
	void ManegeState();

	void IdleState();
	void PlayState();

	void Font();
private:
	enGramophoneState		m_gramophoneState = enGramophoneState_Idle;	// ステート
	Vector3					m_position;									// 座標
	Vector3					m_scale;									// 拡大率
	Quaternion				m_rotation = Quaternion::Identity;			// 回転
	ModelRender				m_modelRender;								// モデルレンダー
	float					m_rateByTime1 = 1.00f;						// 時間による影響率
	SoundSource*			m_soundSource = nullptr;					// 音源
	GameCamera*				m_gameCamera;								// ゲームカメラ
	FontRender				m_font;
};

