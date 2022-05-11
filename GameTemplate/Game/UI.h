#pragma once
#include "sound/SoundSource.h"
class UI:public IGameObject
{
public:
	UI() {};
	~UI();
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
	/// Aボタンを表示・非表示にする
	/// </summary>
	/// <param name="isDraw">表示->1</param>
	void SetUIRender(bool isDraw)
	{
		m_isDraw_A = isDraw;
	}

private:
	//収拾音の生成
	void MakeSound();

	SpriteRender m_spriteRender;	//Aボタンの画像
	SoundSource* m_se;				//アイテム収拾音
	FontRender m_recordFont;		//レコードを拾った
	FontRender m_hammerFont;		//ハンマーを拾った
	FontRender m_crowbarFont;		//バールを拾った
	bool m_isDraw_A = false;		//AボタンのUIを表示するか
};

