#include "stdafx.h"
#include "Bell.h"
#include "sound/SoundEngine.h"
#include "Player.h"
#include "GameCamera.h"
#include "InfoForEdge.h"

namespace
{
	const float POSITION = 60.0f;
	const float TIMER = 3.0f;
	const float VOLUME = 1.0f;
	const float RANGE = 800.0f;
	const float EDGE_FADE_IN_DELTA_VALUE = 0.07f;	// エッジがフェードインするときの変位量
	const float EDGE_FADE_OUT_DELTA_VALUE = 0.0f;	// エッジがフェードアウトするときの変位量
	const float RATE_BY_TIME_MAX_VALUE = 1.00f;		// 時間による影響率の最大値
	const float RATE_BY_TIME_MIN_VALUE = 0.00f;		// 時間による影響率の最小値
	const float MODEL_MULTIPLIER = 35.0f;
	const float MODEL_UP = 25.0f;
	const float SOUND_MULTIPLIER = 15.0f;
}

Bell::Bell()
{
}
Bell::~Bell()
{
	DeleteGO(m_bellSound);
}

bool Bell::Start()
{
	//アニメーションの初期化。
	animationClips[enAnimationClip_Idle].Load("Assets/animData/item/idle.tka");
	animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	animationClips[enAnimationClip_Ring].Load("Assets/animData/item/ring.tka");
	animationClips[enAnimationClip_Ring].SetLoopFlag(true);

	m_player = FindGO<Player>("player");
	m_gameCam = FindGO<GameCamera>("gamecamera");
	//サウンドを登録。
	g_soundEngine->ResistWaveFileBank(0, "Assets/sound/item/bell_low.wav");
	//モデルの初期化。
	m_modelRender.Init("Assets/modelData/item/bell.tkm");
	//m_modelRender.SetScale({ 0.7f,0.7f,0.7f });

	//サウンドをNewGO
	m_bellSound = NewGO<SoundSource>(0);

	g_infoForEdge.InitForSound(0, m_position, RANGE, 0, rate);

	return true;
}

void Bell::Update()
{
	//座標
	Position();
	//ステート管理
	ManageState();
	//アニメーション再生。
	//PlayAnimation();
	// デバッグ用の文字表示
	Font();
	//モデルの更新。
	m_modelRender.Update();
}

void Bell::Position()
{
	Vector3 up, right, forward;
	right = g_camera3D->GetRight();
	forward = g_camera3D->GetForward();
	up.Cross(right, forward);
	up.Normalize();
	//ベルのモデルの座標を計算する。
	m_position = g_camera3D->GetPosition();
	m_position += g_camera3D->GetRight() * MODEL_MULTIPLIER;
	m_position += g_camera3D->GetForward() * MODEL_MULTIPLIER;
	m_position += up * MODEL_UP;
	//座標を設定する
	m_modelRender.SetPosition(m_position);
	m_modelRender.SetRotation(m_player->GetRotation());

	//ベルの音の座標を少し前に設定する。
	m_soundPos = g_camera3D->GetPosition();
	m_soundPos += g_camera3D->GetForward() * SOUND_MULTIPLIER;

}

void Bell::TransitionState()
{
	if (g_pad[0]->IsTrigger(enButtonB))
	{
		m_bellState = enBellState_Ring;
		m_timer = TIMER;
		return;
	}
	m_bellState = enBellState_Idle;
}

void Bell::ManageState()
{
	switch (m_bellState)
	{
		//待機。
	case enBellState_Idle:
		Idle();
		break;
		//音を鳴らす。
	case enBellState_Ring:
		Ring();
	default:
		break;
	}
}

void Bell::Ring()
{
	if (m_timer >= TIMER) {
		m_bellSound->Init(0);
		m_bellSound->SetPosition(m_position);
		m_bellSound->SetVolume(VOLUME);
		m_bellSound->Play(false);
		
	}
	int check;
	if (m_bellSound != nullptr) {
		if (m_bellSound->IsPlaying() == true)
		{
			check = 1;
			if (rate < RATE_BY_TIME_MAX_VALUE) {
				rate += EDGE_FADE_IN_DELTA_VALUE;
			}
		}
		else {
			check = 0;
			if (rate > RATE_BY_TIME_MIN_VALUE && check == 0) {
				rate -= EDGE_FADE_OUT_DELTA_VALUE;
				if (rate <= RATE_BY_TIME_MIN_VALUE) {
					rate = RATE_BY_TIME_MIN_VALUE;
					TransitionState();
				}
			}
		}
		g_infoForEdge.SetInfoForSound(0, m_soundPos, RANGE, check, rate);
	}


	m_timer -= g_gameTime->GetFrameDeltaTime();
	if (m_timer <= 0.0f)
	{
		m_bellSound = NewGO<SoundSource>(0);
		m_timer = 0.0f;
	}
}

void Bell::Idle()
{
	TransitionState();
}

void Bell::PlayAnimation()
{
	switch (m_bellState)
	{
		//待機。
	case enBellState_Idle:
		m_modelRender.PlayAnimation(enAnimationClip_Idle, 0.3f);
		break;
		//音を鳴らす。
	case enBellState_Ring:
		m_modelRender.PlayAnimation(enAnimationClip_Ring, 0.3f);
	default:
		break;
	}
}

void Bell::Font()
{
	wchar_t wcsbuf[256];
	swprintf_s(wcsbuf, 256, L"timer:%.3f", m_timer);
	//表示するテキストを設定。
	m_font.SetText(wcsbuf);
	//フォントの位置を設定。
	m_font.SetPosition(Vector3(-760.0f, 400.0f, 0.0f));
	//フォントの大きさを設定。
	m_font.SetScale(1.0f);
	swprintf_s(wcsbuf, 256, L"rate:%.3f", rate);
	//表示するテキストを設定。
	m_font1.SetText(wcsbuf);
	//フォントの位置を設定。
	m_font1.SetPosition(Vector3(-760.0f, 300.0f, 0.0f));
	//フォントの大きさを設定。
	m_font1.SetScale(1.0f);
}

void Bell::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
	m_font.Draw(rc);
	m_font1.Draw(rc);
}