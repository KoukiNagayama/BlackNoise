#include "stdafx.h"
#include "EntranceDoor.h"
#include "GroundFloor.h"

bool EntranceDoor::Start()
{
	m_animationClips[enAnimationClip_Idle].Load("Assets/animData/entranceDoor/normal.tka");
	m_animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Open].Load("Assets/animData/entranceDoor/open.tka");
	m_animationClips[enAnimationClip_Open].SetLoopFlag(false);
	// ���f���̏�����
	m_modelRender.Init("Assets/modelData/stage1/EntranceDoor.tkm", m_animationClips, enAnimationClip_Num);
	// ���W�A��]�A�g�嗦��ݒ�
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
	// ���f���̍X�V
	m_modelRender.Update();
}

void EntranceDoor::Render(RenderContext& rc)
{
	// ���f���̕`��
	m_modelRender.Draw(rc);
}