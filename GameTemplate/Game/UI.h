#pragma once
#include "sound/SoundSource.h"
class UI:public IGameObject
{
public:
	UI() {};
	~UI() {};
	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <returns>true</returns>
	bool Start();
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();
	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="rc">レンダーコンテキスト</param>
	void Render(RenderContext& rc);
	/// <summary>
	/// ボタンの表示、非表示
	/// </summary>
	/// <param name="isDraw">0->非表示,1->表示</param>
	/// <param name="number">0->Aボタン,1->Xボタン</param>
	void SetButtonRender(bool isDraw,int number)
	{
		switch (number)
		{
		case 0:
			m_spriteRender.Init("Assets/sprite/Abutton.dds", 128, 128);
			break;
		case 1:
			m_spriteRender.Init("Assets/sprite/Xbutton.dds", 128, 128);
			break;
		default:
			break;
		}
		m_isDraw = isDraw;
	}

	//収拾音の生成
	void MakeGetSound();

private:

	SpriteRender m_spriteRender;	//Aボタンの画像
	SoundSource* m_se;				//アイテム収拾音
	FontRender m_recordFont;		//レコードを拾った
	FontRender m_hammerFont;		//ハンマーを拾った
	FontRender m_crowbarFont;		//バールを拾った
	bool m_isDraw = false;		//AボタンのUIを表示するか
};

