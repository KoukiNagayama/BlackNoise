#include "stdafx.h"
#include "Door.h"
#include "GameCamera.h"
#include "sound/SoundEngine.h"
#include "sound/SoundSource.h"

namespace
{
	const float DISTANCE = 90.0f;
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
	m_modelRender.Init("Assets/modelData/stage/door.tkm", m_animationClips, enAnimationClip_Num);
	//座標を設定する。
	m_modelRender.SetPosition(m_position);
	//大きさを設定する。
	m_modelRender.SetScale(m_scale);
	//回転を設定する。
	m_modelRender.SetRotation(m_rotation);

	//モデルの更新。
	m_modelRender.Update();
	//当たり判定を作成する。
	//m_physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());

	m_gamecam = FindGO<GameCamera>("gamecamera");

	//音を読み込む。
	//g_soundEngine->ResistWaveFileBank(5, "Assets/sound/door_cut.wav");
	return true;
}

void Door::Update()
{
	//アニメーションを再生する。
	PlayAnimation();
	//ステート管理。
	ManageState();

	NearPlayer();

	//モデルの更新。
	m_modelRender.Update();
}
bool Door::NearPlayer()
{
	Vector3 cameraPos = m_gamecam->GetPosition();
	cameraPos.y = 0.0f;
	Vector3 disToPlayer = m_position - cameraPos;
	// 一定距離近づいたら
	test = disToPlayer.Length();
	if (disToPlayer.Length() <= DISTANCE) {
		return true;
	}
}

void Door::TransitionState()
{
	test = test;
	if (NearPlayer()!=false && g_pad[0]->IsTrigger(enButtonA))
	{
		//閉じているときは
		if (m_doorState = enDoorState_CloseIdle)
		{
			//開ける
			m_doorState = enDoorState_Open;
		}
		//開いているときは
		else if (m_doorState = enDoorState_OpenIdle)
		{
			//閉じる
			m_doorState = enDoorState_Close;
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
	//オープンアニメーションの再生が終了したら。
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//当たり判定を開放する。
		//ReleasePhysicsObject();
		//オープン終わりステートに遷移する。
		m_doorState = enDoorState_OpenIdle;
	}
}
void Door::OpenIdleState()
{
	TransitionState();
}

void Door::CloseState()
{
	//クローズアニメーションが終了したら。
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//当たり判定を作成する。
		//CreatePhysicsObject();
		//クローズ終わりステートに遷移する。
		m_doorState = enDoorState_CloseIdle;
	}
}
void Door::CloseIdleState()
{
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

