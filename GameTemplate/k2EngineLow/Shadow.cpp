#include "k2EngineLowPreCompile.h"
#include "Shadow.h"

namespace nsK2EngineLow {
	Shadow::Shadow()
	{

	}
	void Shadow::Init(const char* filePath)
	{
		InitRenderTargetForShadowMap();

		InitLightCamera();

	//	InitModelForShadowMap(filePath);
	}

	void Shadow::InitRenderTargetForShadowMap()
	{
		float clearColor[4] = { 1.0f,1.0f,1.0f,1.0f };
		m_shadowMap.Create(
			1024,
			1024,
			1,
			1,
			DXGI_FORMAT_R8G8B8A8_UNORM,
			DXGI_FORMAT_D32_FLOAT,
			clearColor
		);
	}

	void Shadow::InitLightCamera()
	{
		// カメラの位置(ライトの位置)
		m_lightCamera.SetPosition(0, 600, 0);
		// カメラの注視点
		m_lightCamera.SetTarget(0, 0, 0);
		// カメラの上方向
		m_lightCamera.SetUp(1, 0, 0);
	    // 画角
		m_lightCamera.SetViewAngle(Math::DegToRad(20.0f));
		// ライトビュープロジェクション行列の計算
		m_lightCamera.Update();
	}

	void Shadow::InitModelForShadowMap(const char* filePath)
	{
		/*ModelInitData shadowModelInitData;

		// シェーダーファイルのファイルパスを指定する。
		shadowModelInitData.m_fxFilePath = "Assets/shader/shadowMap.fx";
		// tkmファイルのファイルパスを指定する。
		shadowModelInitData.m_tkmFilePath = filePath;

		// シャドウマップ描画用のモデルを初期化
		/*Model shadowModel;
		shadowModel.Init(shadowModelInitData);
		shadowModel.UpdateWorldMatrix(
			{ 0,50,0 },
			g_quatIdentity,
			g_vec3One
		);

		m_shadowModelArray.push_back(shadowModel);*/
	}

	void Shadow::Render(RenderContext& rc)
	{
		
		//レンダリングターゲットをシャドウマップに変更する。
		rc.WaitUntilToPossibleSetRenderTarget(m_shadowMap);
		rc.SetRenderTargetAndViewport(m_shadowMap);
		rc.ClearRenderTargetView(m_shadowMap);

		for (auto& model : m_shadowModelArray) {
			//影モデルを描画。
			model->Draw(rc, m_lightCamera);
		}

		//書き込み完了待ち。
		rc.WaitUntilFinishDrawingToRenderTarget(m_shadowMap);

		// レンダリングターゲットをフレームバッファに変更する。
		rc.SetRenderTarget(
			g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
			g_graphicsEngine->GetCurrentFrameBuffuerDSV()
		);
		rc.SetViewportAndScissor(g_graphicsEngine->GetFrameBufferViewport());

		//m_copyToFrameBufferSprite.Draw(rc);
		
	}
	Shadow g_shadow;
}

