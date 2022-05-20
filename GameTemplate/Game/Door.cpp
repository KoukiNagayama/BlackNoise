#include "stdafx.h"
#include "Door.h"
#include "GameCamera.h"
#include "Enemy.h"
#include "sound/SoundEngine.h"

namespace
{
	const float DISTANCE = 130.0f;					//プレイヤーの距離
	const float EDGE_FADE_IN_DELTA_VALUE = 0.07f;	// エッジがフェードインするときの変位量
	const float EDGE_FADE_OUT_DELTA_VALUE = 0.005f;	// エッジがフェードアウトするときの変位量
	const float RATE_BY_TIME_MAX_VALUE = 1.00f;		// 時間による影響率の最大値
	const float RATE_BY_TIME_MIN_VALUE = 0.00f;		// 時間による影響率の最小値
	const float SOUND_RANGE = 300.0f;				//影響する範囲
	const float ADD_DEG = 4.0f;						//1フレームで加算する角度
}

Door::Door()
{

}

Door::~Door()
{

}

bool Door::Start()
{
	//アニメーションを読み込む。
	m_animationClips[enAnimationClip_CloseIdle].Load("Assets/animData/door/closeidle.tka");
	m_animationClips[enAnimationClip_CloseIdle].SetLoopFlag(false);
	m_animationClips[enAnimationClip_OpenIdle].Load("Assets/animData/door/openidle.tka");
	m_animationClips[enAnimationClip_OpenIdle].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Open].Load("Assets/animData/door/open.tka");
	m_animationClips[enAnimationClip_Open].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Close].Load("Assets/animData/door/close.tka");
	m_animationClips[enAnimationClip_Close].SetLoopFlag(false);
	//モデルを読み込む。
	m_modelRender.Init("Assets/modelData/stage/door.tkm");
	//座標を設定する。
	m_modelRender.SetPosition(m_position);
	//大きさを設定する。
	m_modelRender.SetScale(m_scale);
	//回転を設定する。
	m_modelRender.SetRotation(m_rotation);

	//モデルの更新。
	m_modelRender.Update();
	//当たり判定を作成する。
	m_physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());

	//音の読み込み
	g_soundEngine->ResistWaveFileBank(12, "Assets/sound/door/door_open.wav");
	g_soundEngine->ResistWaveFileBank(13, "Assets/sound/door/door_close.wav");
	//輪郭線描写の初期化
	g_infoForEdge.InitForSound(8, m_position, SOUND_RANGE, 0, m_rateByTime);

	m_gamecam = FindGO<GameCamera>("gamecamera");
	m_enemy = FindGO<Enemy>("enemy");
	return true;
}

void Door::Update()
{
	//アニメーションを再生する。
	PlayAnimation();
	//ステート管理。
	ManageState();
	//ドアとの距離
	NearPlayer();
	//ドアとの距離(Enemy)
	NearEnemy();
	//影響率
	CheckRate();

	//モデルの更新。
	m_modelRender.Update();
}
bool Door::NearPlayer()
{
	Vector3 cameraPos = m_gamecam->GetPosition();
	cameraPos.y = 0.0f;
	Vector3 disToPlayer = m_position - cameraPos;
	// 一定距離近づいたら
	if (disToPlayer.Length() <= DISTANCE) {
		return true;
	}
	return false;
}

bool Door::NearEnemy()
{
	Vector3 enemyPos = m_enemy->GetPosition();
	enemyPos.y = 0.0f;
	Vector3 disToPlayer = m_position - enemyPos;
	// 一定距離近づいたら
	if (disToPlayer.Length() <= DISTANCE) {
		return true;
	}
	return false;
}

void Door::TransitionState()
{
	//ドア壊せる状態のとき、Aボタンを押すと
	if (NearPlayer() != false && g_pad[0]->IsTrigger(enButtonA))
	{
		//しまっているとき
		if (m_doorState == enDoorState_CloseIdle)
		{
			MakeSound(0);
			//ドアを開ける
			m_doorState = enDoorState_Open;
		}
		//開いているとき
		else if (m_doorState == enDoorState_OpenIdle)
		{
			m_close = false;
			//ドアを閉じる
			m_doorState = enDoorState_Close;
		}
	}
	else if (NearEnemy() != false)
	{
		//しまっているとき
		if (m_doorState == enDoorState_CloseIdle)
		{
			MakeSound(0);
			//ドアを開ける
			m_doorState = enDoorState_Open;
		}
	}
}

