#include "k2EngineLowPreCompile.h"
#include "DirectionLight.h"

namespace nsK2EngineLow {
	void DirectionLight::Init()
	{
		// ライトは斜め上から当たっている
		m_directionLig.ligDirection.x = 1.0f;
		m_directionLig.ligDirection.y = -1.0f;
		m_directionLig.ligDirection.z = -1.0f;

		// 正規化する
		m_directionLig.ligDirection.Normalize();

		//Ctrl+H
		//ライトのカラー
		m_directionLig.ligColor.x = 1.0f;
		m_directionLig.ligColor.y = 1.0f;
		m_directionLig.ligColor.z = 1.0f;

		m_directionLig.eyePos = g_camera3D->GetPosition();

	}
	DirectionLight g_directionLight;
}
