#include "stdafx.h"
#include "Bell.h"
#include "sound/SoundEngine.h"
#include "Player.h"
#include "GameCamera.h"

namespace
{
	const float POSITION = 60.0f;
	const float TIMER = 3.0f;
}

Bell::Bell()
{
}
Bell::~Bell()
{
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
	g_soundEngine->ResistWaveFileBank(2, "Assets/sound/item/bell_low.wav");
	//モデルの初期化。
	m_modelRender.Init("Assets/modelData/item/bell.tkm");
	//サウンドをNewGO
	m_bellSound = NewGO<SoundSource>(2);

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
	// 
	Font();
	// 
	//モデルの更新。
	m_modelRender.Update();
}

void Bell::Position()
{
	m_position = g_camera3D->GetTarget();
	m_position += g_camera3D->GetRight() * 60.0f;
	m_position.y *= 0.6f;
	//座標を設定する。
	m_modelRender.SetPosition(m_position);
	m_modelRender.SetRotation(m_player->GetRotation());
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
		m_bellSound->Init(2);
		m_bellSound->SetPosition(m_position);
		m_bellSound->Play(false);
	}
	m_timer -= g_gameTime->GetFrameDeltaTime();
	if (m_timer <= 0.0f)
	{
		m_bellSound = NewGO<SoundSource>(2);
		m_timer = 0.0f;
		TransitionState();
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
	swprintf_s(wcsbuf, 256, L"%.3f", m_timer);
	//表示するテキストを設定。
	m_font.SetText(wcsbuf);
	//フォントの位置を設定。
	m_font.SetPosition(Vector3(-760.0f, 400.0f, 0.0f));
	//フォントの大きさを設定。
	m_font.SetScale(2.0f);
}

void Bell::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
	m_font.Draw(rc);
}