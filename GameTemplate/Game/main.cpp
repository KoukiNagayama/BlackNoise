#include "stdafx.h"
#include "system/system.h"
#include "Game.h"
#include "DirectionLight.h"
#include "Light.h"
#include "Bloom.h"
#include "Shadow.h"


// K2EngineLowのグローバルアクセスポイント。
K2EngineLow* g_k2EngineLow = nullptr;

void InitModel(Model& bgModel);
/// <summary>
/// メイン関数
/// </summary>
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	// ゲームの初期化。
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("Game"));
	
	// k2EngineLowの初期化。
	g_k2EngineLow = new K2EngineLow();
	g_k2EngineLow->Init(g_hWnd, FRAME_BUFFER_W, FRAME_BUFFER_H);
	g_camera3D->SetPosition({ 0.0f, 50.0f, 200.0f });
	g_camera3D->SetTarget({ 0.0f, 70.0f, 0.0f });
	
	g_light.Init();
	
	//g_shadow.Init("Assets/modelData/unityChan.tkm");

	auto game = NewGO<Game>(0,"game");

	Model bgModel;
	InitModel(bgModel);

	RenderTarget mainRenderTarget;
	mainRenderTarget.Create(
		1280,						//テクスチャの幅。
		720,						//テクスチャの高さ。
		1,							//Mipmapレベル。
		1,							//テクスチャ配列のサイズ。
		DXGI_FORMAT_R8G8B8A8_UNORM, //カラーバッファのフォーマット。
		DXGI_FORMAT_D32_FLOAT		//デプスステンシルバッファのフォーマット。
	);

	//g_bloom.InitBloom(mainRenderTarget);

	auto& renderContext = g_graphicsEngine->GetRenderContext();

	// ここからゲームループ。
	while (DispatchWindowMessage())
	{
		
		// フレームの開始時に呼び出す必要がある処理を実行
		g_k2EngineLow->BeginFrame();
		
		//bgModel.Draw(renderContext);
		// ゲームオブジェクトマネージャーの更新処理を呼び出す。
		g_k2EngineLow->ExecuteUpdate();
		g_light.Update();
		// ゲームオブジェクトマネージャーの描画処理を呼び出す。
		g_k2EngineLow->ExecuteRender();

		//g_shadow.Render(renderContext);

		//g_bloom.ChangeRenderTarget(renderContext, mainRenderTarget);

		//game->Render(renderContext);

		//g_bloom.Render(renderContext, mainRenderTarget);



		g_k2EngineLow->ExecuteRender();

		// デバッグ描画処理を実行する。
		g_k2EngineLow->DebubDrawWorld();

		// フレームの終了時に呼び出す必要がある処理を実行。
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