#include "stdafx.h"
#include "BrokenDoor.h"
#include "GameCamera.h"
#include "sound/SoundEngine.h"

namespace {
	const float DISTANCE = 150.0f;					//ドアとの距離
	const float EDGE_FADE_IN_DELTA_VALUE = 0.07f;	// エッジがフェードインするときの変位量
	const float EDGE_FADE_OUT_DELTA_VALUE = 0.005f;	// エッジがフェードアウトするときの変位量
	const float RATE_BY_TIME_MAX_VALUE = 1.00f;		// 時間による影響率の最大値
	const float RATE_BY_TIME_MIN_VALUE = 0.00f;		// 時間による影響率の最小値
	const float SOUND_RANGE = 900.0f;				//影響する範囲
	const float ADD_DEG = 4.0f;						//1フレームで加算する角度
}

bool BrokenDoor::Start()
{
	//アニメーションを読み込む。
	m_animationClips[enAnimationClip_Before].Load("Assets/animData/door/closeidle.tka");
	m_animationClips[enAnimationClip_Before].SetLoopFlag(true);
	m_animationClips[enAnimationClip_CloseIdle].Load("Assets/animData/brokendoor/closeidle.tka");
	m_animationClips[enAnimationClip_CloseIdle].SetLoopFlag(true);
	m_animationClips[enAnimationClip_OpenIdle].Load("Assets/animData/brokendoor/openidle.tka");
	m_animationClips[enAnimationClip_OpenIdle].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Open].Load("Assets/animData/brokendoor/open.tka");
	m_animationClips[enAnimationClip_Open].SetLoopFlag(false);
	m_animationClips[enAnimationClip_Close].Load("Assets/animData/brokendoor/close.tka");
	m_animationClips[enAnimationClip_Close].SetLoopFlag(false);
	//モデルを読み込む。
	m_modelRender.Init("Assets/modelData/stage/door.tkm");
	//座標を設定する。
	m_modelRender.SetPosition(m_position);
	//回転を設定する。
	m_modelRender.SetRotation(m_rotation);
	//モデルの更新。
	m_modelRender.Update();
	//当たり判定を作成する。
	m_physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());
	//ゲームカメラ
	m_gamecam = FindGO<GameCamera>("gamecamera");
	//破壊音の読み込み
	g_soundEngine->ResistWaveFileBank(9, "Assets/sound/door/door_break.wav");
	//輪郭線描写の初期化
	g_infoForEdge.InitForSound(5, m_position, SOUND_RANGE, 0, m_rateByTime);

	return true;
}

void BrokenDoor::Update()
{
	//ステート管理
	ManageState();
	//アニメーション再生
	PlayAnimation();
	//ドアから近いか
	NearDoor();
	//影響率
	CheckRate();
	//モデル更新
	m_modelRender.Update();
}

void BrokenDoor::MakeSound()
{
	m_sound = NewGO<SoundSource>(9);
	m_sound->Init(9);
	m_sound->SetVolume(1.0f);
	m_sound->Play(false);
}

void BrokenDoor::CheckRate()
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
bool BrokenDoor::NearDoor()
{
	Vector3 disToPlayer = m_position - m_gamecam->GetYaxisZeroPosition();
	if (disToPlayer.Length() <= DISTANCE)
	{
		return true;
	}
}

void BrokenDoor::ManageState()
{
	switch (m_doorState)
	{
		//壊す前
	case enDoorState_Before:
		BeforeState();
		break;
		//クローズ
	case enDoorState_Close:
		CloseState();
		break;
		//クローズ待機
	case enDoorState_CloseIdle:
		CloseIdleState();
		break;
		//オープン
	case enDoorState_Open:
		OpenState();
		break;
		//オープン待機
	case enDoorState_OpenIdle:
		OpenIdleState();
		break;
	default:
		break;
	}
}
void BrokenDoor::CreatePhysicsObject()
{
	m_physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());
}

void BrokenDoor::ReleasePhysicsObject()
{
	m_physicsStaticObject.Release();
}

bool BrokenDoor::CanBreakDoor()
{
	//ハンマーを持っていて、ドアに近い時
	if (m_haveHammer != false /*&& NearDoor() != false*/)
	{
		if (NearDoor() != false) {
			//壊せる
			return true;
		}
	}
}

void BrokenDoor::TransitionState()
{
	//壊す前のとき
	if (m_doorState == enDoorState_Before)
	{
		//何も処理しない
		return;
	}
	//ドア壊せる状態のとき、Aボタンを押すと
	if (NearDoor() != false && g_pad[0]->IsTrigger(enButtonA))
	{
		//しまっているとき
		if (m_doorState == enDoorState_CloseIdle)
		{
			//ドアを開ける
			m_doorState = enDoorState_Open;
		}
		//開いているとき
		else if (m_doorState == enDoorState_OpenIdle)
		{
			//ドアを閉じる
			m_doorState = enDoorState_Close;
		}
	}
}

void BrokenDoor::BeforeState()
{
	if (CanBreakDoor() != true)
	{
		return;
	}
	if (g_pad[0]->IsTrigger(enButtonX))
	{
		//モデルを差し替える
		m_modelRender.Init("Assets/modelData/stage/brokendoor.tkm");

		//音を出す
		MakeSound();
		
		//ステートをクローズ状態にする
		m_doorState = enDoorState_CloseIdle;
		
		ReleasePhysicsObject();
	}
}

void BrokenDoor::CloseIdleState()
{
	TransitionState();
}

void BrokenDoor::CloseState()
{
	ReleasePhysicsObject();

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
void BrokenDoor::OpenState()
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

void BrokenDoor::OpenIdleState()
{
	TransitionState();
}

void BrokenDoor::PlayAnimation()
{
	switch (m_doorState)
	{
		//壊す前の時
	case enDoorState_Before:
		//壊す前のドアアニメーションを再生
		m_modelRender.PlayAnimation(enAnimationClip_Before);
		break;
		//オープンステート
	case enDoorState_Open:
		//オープンアニメーションを再生
		m_modelRender.PlayAnimation(enAnimationClip_Open);
		break;
		//オープン待機ステート
	case enDoorState_OpenIdle:
		//オープンアニメーションを再生
		m_modelRender.PlayAnimation(enAnimationClip_OpenIdle);
		break;
		//クローズステート
	case enDoorState_Close:
		//クローズアニメーションを再生
		m_modelRender.PlayAnimation(enAnimationClip_Close);
		break;
		//クローズ待機ステート
	case enDoorState_CloseIdle:
		//クローズ待機アニメーションを再生
		m_modelRender.PlayAnimation(enAnimationClip_Close);
		break;
	default:
		break;
	}
}

void BrokenDoor::Render(RenderContext& rc)
{
	//if(m_doorState == enDoorState_Before || m_doorState != enDoorState_CloseIdle)
	m_modelRender.Draw(rc);
}