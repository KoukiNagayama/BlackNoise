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
	const Vector3	ITEM_NAME_POSITION = Vector3(200.0f, -80.0f, 0.0f);		// アイテム名を表示する座標
	const Vector3	SUPPLEMENT_POSITION = Vector3(200.0f, -120.0f, 0.0f);	// 補足を表示する座標
	const float		DISTANCE_TO_ITEM = 400.0f;								// アイテムとの距離
	const float		VECTOR_CONSISTENCY = 0.8f;								// ベクトルが一致しているか比較する値
	const float		TIME_TO_DISPLAY = 2.5f;
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

	// バールを検索
	m_crowbar = FindGO<Crowbar>("crowbar");
	// プレイヤーの座標を取得
	m_playerPos = m_gameCamera->GetPosition();
	// プレイヤーの前方向を取得
	m_playerForward = g_camera3D->GetForward();


	FindLocateOfCrowbar();
	
	// 文字列の表示時間を減らす
	if (m_timeToDisplay >= 0.0f) {
		m_timeToDisplay -= g_gameTime->GetFrameDeltaTime();
	}
}

void UI::FindLocateOfCrowbar()
{
	if (m_crowbar == nullptr) {
		return;
	}
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
			return;
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
	if (dot >= VECTOR_CONSISTENCY) {
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

		// 文字列を表示するように設定する
		if (m_isCrowbarDescript == false) {
			m_isCrowbarDescript = true;
			m_timeToDisplay = TIME_TO_DISPLAY;
		}
		return;
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
	if (m_isDraw) {
		if(m_timeToDisplay >= 0.0f){
			// 描画
			m_itemNameFont.Draw(rc);
			m_supplementFont.Draw(rc);
		}
	}
}