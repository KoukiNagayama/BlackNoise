#include "stdafx.h"
#include "Game.h"
#include "Light.h";
#include "Player.h"
#include "GameCamera.h"
#include "sound/SoundEngine.h"
#include "InfoForEdge.h"

Game::Game()
{
}

Game::~Game()
{

}

bool Game::Start()
{
	//////////////////////////////////////////////////////////////////////////////////////
	////アニメーションのロード
	//m_animationClipArray[enAnimClip_Idle].Load("Assets/animData/idle.tka");
	//m_animationClipArray[enAnimClip_Idle].SetLoopFlag(true);
	//m_animationClipArray[enAnimClip_Run].Load("Assets/animData/walk.tka");
	//m_animationClipArray[enAnimClip_Run].SetLoopFlag(true);

	//
	////モデルの初期化
	//m_modelRender.Init("Assets/modelData/unityChan.tkm");
	//////////////////////////////////////////////////////////////////////////////////////
	//PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();
	//背景モデルの読み込み。

	//m_bgModelRender.Init("Assets/modelData/stage/Sample.tkm", nullptr, 0, true, enModelUpAxisZ, false);

	//各クラスを生成。
	m_gamecam = NewGO<GameCamera>(0, "gamecamera");
	m_player = NewGO<Player>(0, "player");
	
	
	//m_modelRender.Init("Assets/modelData/unityChan.tkm");

	m_bgModelRender.Init("Assets/modelData/bg.tkm", nullptr, 0, true, enModelUpAxisZ, false);

	// サウンドテスト
	g_soundEngine->ResistWaveFileBank(0, "Assets/sound/gameBGM.wav");
	g_soundEngine->ResistWaveFileBank(1, "Assets/sound/shot.wav");
	//m_sound1->Play(false);
	Vector3 position;
	position.x = 5.0f;
	position.y = 1.0f;
	position.z = 1.0f;
	Vector3 position2;
	position2.x = 0.0f;
	position2.y = 0.0f;
	position2.z = 5.0f;
	g_infoForEdge.InitForSound(0, position, 200.0f, 0);
	g_infoForEdge.InitForSound(1, position2, 500.0f, 1);

	m_sound1 = NewGO<SoundSource>(0);
	m_sound2 = NewGO<SoundSource>(1);

	m_position.x = 100.0f;
	return true;
}

void Game::Update()
{

	m_position.x += g_pad[0]->GetLStickXF();
	m_position.z += g_pad[0]->GetLStickYF();

	Vector3 position2;
	position2.x = 0.0f;
	position2.y = 0.0f;
	position2.z = 5.0f;

	if (g_pad[0]->IsTrigger(enButtonB)) {
		m_sound2->Init(1, true);
		m_sound2->SetPosition(position2);
		m_sound2->Play(false);
	}

	int check2;
	if (m_sound2->IsPlaying() == true)
	{
		check2 = 1;
	}
	else {
		check2 = 0;
		m_sound2 = NewGO<SoundSource>(1);
	}
	g_infoForEdge.SetIsSound(1, check2);

	m_modelRender.SetPosition(m_position);
	m_modelRender.SetRotation(m_rotation);
	m_modelRender.SetScale(m_scale);
	m_bgModelRender.Update();
	m_modelRender.Update();

}

void Game::Render(RenderContext& rc)
{
	//背景を描写
	
	m_modelRender.Draw(rc);
	m_bgModelRender.Draw(rc);
}
