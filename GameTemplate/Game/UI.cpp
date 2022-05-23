#include "stdafx.h"
#include "UI.h"
#include "sound/SoundEngine.h"
#include "GameCamera.h"
#include "Crowbar.h"
#include "Gramophone.h"
#include "Hammer.h"
#include "Record.h"


namespace
{
	const Vector3 ITEM_NAME_POSITION = Vector3(200.0f, -80.0f, 0.0f);	// フォントを表示する座標
	const Vector3 SUPPLEMENT_POSITION = Vector3(200.0f, -120.0f, 0.0f);
	const float DISTANCE_TO_ITEM = 400.0f;								// アイテムとの距離
}

bool UI::Start()
{
	//m_spriteRender.Init("Assets/sprite/Abutton.dds", 128, 128);
	//m_spriteRender.SetPosition(Vector3(0.0f, -130.0f, 0.0f));
	
	//破壊音の読み込み
	g_soundEngine->ResistWaveFileBank(10, "Assets/sound/se/item_get.wav");

	m_gameCamera = FindGO<GameCamera>("gamecamera");

	m_crowbar = FindGO<Crowbar>("crowbar");


	return true;
}

void UI::Update()
{
	if (m_crowbar == nullptr) {
		return;
	}
	// バールを検索
	m_crowbar = FindGO<Crowbar>("crowbar");

	// プレイヤーの座標を取得
	m_playerPos = m_gameCamera->GetPosition();
	// プレイヤーの前方向を取得
	m_playerForward = g_camera3D->GetForward();

	if (m_crowbar != nullptr) {
		FindLocateOfItem();
	}
}

void UI::FindLocateOfItem()
{

	// バールの座標
	Vector3 crowbarPos = m_crowbar->GetPosition();
	// プレイヤーとアイテムの座標の差
	Vector3 diffToItem = crowbarPos - m_playerPos;

	// プレイヤーとバールが一定距離以下ならば
	if (diffToItem.Length() <= DISTANCE_TO_ITEM) {
		// プレイヤーが見ている方向にアイテムがあるか調べる
		if (CheckPlayerOrientation(diffToItem)) {
			// 表示する文字列を指定
			SpecifyStringToDisplay("crowbar");
		}
	}

}

bool UI::CheckPlayerOrientation(Vector3 diffToItem)
{
	// プレイヤーから見たアイテムの方向
	diffToItem.Normalize();
	// 内積を求める
	float dot = m_playerForward.Dot(diffToItem);
	// プレイヤーがアイテムに近い方向を向いていたら
	if (dot >= 0.8f) {
		m_isDraw = true;
		return true;
	}
	else {
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
	}
}

void UI::MakeGetSound()
{
	m_se = NewGO<SoundSource>(10);
	m_se->Init(10);
	m_se->SetVolume(1.0f);
	m_se->Play(false);
}

void  UI::Render(RenderContext& rc)
{
	if (m_isDraw == true) {
		// 描画
		m_itemNameFont.Draw(rc);
		m_supplementFont.Draw(rc);
	}
}