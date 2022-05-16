#include "stdafx.h"
#include "system/system.h"
#include "Game.h"
#include "GameStart.h"
#include "DirectionLight.h"
#include "Light.h"
#include "Bloom.h"
#include "Shadow.h"
#include "CreatingMaps.h"
#include "Title.h"
#include "ForwardRendering.h"
#include "MainRenderTarget.h"


// K2EngineLow�̃O���[�o���A�N�Z�X�|�C���g�B
K2EngineLow* g_k2EngineLow = nullptr;

/// <summary>
/// ���C���֐�
/// </summary>
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	// �Q�[���̏������B
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("Game"));
	
	// k2EngineLow�̏������B
	g_k2EngineLow = new K2EngineLow();
	g_k2EngineLow->Init(g_hWnd, FRAME_BUFFER_W, FRAME_BUFFER_H);
	g_camera3D->SetPosition({ 0.0f, 50.0f, 200.0f });
	g_camera3D->SetTarget({ 0.0f, 70.0f, 0.0f });
	
	// ���C�g���̏�����
	//g_light.Init();
	
	//g_shadow.Init();

	g_creatingMaps.Init();

	auto game = NewGO<GameStart>(0,"gamestart");

	//auto title = NewGO<Title>(0, "title");

	// �����_�����O�R���e�L�X�g
	auto& renderContext = g_graphicsEngine->GetRenderContext();

	g_mainRenderTarget.Init();

	// �u���[����������
	g_bloom.InitBloom(g_mainRenderTarget.GetMainRenderTarget());


	// ��������Q�[�����[�v�B
	while (DispatchWindowMessage())
	{
		
		// �t���[���̊J�n���ɌĂяo���K�v�����鏈�������s
		g_k2EngineLow->BeginFrame();


		// �Q�[���I�u�W�F�N�g�}�l�[�W���[�̍X�V�������Ăяo���B
		g_k2EngineLow->ExecuteUpdate();

		// �V���h�E�}�b�v�ւ̃��f���̕`��
		//g_shadow.RenderToShadowMap(renderContext);

		//  �[�x�l�}�b�v�A���[���h���W�A�@���}�b�v�̍X�V
		g_creatingMaps.Update();

		// �[�x�l�}�b�v�A���[���h���W�A�@���}�b�v�ւ̃��f���̕`��
		g_creatingMaps.RenderToDepthValueMap(renderContext);

		// ���C�g���̍X�V
		//g_light.Update();

		// �֊s�����̍X�V
		g_infoForEdge.Update();

		// �t�H���[�h�����_�����O�ɂ��`��
		g_forwardRendering.Render(renderContext);

		// �Q�[���I�u�W�F�N�g�}�l�[�W���[�̕`�揈�����Ăяo���B
		g_k2EngineLow->ExecuteRender();

		// �u���[���̕`��
		//g_bloom.Render(renderContext, g_mainRenderTarget.GetMainRenderTarget());

		// ���C�������_�����O�^�[�Q�b�g���t���[���o�b�t�R�s�[
		g_mainRenderTarget.CopyMainRenderTargetToFrameBuffer(renderContext);
		
		// �f�o�b�O�`�揈�������s����B
		g_k2EngineLow->DebubDrawWorld();

		// ���C�������_�����O�^�[�Q�b�g���N���A
		renderContext.ClearRenderTargetView(g_mainRenderTarget.GetMainRenderTarget());


		// �t���[���̏I�����ɌĂяo���K�v�����鏈�������s�B
		g_k2EngineLow->EndFrame();
	}

	delete g_k2EngineLow;

	return 0;
}
