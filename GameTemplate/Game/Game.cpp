#include "stdafx.h"
#include "Game.h"
#include "Light.h";

Game::Game()
{
}

Game::~Game()
{

}

bool Game::Start()
{
	//�A�j���[�V�����̃��[�h
	m_animationClipArray[enAnimClip_Idle].Load("Assets/animData/idle.tka");
	m_animationClipArray[enAnimClip_Idle].SetLoopFlag(true);
	m_animationClipArray[enAnimClip_Run].Load("Assets/animData/walk.tka");
	m_animationClipArray[enAnimClip_Run].SetLoopFlag(true);

	
	//���f���̏�����

	m_bgModelRender.Init("Assets/modelData/bg.tkm");

	m_modelRender.Init("Assets/modelData/unityChan.tkm", m_animationClipArray, enAnimClip_Num, true, enModelUpAxisY);

	return true;
}

void Game::Update()
{
	// ���s�ړ�
	//m_position.x += g_pad[0]->GetLStickXF();
	//m_position.y += g_pad[0]->GetLStickYF();

	// ��]
/*	m_rotation.AddRotationY(g_pad[0]->GetRStickXF() * 0.05f);
	m_rotation.AddRotationX(g_pad[0]->GetRStickYF() * 0.05f);*/

	// �g�嗦�ύX
	/*if (g_pad[0]->IsPress(enButtonUp)) {
		m_scale.y += 0.02f;
	}
	if (g_pad[0]->IsPress(enButtonDown)) {
		m_scale.y -= 0.02f;
	}
	if (g_pad[0]->IsPress(enButtonRight)) {
		m_scale.x += 0.02f;
	}
	if (g_pad[0]->IsPress(enButtonLeft)) {
		m_scale.x -= 0.02f;
	}*/

	//�A�j���[�V�����؂�ւ�
	if (g_pad[0]->IsPress(enButtonA)) {
		m_modelRender.PlayAnimation(enAnimClip_Idle, 0.2f);
	}
	if (g_pad[0]->IsPress(enButtonB)) {
		m_modelRender.PlayAnimation(enAnimClip_Run, 0.2f);
	}

	//�|�C���g���C�g�ړ�
	//m_spPosition.x += g_pad[0]->GetLStickXF();
	//m_spPosition.z += g_pad[0]->GetLStickYF(); 
	//g_light.SetSpotLightPosition(m_spPosition);

	m_modelRender.SetPosition(m_position);
	m_modelRender.SetRotation(m_rotation);
	m_modelRender.SetScale(m_scale);
	m_modelRender.Update();
	m_bgModelRender.Update();
}

void Game::Render(RenderContext& rc)
{
	// �h���[
	m_bgModelRender.Draw(rc);

	m_modelRender.Draw(rc);
}
