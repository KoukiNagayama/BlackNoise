#pragma once
#include "sound/SoundSource.h"
#include "sound/SoundEngine.h"

class GameCamera;
class Bell;
class UIandSE : public IGameObject
{
public:
	UIandSE() {};
	~UIandSE();
	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <returns>true</returns>
	bool Start();
	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="rc">レンダーコンテキスト</param>
	void Render(RenderContext& rc);

	void DrawSprite();
	
private:
	
	
	Vector3 m_disToPlayer;					//プレイヤーとの距離
	SpriteRender m_spriteRender;			//画像
	FontRender m_fontRender;				//文字
	SoundSource* m_sound1;					//効果音

	GameCamera* m_gamecam = nullptr;		//ゲームカメラ
	Bell* m_bell = nullptr;					//ベル

	bool m_drawSprite = false;
};