void Door::PlayAnimation()
{
	switch (m_doorState)
	{
		//オープンステートの時。
	case enDoorState_Open:
		//オープンアニメーションを再生する。
		m_modelRender.PlayAnimation(enAnimationClip_Open);
		break;
		//オープンステートの時。
	case enDoorState_OpenIdle:
		//オープンアニメーションを再生する。
		m_modelRender.PlayAnimation(enAnimationClip_OpenIdle);
		break;
		//クローズステートの時。
	case enDoorState_Close:
		//クローズアニメーションを再生する。
		m_modelRender.PlayAnimation(enAnimationClip_Close);
		break;
		//クローズステートの時。
	case enDoorState_CloseIdle:
		//クローズアニメーションを再生する。
		m_modelRender.PlayAnimation(enAnimationClip_Close);
		break;
	default:
		break;
	}
}
void Door::MakeSound(int number)
{
	switch (number)
	{
		//開く音
	case 0:
		m_sound = NewGO<SoundSource>(12);
		m_sound->Init(12);
		m_sound->SetVolume(1.0f);
		m_sound->Play(false);
		break;

		//閉じる音
	case 1:
		m_sound = NewGO<SoundSource>(13);
		m_sound->Init(13);
		m_sound->SetVolume(1.0f);
		m_sound->Play(false);
		break;
	default:
		break;
	}
}

void Door::CheckRate()
{
	int check1;
	if (m_sound != nullptr) {
		if (m_sound->IsPlaying() == true)
		{
			check1 = 1;
			if (m_rateByTime < RATE_BY_TIME_MAX_VALUE) {
				m_rateByTime += EDGE_FADE_IN_DELTA_VALUE;
			}
		}
		else {
			check1 = 0;
			if (m_rateByTime > RATE_BY_TIME_MIN_VALUE && check1 == 0) {
				m_rateByTime -= EDGE_FADE_OUT_DELTA_VALUE;
				if (m_rateByTime <= RATE_BY_TIME_MIN_VALUE) {
					m_rateByTime = RATE_BY_TIME_MIN_VALUE;
				}
			}
		}
		g_infoForEdge.SetInfoForSound(5, m_position, SOUND_RANGE, check1, m_rateByTime);
	}
}

void Door::ReleasePhysicsObject()
{
	//当たり判定を開放する。
	m_physicsStaticObject.Release();
}

void Door::CreatePhysicsObject()
{
	//当たり判定を作成する。
	m_physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());
}

void Door::OpenState()
{
	ReleasePhysicsObject();

	//100度回転させる
	if (m_deg <= 200.0f)
	{
		m_rotation.AddRotationDegY(ADD_DEG);
		m_modelRender.SetRotation(m_rotation);
		m_deg += ADD_DEG;
	}
	else
	{
		CreatePhysicsObject();
		//クローズ終わりステートに遷移する。
		m_doorState = enDoorState_OpenIdle;
	}
}
void Door::OpenIdleState()
{
	TransitionState();
}

void Door::CloseState()
{
	ReleasePhysicsObject();
	//if(PlayAnimation)

	if (m_deg >= 0.0f)
	{
		m_rotation.AddRotationDegY(-ADD_DEG);
		m_modelRender.SetRotation(m_rotation);
		m_deg -= ADD_DEG;
	}
	else
	{
		CreatePhysicsObject();
		//クローズ終わりステートに遷移する。
		m_doorState = enDoorState_CloseIdle;
	}
}
void Door::CloseIdleState()
{
	if (m_close != true) {
		MakeSound(1);
		m_close = true;
	}
	TransitionState();
}



void Door::ManageState()
{
	switch (m_doorState)
	{
		//オープンステートの時。
	case enDoorState_Open:
		//オープンステートのステート遷移処理。
		OpenState();
		break;
		//オープン終わりステートの時。
	case enDoorState_OpenIdle:
		OpenIdleState();
		break;
		//クローズステートの時。
	case enDoorState_Close:
		//クローズステートのステート遷移処理。
		CloseState();
		break;
		//クローズ終わりステートの時。
	case enDoorState_CloseIdle:
		CloseIdleState();
		break;
	default:
		break;
	}
}

void Door::Render(RenderContext& rc)
{
	//モデルを描画する。
	m_modelRender.Draw(rc);
}

