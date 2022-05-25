#include "stdafx.h"
#include "UI.h"
#include "sound/SoundEngine.h"
#include "GameCamera.h"
#include "Crowbar.h"
#include "Gramophone.h"
#include "Hammer.h"
#include "Record.h"
#include "FloorGimmick.h"
#include "Item.h"


namespace
{
	const Vector3	ITEM_NAME_POSITION = Vector3(200.0f, -80.0f, 0.0f);		// アイテム名を表示する座標
	const Vector3	SUPPLEMENT_POSITION = Vector3(200.0f, -120.0f, 0.0f);	// 補足を表示する座標
	const float		DISTANCE_TO_ITEM = 150.0f;								// アイテムとの距離
	const float		DISTANCE_TO_FLOORGIMMICK = 350.0f;						// フロアを封鎖するギミックとの距離
	const float		VECTOR_CONSISTENCY = 0.7f;								// ベクトルが一致しているか比較する値		
	const float		TIME_TO_DISPLAY = 3.0f;									// アイテムに対する反応のテキストを表示させる時間
}

bool UI::Start()
{

	m_gameCamera = FindGO<GameCamera>("gamecamera");

	m_crowbar = FindGO<Crowbar>("crowbar");

	return true;
}

void UI::Update()
{

	// バールを検索
	m_crowbar = FindGO<Crowbar>("crowbar");
	// プレイヤーの座標を取得
	m_playerPos = m_gameCamera->GetPosition();
	// プレイヤーの前方向を取得
	m_playerForward = g_camera3D->GetForward();

	m_item = FindGO<Item>("item");

	// バールを見つけているか
	bool isFindCrowbar = FindLocateOfCrowbar();
	// レコードを見つけているか
	bool isFindRecord = FindLocateOfRecord();
	// フロアの封鎖用ギミックを見つけているか
	bool isFindFloorGimmick = FindLocateOfFloorGimmick();

	// 全てを見つけていないならば
	if (isFindCrowbar == false
		&& isFindRecord == false
		&& isFindFloorGimmick == false)
	{
		// テキストを描画しない
		m_isDraw = false;
	}
	
	// 文字列の表示時間を減らす
	/*if (m_timeToDisplay >= 0.0f) {
		m_timeToDisplay -= g_gameTime->GetFrameDeltaTime();
	}*/
}

bool UI::FindLocateOfCrowbar()
{
	if (m_crowbar == nullptr) {
		return false;
	}
	// バールの座標
	Vector3 crowbarPos = m_crowbar->GetPosition();

	// プレイヤーとバールの座標の差
	Vector3 diffToItem = crowbarPos - m_playerPos;

	// プレイヤーとバールが一定距離以下ならば
	if (m_item->GetHaveCrowbar() == false 
		&& diffToItem.Length() <= DISTANCE_TO_ITEM
	) {
		// プレイヤーが見ている方向にアイテムがあるか調べる
		if (CheckPlayerOrientation(diffToItem)) {
			// 表示する文字列を指定
			SpecifyStringToDisplay("crowbar");
			return true;
		}
	}
	return false;
}

bool UI::FindLocateOfRecord()
{
	// レコードを検索して配列とする
	const auto& records = FindGOs<Record>("record");
	// レコード配列のサイズ
	const int recordSize = records.size();
	for (int i = 0; i < recordSize; i++) {
		m_record = records[i];
		if (m_record == nullptr) {
			return false;
		}
		// レコードの座標
		Vector3 recordPos = m_record->GetPosition();

		// プレイヤーとレコードの座標の差
		Vector3 diffToItem = recordPos - m_playerPos;

		// プレイヤーとレコードが一定距離以下ならば
		if (m_item->GetHaveRecord() == 0
			&& diffToItem.Length() <= DISTANCE_TO_ITEM) 
		{
			// プレイヤーが見ている方向にアイテムがあるか調べる
			if (CheckPlayerOrientation(diffToItem)) {
				// 表示する文字列を指定
				SpecifyStringToDisplay("record");
				return true;
			}
		}

	}
	return false;
}

