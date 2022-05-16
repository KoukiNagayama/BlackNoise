#include "k2EngineLowPreCompile.h"
#include "CreatingMaps.h"

namespace nsK2EngineLow {
	void CreatingMaps::Init()
	{
		InitRenderTargetForDepthValueMap();

		InitRenderTargetForWorldCoordinateMap();

		InitRenderTargetForNormalMap();

		InitCamera();
	}

	void CreatingMaps::InitRenderTargetForDepthValueMap()
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

	void CreatingMaps::InitRenderTargetForWorldCoordinateMap()
	{
		float clearColor[4] = { 3000.0f, 3000.0f, 3000.0f, 1.0f };
		m_worldCoordinateMap.Create(
			1600,
			900,
			1,
			1,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_D32_FLOAT,
			clearColor
		);
	}

	void CreatingMaps::InitRenderTargetForNormalMap()
	{
		m_normalMap.Create(
			1600,
			900,
			1,
			1,
			DXGI_FORMAT_R16G16B16A16_FLOAT,
			DXGI_FORMAT_D32_FLOAT
		);
	}

	void CreatingMaps::InitCamera()
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
		m_camera.SetFar(10000.0f);
		// 近平面
		m_camera.SetNear(5.0f);
		// ライトビュープロジェクション行列の計算
		m_camera.Update();
	}

	void CreatingMaps::Update()
	{
		// カメラの位置(ライトの位置)
		m_camera.SetPosition(g_camera3D->GetPosition());
		// カメラの注視点
		m_camera.SetTarget(g_camera3D->GetTarget());
		// ライトビュープロジェクション行列の計算
		m_camera.Update();
	}

	void CreatingMaps::RenderToDepthValueMap(RenderContext& rc)
	{
		RenderTarget* rts[] = {
			&m_depthValueMap,
			&m_worldCoordinateMap,
			&m_normalMap
		};

		// レンダリングターゲットを変更する。
		rc.WaitUntilToPossibleSetRenderTargets(3, rts);
		rc.SetRenderTargetsAndViewport(3, rts);
		rc.ClearRenderTargetViews(3, rts);

		for (auto& model : m_modelArray) {
			// モデルを描画。
			model->Draw(rc, m_camera);
		}

		m_modelArray.clear();

		// 書き込み完了待ち。
		rc.WaitUntilFinishDrawingToRenderTargets(3, rts);

		// レンダリングターゲットをフレームバッファに変更する。
		rc.SetRenderTarget(
			g_graphicsEngine->GetCurrentFrameBuffuerRTV(),
			g_graphicsEngine->GetCurrentFrameBuffuerDSV()
		);
		rc.SetViewportAndScissor(g_graphicsEngine->GetFrameBufferViewport());
	}

	CreatingMaps g_creatingMaps;
}
