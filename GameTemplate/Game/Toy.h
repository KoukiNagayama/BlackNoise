#pragma once
#include "sound/SoundSource.h"

class GameCamera;
class Toy : public IGameObject
{
public:
	enum enToyState {
		enToyState_usual,		// 通常時
		enToyState_collapse		// 崩壊時
	};
	Toy() {};
	~Toy();
	/// <summary>
	/// 初期化関数。
	/// </summary>
	/// <returns>true</returns>
	bool Start();
	/// <summary>
	/// 座標を設定
	/// </summary>
	/// <param name="position">座標</param>
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}
	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="rc">レンダリングコンテキスト</param>
	void Render(RenderContext& rc);
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();
	bool IsSound() const
	{
		return m_isSound;
	}
private:
	/// <summary>
	/// 音を鳴らす
	/// </summary>
	void MakeSound();
	/// <summary>
	/// 影響率を調べる
	/// </summary>
	void CheckRate();
private:
	ModelRender			m_modelRender;					// モデルレンダー
	Vector3				m_position;						// 座標
	GameCamera*			m_gameCamera;					// ゲームカメラ
	Vector3				m_gameCameraPos;				// ゲームカメラの座標
	enToyState			m_toyState = enToyState_usual;
	float				m_rateByTime;
	SoundSource*		m_sound = nullptr;
	bool				m_isSound = false;

};

