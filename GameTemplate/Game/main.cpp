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


// K2EngineLowのグローバルアクセスポイント。
K2EngineLow* g_k2EngineLow = nullptr;

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
	
	// ライト情報の初期化
	//g_light.Init();
	
	//g_shadow.Init();

	g_creatingMaps.Init();

	auto game = NewGO<GameStart>(0,"gamestart");

	//auto title = NewGO<Title>(0, "title");

	// レンダリングコンテキスト
	auto& renderContext = g_graphicsEngine->GetRenderContext();

	g_mainRenderTarget.Init();

	// ブルームを初期化
	g_bloom.InitBloom(g_mainRenderTarget.GetMainRenderTarget());


	// ここからゲームループ。
	while (DispatchWindowMessage())
	{
		
		// フレームの開始時に呼び出す必要がある処理を実行
		g_k2EngineLow->BeginFrame();


		// ゲームオブジェクトマネージャーの更新処理を呼び出す。
		g_k2EngineLow->ExecuteUpdate();

		// シャドウマップへのモデルの描画
		//g_shadow.RenderToShadowMap(renderContext);

		//  深度値マップ、ワールド座標、法線マップの更新
		g_creatingMaps.Update();

		// 深度値マップ、ワールド座標、法線マップへのモデルの描画
		g_creatingMaps.RenderToDepthValueMap(renderContext);

		// ライト情報の更新
		//g_light.Update();

		// 輪郭線情報の更新
		g_infoForEdge.Update();

		// フォワードレンダリングによる描画
		g_forwardRendering.Render(renderContext);

		// ゲームオブジェクトマネージャーの描画処理を呼び出す。
		g_k2EngineLow->ExecuteRender();

		// ブルームの描画
		//g_bloom.Render(renderContext, g_mainRenderTarget.GetMainRenderTarget());

		// メインレンダリングターゲットをフレームバッフコピー
		g_mainRenderTarget.CopyMainRenderTargetToFrameBuffer(renderContext);
		
		// デバッグ描画処理を実行する。
		g_k2EngineLow->DebubDrawWorld();

		// メインレンダリングターゲットをクリア
		renderContext.ClearRenderTargetView(g_mainRenderTarget.GetMainRenderTarget());


		// フレームの終了時に呼び出す必要がある処理を実行。
		g_k2EngineLow->EndFrame();
	}

	delete g_k2EngineLow;

	return 0;
}
