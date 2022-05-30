#pragma once
#include "sound/SoundSource.h"
class Hammer;
class Record;
class Gramophone;
class Crowbar;
class GameCamera;
class Item;
class FloorGimmick;


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
	/// <summary>
	/// バールの位置を検索
	/// </summary>
	bool FindLocateOfCrowbar();
	/// <summary>
	/// レコードの位置を検索
	/// </summary>
	bool FindLocateOfRecord();
	/// <summary>
	/// 封鎖する木の板の位置を検索
	/// </summary>
	bool FindLocateOfFloorGimmick();
	/// <summary>
	/// プレイヤーがアイテムを向いているか調べる
	/// </summary>
	/// <param name="diffToItem">アイテムとプレイヤーの座標の差</param> 
	bool CheckPlayerOrientation(Vector3 diffToItem);
	/// <summary>
	/// 表示する文字列を指定
	/// </summary>
	void SpecifyStringToDisplay(std::string item);

private:
	SpriteRender		m_spriteRender;						// Aボタンの画像
	SoundSource*		m_se = nullptr;								// アイテム収拾音
	FontRender			m_itemNameFont;						// アイテム名用フォント
	FontRender			m_supplementFont;					// 補足用フォント
	bool				m_isDraw = false;					// AボタンのUIを表示するか
	Hammer*				m_hammer = nullptr;					// ハンマー
	Record*				m_record = nullptr;					// レコード
	Gramophone*			m_gramophone = nullptr;				// 蓄音機
	Crowbar*			m_crowbar = nullptr;				// バール
	GameCamera*			m_gameCamera = nullptr;				// ゲームカメラ
	FloorGimmick*		m_floorGimmick = nullptr;			// フロア封鎖用ギミック
	Item*				m_item = nullptr;					// アイテムクラス
	Vector3				m_playerForward;					// プレイヤーの正面
	Vector3				m_playerPos;						// プレイヤーの座標
	Vector3				m_diffToItem;						// アイテムとの座標の差
	bool				m_isCrowbarDescript = false;		// バールの説明を表示するか
	bool				m_isHammerDescript = false;			// ハンマーの説明を表示するか
	bool				m_isRecordDescript = false;			// レコードの説明を表示するか
	bool				m_isGramophoneDescript = false;		// 蓄音機の説明を表示するか
	bool				m_isFloorGimmickDescript = false;	// フロア封鎖用ギミックの説明を表示するか
	float				m_timeToDisplay = 0.0f;				// テキストを表示する時間
};