bool UI::FindLocateOfFloorGimmick()
{
	// レコードを検索して配列とする
	const auto& gimmicks = FindGOs<FloorGimmick>("floorgimmick");
	// レコード配列のサイズ
	const int gimmickSize = gimmicks.size();
	for (int i = 0; i < gimmickSize; i++) {
		m_floorGimmick = gimmicks[i];
		if (m_floorGimmick == nullptr) {
			return false;
		}

		// レコードの座標
		Vector3 gimmickPos = m_floorGimmick->GetPosition();

		// プレイヤーとレコードの座標の差
		Vector3 diffToItem = gimmickPos - m_playerPos;

		// プレイヤーとレコードが一定距離以下ならば
		if (m_floorGimmick->GetAttackCount() != 0 
			&& diffToItem.Length() <= DISTANCE_TO_FLOORGIMMICK) {
			// プレイヤーが見ている方向にアイテムがあるか調べる
			if (CheckPlayerOrientation(diffToItem)) {
				// 表示する文字列を指定
				SpecifyStringToDisplay("floorGimmick");
				return true;
			}
		}
	}
	return false;
}

bool UI::CheckPlayerOrientation(Vector3 diffToItem)
{
	// プレイヤーから見たアイテムの方向
	diffToItem.Normalize();
	// 内積を求める
	float dot = m_playerForward.Dot(diffToItem);
	// プレイヤーがアイテムに近い方向を向いていたら
	if (dot >= VECTOR_CONSISTENCY) {
		// テキストを描画する
		m_isDraw = true;
		return true;
	}
	else {
		// テキストを描画しない
		m_isDraw = false;
		return false;
	}
}

void UI::SpecifyStringToDisplay(std::string item)
{
	wchar_t itemName[256];
	wchar_t supplement[256];
	// バールの場合
	if (item == "crowbar") {
		// バール用文字列を設定
		swprintf_s(itemName, 256, L"バールだ");
		swprintf_s(supplement, 256, L"何かに使えるかも");
		// アイテム名を設定
		m_itemNameFont.SetText(itemName);
		m_itemNameFont.SetPosition(ITEM_NAME_POSITION);
		// 補足情報を設定
		m_supplementFont.SetText(supplement);
		m_supplementFont.SetPosition(SUPPLEMENT_POSITION);

		// 文字列を表示するように設定する
		/*if (m_isCrowbarDescript == false) {
			m_isCrowbarDescript = true;
			m_timeToDisplay = TIME_TO_DISPLAY;
		}*/
		return;
	}
	// レコードの場合
	else if (item == "record") {
		// レコード用文字列を設定
		swprintf_s(itemName, 256, L"レコードだ");
		swprintf_s(supplement, 256, L"どこかで使えるかな");
		// アイテム名を設定
		m_itemNameFont.SetText(itemName);
		m_itemNameFont.SetPosition(ITEM_NAME_POSITION);
		// 補足情報を設定
		m_supplementFont.SetText(supplement);
		m_supplementFont.SetPosition(SUPPLEMENT_POSITION);

		// 文字列を表示するように設定する
		if (m_isRecordDescript == false) {
			m_isRecordDescript = true;
			m_timeToDisplay = TIME_TO_DISPLAY;
		}
		return;
	}
	// 
	else if (item == "floorGimmick") {
		// レコード用文字列を設定
		swprintf_s(itemName, 256, L"木の板で封鎖されている");
		swprintf_s(supplement, 256, L"どうにかして壊せないだろうか");
		// アイテム名を設定
		m_itemNameFont.SetText(itemName);
		m_itemNameFont.SetPosition(ITEM_NAME_POSITION);
		// 補足情報を設定
		m_supplementFont.SetText(supplement);
		m_supplementFont.SetPosition(SUPPLEMENT_POSITION);

		// 文字列を表示するように設定する
		if (m_isFloorGimmickDescript == false) {
			m_isFloorGimmickDescript = true;
			m_timeToDisplay = TIME_TO_DISPLAY;
		}
		return;
	}
}

void UI::MakeGetSound()
{
	/*m_se = NewGO<SoundSource>(10);
	m_se->Init(10);
	m_se->SetVolume(1.0f);
	m_se->Play(false);*/
}

void  UI::Render(RenderContext& rc)
{
	if (m_isDraw) {
		// 描画
		m_itemNameFont.Draw(rc);
		m_supplementFont.Draw(rc);
	}
}