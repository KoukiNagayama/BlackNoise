#include "stdafx.h"
#include "system/system.h"
#include "Game.h"
#include "DirectionLight.h"
#include "Light.h"
#include "Bloom.h"


// K2EngineLow�̃O���[�o���A�N�Z�X�|�C���g�B
K2EngineLow* g_k2EngineLow = nullptr;

void InitModel(Model& bgModel);
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
	g_camera3D->SetTarget({ 0.0f, 100.0f, 0.0f });
	
	g_light.Init();

	auto game = NewGO<Game>(0,"game");

	Model bgModel;
	InitModel(bgModel);
	
	//RenderTarget offscreenRenderTarget;


	/*float a[4] = {1.0f,0.0f,0.0f,0.0f};

	bool b = offscreenRenderTarget.Create(
		500.0f,
		500.0f,
		1,
		1,
		DXGI_FORMAT_R8G8B8A8_UNORM, //�J���[�o�b�t�@�̃t�H�[�}�b�g�B
		DXGI_FORMAT_D32_FLOAT,		//�f�v�X�X�e���V���o�b�t�@�̃t�H�[�}�b�g�B
		a
	);*/

	RenderTarget mainRenderTarget;
	mainRenderTarget.Create(
		1280,
		720,
		1,
		1,
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		DXGI_FORMAT_D32_FLOAT
	);

	g_bloom.InitLuminanceRenderTarget(mainRenderTarget);
	g_bloom.InitSprite(mainRenderTarget);
	g_bloom.InitFinalSprite(mainRenderTarget);


	

	auto& renderContext = g_graphicsEngine->GetRenderContext();

	// ��������Q�[�����[�v�B
	while (DispatchWindowMessage())
	{
		
		// �t���[���̊J�n���ɌĂяo���K�v�����鏈�������s
		g_k2EngineLow->BeginFrame();
		
	//	bgModel.Draw(renderContext);
		// �Q�[���I�u�W�F�N�g�}�l�[�W���[�̍X�V�������Ăяo���B
		g_k2EngineLow->ExecuteUpdate();
		g_light.Update();
		// �Q�[���I�u�W�F�N�g�}�l�[�W���[�̕`�揈�����Ăяo���B
		g_k2EngineLow->ExecuteRender();

	/*	RenderTarget* rtArray[] = {&offscreenRenderTarget};
		//�����_�����O�^�[�Q�b�g�Ƃ��ė��p�ł���܂ő҂B
		renderContext.WaitUntilToPossibleSetRenderTargets(1, rtArray);
		//�����_�����O�^�[�Q�b�g��ݒ�B
		renderContext.SetRenderTargets(1, rtArray);
		//�����_�����O�^�[�Q�b�g���N���A�B
		renderContext.ClearRenderTargetViews(1, rtArray);*/

		g_bloom.ChangeRenderTarget(renderContext,mainRenderTarget);

		game->Render(renderContext);

		g_bloom.LuminanceExtraction(renderContext, mainRenderTarget);
		g_bloom.RunBlur(renderContext);
		g_bloom.CompositeImageToMainRenderTarget(renderContext, mainRenderTarget);

		g_k2EngineLow->ExecuteRender();

		// �f�o�b�O�`�揈�������s����B
		g_k2EngineLow->DebubDrawWorld();

		// �t���[���̏I�����ɌĂяo���K�v�����鏈�������s�B
		g_k2EngineLow->EndFrame();
	}

	

	delete g_k2EngineLow;

	return 0;
}

void InitModel(Model& bgModel)
{
	ModelInitData bgModelInitData;
	bgModelInitData.m_tkmFilePath = "Assets/modelData/bg.tkm";
	bgModelInitData.m_fxFilePath = "Assets/shader/model.fx";
	bgModelInitData.m_vsEntryPointFunc = "VSMain";
	bgModelInitData.m_expandConstantBuffer = g_light.GetLightData();
	bgModelInitData.m_expandConstantBufferSize = sizeof(*g_light.GetLightData());
	bgModel.Init(bgModelInitData);
}


/*�R���Q��*/
/*kasdjfsghksghakhg*/
/*daisuke*/
/*���q�`�[���撣�낤�I*/

