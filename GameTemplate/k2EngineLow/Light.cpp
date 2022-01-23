#include "k2EngineLowPreCompile.h"
#include "Light.h"

namespace nsK2EngineLow {
	void Light::Init()
	{
		// ディレクションライト
		// ライトの方向
		m_light.dirDirection.x = 1.0f;
		m_light.dirDirection.y = -1.0f;
		m_light.dirDirection.z = -1.0f;
		// 正規化
		m_light.dirDirection.Normalize();
		// カラー
		m_light.dirColor.x = 0.5f;
		m_light.dirColor.y = 0.5f;
		m_light.dirColor.z = 0.5f;

		// ポイントライト
		// 座標
		m_light.ptPosition.x = 0.0f;
		m_light.ptPosition.y = 50.0f;
		m_light.ptPosition.z = 50.0f;
		// カラー
		m_light.ptColor.x = 15.0f;
		m_light.ptColor.y = 0.0f;
		m_light.ptColor.z = 0.0f;
		// 影響範囲
		m_light.ptRange = 100.0f;

		// 視点の位置
		m_light.eyePos = g_camera3D->GetPosition();

		// 環境光
		m_light.ambientLight.x = 0.3f;
		m_light.ambientLight.y = 0.3f;
		m_light.ambientLight.z = 0.3f;
	}

	void Light::Update()
	{
		m_light.ptPosition = m_ptPosition;
	}
	Light g_light;
}