#include "stdafx.h"
#include "Title.h"
#include "TitleCamera.h"
#include "TitleText.h"
#include "BackGround.h"
#include "TitleSprite.h"
#include "GameStart.h"

#include "sound/SoundEngine.h"


namespace {
	const float RANGE = 800.0f;						// ベルの音による輪郭線が描画される範囲
	const float EDGE_FADE_IN_DELTA_VALUE = 0.01f;	// エッジがフェードインするときの変位量
	const float EDGE_FADE_OUT_DELTA_VALUE = 0.005f;	// エッジがフェードアウトするときの変位量
	const float RATE_BY_TIME_MAX_VALUE = 1.00f;		// 時間による影響率の最大値
	const float RATE_BY_TIME_MIN_VALUE = 0.00f;		// 時間による影響率の最小値
}

Title::~Title()
{
	DeleteGO(m_titleCamera);
	DeleteGO(m_titleSprite);
	DeleteGO(m_titleText);
	DeleteGO(m_backGround);
	g_infoForEdge.Clear();
}

bool Title::Start()
{
	// 音源を登録
	g_soundEngine->ResistWaveFileBank(10, "Assets/sound/item/bell_low.wav");
	

	// 各オブジェクトを生成
	m_levelRender1.Init("Assets/level3D/title.tkl", [&](LevelObjectData& objData)
	{
		// タイトルモデル
		if (objData.EqualObjectName(L"titleText") == true) {
			m_titleText = NewGO<TitleText>(0, "titleText");
			m_titleText->SetPosition(objData.position);
			return true;
		}
		// タイトル用カメラ
		if (objData.EqualObjectName(L"titleCamera") == true) {
			m_titleCamera = NewGO<TitleCamera>(0, "titleCamera");
			m_titleCamera->SetPosition(objData.position);
			return true;
		}
		// 背景
		if (objData.EqualObjectName(L"stage2") == true) {
			m_backGround = NewGO<BackGround>(0, "backGround");
			m_backGround->SetPosition(objData.position);
			return true;
		}
	});
	// タイトル用画像
	m_titleSprite = NewGO<TitleSprite>(0, "titleSprite");

	// タイトル用カメラを検索
	m_titleCamera = FindGO<TitleCamera>("titleCamera");
	// タイトル用カメラの座標を取得
	m_position = m_titleCamera->GetPosition();

	// 音源データを初期化
	g_infoForEdge.InitForSound(0, m_position, RANGE, 0, 1.0f);
	return true;
}

void Title::Update()
{
	ManageState();

	ChangeRate();

	g_infoForEdge.SetInfoForSound(0, m_position, RANGE, m_isRateChange, m_rateByTime);
}

void Title::ManageState()
{
	switch (m_titleState) {
	// 待機中ならば
	case enTitleState_Idle:
		ProcessIdleStateTransition();
		break;
	// ベルを鳴らしている状態ならば
	case enTitleState_RingBell:
		ProcessRingBellStateTransition();
		break;
	case enTitleState_FadeOut:
		ProcessFromFadeOutStateToStartingGame();
		break;
	}
}

void Title::ProcessIdleStateTransition()
{
	// Bボタンが押されたならば
	if (g_pad[0]->IsTrigger(enButtonB)) {
		// ステートをベルを鳴らした状態に遷移
		m_titleState = enTitleState_RingBell;
		// 音を鳴らす
		MakeSound();
	}
}

void Title::ProcessRingBellStateTransition()
{
	if (m_isEndFadeIn == false) {
		return;
	}
	// Aボタンが押されたならば
	if (g_pad[0]->IsTrigger(enButtonA)) {
		m_titleState = enTitleState_FadeOut;
	}
}

void Title::ProcessFromFadeOutStateToStartingGame()
{
	if (m_isEndStanging == true) {
		// 輪郭線の登録情報をクリアする
		g_infoForEdge.Clear();
		// ゲームクラスのオブジェクトを生成する
		m_gameStart = NewGO<GameStart>(0, "gamestart");
		// 削除
		DeleteGO(this);
	}
}

void Title::MakeSound()
{
	m_bellSound = NewGO<SoundSource>(0);
	m_bellSound->Init(10);
	m_bellSound->Play(false);
	g_infoForEdge.SetIsSound(0, 1);
}

void Title::ChangeRate()
{
	switch (m_titleState) {
	case enTitleState_RingBell:
		IncreaseRate();
		break;
	case enTitleState_FadeOut:
		ReduceRate();
		break;
	}
}

void Title::IncreaseRate()
{
	if (m_bellSound != nullptr) {
		if (m_bellSound->IsPlaying() == true)
		{
			m_isRateChange = 1;
			if (m_rateByTime < RATE_BY_TIME_MAX_VALUE) {
				m_rateByTime += EDGE_FADE_IN_DELTA_VALUE;

			}
			if (m_rateByTime >= RATE_BY_TIME_MAX_VALUE) {
				m_isEndFadeIn = true;
			}
		}
	}
}

void Title::ReduceRate()
{
	m_isRateChange = 0;
	if (m_rateByTime > RATE_BY_TIME_MIN_VALUE && m_isRateChange == 0) {
		m_rateByTime -= EDGE_FADE_OUT_DELTA_VALUE;
		if (m_rateByTime <= RATE_BY_TIME_MIN_VALUE) {
			m_isEndStanging = true;
		}
	}
}

void Title::Render(RenderContext& rc)
{
	
}
