#pragma once
#include "sound/SoundSource.h"
class Title;
class TitlePiano : public IGameObject
{
public:
	TitlePiano() {};
	~TitlePiano() {};

	/// <summary>
	/// 初期化処理
	/// </summary>
	bool Start();
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();
	/// <summary>
	/// 座標を設定
	/// </summary>
	/// <param name="position">座標</param>
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}
private:
	/// <summary>
	/// 音を鳴らす
	/// </summary>
	void MakeSound();
	/// <summary>
	/// 影響率を変える
	/// </summary>
	void ChangeRate();

private:
	Vector3			m_position;				// 座標
	SoundSource*	m_soundSource;			// 音源
	float			m_rateByTime;			// 時間による影響率
	int				m_count;				// ピアノが鳴った回数
	Title*			m_title;				// タイトル
};

