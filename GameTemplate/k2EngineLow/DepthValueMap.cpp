#include "k2EngineLowPreCompile.h"
#include "DepthValueMap.h"

namespace nsK2EngineLow {
	void DepthValueMap::Init()
	{
		InitRenderTargetForDepthValueMap();

		InitCamera();
	}

	void DepthValueMap::InitRenderTargetForDepthValueMap()
	{
		m_depthValueMap.Create(
			1600,
			900,
			1,
			1,
			DXGI_FORMAT_R32_FLOAT,
			DXGI_FORMAT_D32_FLOAT
		);
	}

	void DepthValueMap::InitCamera()
	{
		// カメラの位置(ライトの位置)
		m_camera.SetPosition(g_camera3D->GetPosition());
		// カメラの注視点
		m_camera.SetTarget(g_camera3D->GetTarget());
		// カメラの上方向
		m_camera.SetUp(g_camera3D->GetUp());
		// 画角
		m_camera.SetViewAngle(g_camera3D->GetViewAngle());
		// 遠平面
		m_camera.SetFar(1000.0f);
		// 近平面
		m_camera.SetNear(5.0f);
	}

	void DepthValueMap::Update()
	{
		// カメラの位置(ライトの位置)
		m_camera.SetPosition(g_camera3D->GetPosition());
		// カメラの注視点
		m_camera.SetTarget(g_camera3D->GetTarget());
		// ライトビュープロジェクション行列の計算
		m_camera.Update();
	}

	void DepthValueMap::RenderToDepthValueMap(RenderContext& rc)
	{
		// レンダリングターゲットを深度値マップに変更する。
		rc.WaitUntilToPossibleSetRenderTarget(m_depthValueMap);
		rc.SetRenderTargetAndViewport(m_depthValueMap);
		rc.ClearRenderTargetView(m_depthValueMap);

		for (auto& model : m_modelArray) {
			// モデルを描画。
			model->Draw(rc, m_camera);
		}

		m_modelArray.clear();

		// 書き込み完了待ち。
		rc.WaitUntilFinishDrawingToRenderTarget(m_depthValueMap);

		// レンダリングターゲットをフレームバッファに変更する。
		rc.SetRenderTarget(
			g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
			g_graphicsEngine->GetCurrentFrameBuffuerDSV()
		);
		rc.SetViewportAndScissor(g_graphicsEngine->GetFrameBufferViewport());
	}

	DepthValueMap g_depthValueMap;
}
