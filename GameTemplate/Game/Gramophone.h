#pragma once
#include "sound/SoundSource.h"

class Item;
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
	~Gramophone() ;
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
	/// <summary>
	/// オブジェクトの番号を設定
	/// </summary>
	/// <param name="number">番号</param>
	void SetNumber(const int& number)
	{
		m_number = number;
	}

private:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="num">オブジェクトの番号</param>
	void Init(int num);
	/// <summary>
	/// 音を鳴らす
	/// </summary>
	void MakeSound();
	/// <summary>
	/// プレイヤーと音源の距離により音の大きさを調整する
	/// </summary>
	/// <param name="range">音源の影響範囲</param>
	float SoundLevelByDistance(float range);
	/// <summary>
	/// 音量調整
	/// </summary>
	void VolumeControl();
	/// <summary>
	/// ステート管理
	/// </summary>
	void ManegeState();
	/// <summary>
	/// 待機時の動作
	/// </summary>
	void IdleState();
	/// <summary>
	/// 演奏中の動作
	/// </summary>
	void PlayState();
	/// <summary>
	/// 影響率
	/// </summary>
	void ChangeRate(int num);

private:
	enGramophoneState		m_gramophoneState = enGramophoneState_Idle;	// ステート
	Vector3					m_position;									// 座標
	Vector3					m_scale;									// 拡大率
	Quaternion				m_rotation;									// 回転
	ModelRender				m_modelRender;								// モデルレンダー
	float					m_rateByTime;								// 時間による影響率
	SoundSource*			m_soundSource1 = nullptr;					// 音源1
	SoundSource*			m_soundSource2 = nullptr;					// 音源2
	SoundSource*			m_soundSource3 = nullptr;					// 音源3
	GameCamera*				m_gameCamera;								// ゲームカメラ
	int						m_number;									// オブジェクトの番号
	Item*					m_item;
	int						m_recordNum;
};

