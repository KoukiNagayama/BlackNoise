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

	auto& renderContext = g_graphicsEngine->GetRenderContext();

	RenderTarget mainRenderTarget;
	mainRenderTarget.Create(
		g_graphicsEngine->GetFrameBufferWidth(),
		g_graphicsEngine->GetFrameBufferHeight(),
		1,
		1,
		DXGI_FORMAT_R16G16B16A16_FLOAT,
		DXGI_FORMAT_UNKNOWN
	);

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


		//sprite.Update({ FRAME_BUFFER_W / -2.0f, FRAME_BUFFER_H / 2.0f,  0.0f }, g_quatIdentity, g_vec3One, { 0.0f, 1.0f });
		//sprite.Draw(renderContext);

		// �f�o�b�O�`�揈�������s����B
		g_k2EngineLow->DebubDrawWorld();

		// �t���[���̏I�����ɌĂяo���K�v�����鏈�������s�B
		g_k2EngineLow->EndFrame();
	}

	delete g_k2EngineLow;

	return 0;
}
