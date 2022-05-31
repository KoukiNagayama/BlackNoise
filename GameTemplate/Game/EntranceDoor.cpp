#include "stdafx.h"
#include "EntranceDoor.h"
#include "GroundFloor.h"

bool EntranceDoor::Start()
{
	m_animationClips[enAnimationClip_Idle].Load("Assets/animData/entranceDoor/normal.tka");
	m_animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Open].Load("Assets/animData/entranceDoor/open.tka");
	m_animationClips[enAnimationClip_Open].SetLoopFlag(false);
	// モデルの初期化
	m_modelRender.Init("Assets/modelData/stage1/EntranceDoor.tkm", m_animationClips, enAnimationClip_Num);
	// 座標、回転、拡大率を設定
	m_modelRender.SetTRS(m_position, m_rotation, m_scale);

	m_groundFloor = FindGO<GroundFloor>("groundfloor");

	return true;
}

void EntranceDoor::Update()
{
	if (m_groundFloor->IsGameClear() == true && m_isOpened == false) {
		m_modelRender.PlayAnimation(enAnimationClip_Open, 0.1f);
		m_isOpened = true;
	}
	// モデルの更新
	m_modelRender.Update();
}

void EntranceDoor::Render(RenderContext& rc)
{
	// モデルの描画
	m_modelRender.Draw(rc);
}