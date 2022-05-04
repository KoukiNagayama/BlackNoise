#include "stdafx.h"
#include "BrokenDoor.h"
#include "GameCamera.h"

namespace
{
	const float DISTANCE = 70.0f;		//ドアとの距離
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
	m_modelRender.Init("Assets/modelData/stage/door.tkm", m_animationClips, enAnimationClip_Num);
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
	//モデル更新
	m_modelRender.Update();
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
	if (m_haveHammer != false && NearDoor() != false)
	{
		//壊せる
		return true;
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
	if (CanBreakDoor() == false)
	{
		return;
	}
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		//モデルを差し替える
		m_modelRender.Init("Assets/modelData/stage/brokendoor_low.tkm", m_animationClips, enAnimationClip_Num);

		//音を出す
		
		//ステートをクローズ状態にする
		m_doorState = enDoorState_CloseIdle;

	}
}

void BrokenDoor::CloseIdleState()
{
	TransitionState();
}

void BrokenDoor::CloseState()
{
	ReleasePhysicsObject();
	//クローズアニメーションが終了したら。
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//当たり判定を作成する。
		CreatePhysicsObject();
		//クローズ終わりステートに遷移する。
		m_doorState = enDoorState_CloseIdle;
	}
}
void BrokenDoor::OpenState()
{
	ReleasePhysicsObject();
	//クローズアニメーションが終了したら。
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//当たり判定を作成する。
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
	if(m_doorState == enDoorState_Before || m_doorState != enDoorState_CloseIdle)
	m_modelRender.Draw(rc);
}