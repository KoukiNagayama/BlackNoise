#include "k2EngineLowPreCompile.h"
#include "Light.h"

namespace nsK2EngineLow {
	void Light::Init()
	{
		// ディレクションライト
		InitDirectionLight();

		// ポイントライト
		InitPointLight();

		// スポットライト
		InitSpotLight();

		// リムライト
		InitLimLight();

		// 視点の位置
		m_light.eyePos = g_camera3D->GetPosition();

		// 環境光
		InitAmbientLight();
	}

	void Light::InitDirectionLight()
	{
		// ライトの方向
		m_light.dirDirection.x = 0.0f;
		m_light.dirDirection.y = 0.0f;
		m_light.dirDirection.z = -1.0f;
		// 正規化
		m_light.dirDirection.Normalize();
		// カラー
		m_light.dirColor.x = 0.8f;
		m_light.dirColor.y = 0.8f;
		m_light.dirColor.z = 0.8f;
	}

	void Light::InitPointLight()
	{
		// 座標
		m_light.ptPosition.x = 0.0f;
		m_light.ptPosition.y = 50.0f;
		m_light.ptPosition.z = 50.0f;
		// カラー
		m_light.ptColor.x = 0.0f;
		m_light.ptColor.y = 0.0f;
		m_light.ptColor.z = 0.0f;
		// 影響範囲
		m_light.ptRange = 100.0f;
	}

	void Light::InitSpotLight()
	{
		// 座標
		m_light.spPosition.x = 0.0f;
		m_light.spPosition.z = 0.0f;
		m_light.spPosition.y = 50.0f;
		// カラー
		m_light.spColor.x = 10.0f;
		m_light.spColor.y = 10.0f;
		m_light.spColor.z = 10.0f;
		// 射出方向
		m_light.spDirection.x = 1.0f;
		m_light.spDirection.y = -1.0f;
		m_light.spDirection.z = -1.0f;
		m_light.spDirection.Normalize();
		// 射出範囲
		m_light.spRange = 400.0f;
		// 射出角度
		m_light.spAngle = Math::DegToRad(20.0f);
	}

	void Light::InitLimLight()
	{
		// 射出方向
		m_light.limDirection.x = 0.0f;
		m_light.limDirection.y = 0.0f;
		m_light.limDirection.z = 1.0f;
		m_light.limDirection.Normalize();
		// カラー
		m_light.limColor.x = 0.5f;
		m_light.limColor.y = 0.5f;
		m_light.limColor.z = 0.5f;
	}

	void Light::InitAmbientLight()
	{
		m_light.ambientLight.x = 0.5f;
		m_light.ambientLight.y = 0.5f;
		m_light.ambientLight.z = 0.5f;
	}

	void Light::Update()
	{
		m_light.spPosition = m_spPosition;

		/*Quaternion qRotY;
		qRotY.SetRotationY(g_pad[0]->GetRStickXF() * 0.01f);
		//計算したクォータニオンでライトの方向を回す。
		qRotY.Apply(m_light.spDirection);

		//X軸周りの回転クォータニオンを計算する。
		Vector3 rotAxis;
		rotAxis.Cross(g_vec3AxisY, m_light.spDirection);*/
		/*Quaternion qRotY;
		qRotY.SetRotation(g_vec3AxisY, g_pad[0]->GetRStickXF() * 0.02f);
		//計算したクォータニオンでライトの方向を回す。
		qRotY.Apply(m_light.limDirection);*/

		//m_light.dirColor.x += g_pad[0]->GetLStickXF() * 0.5f;
		//m_light.dirColor.y += g_pad[0]->GetLStickXF() * 0.5f;
		//m_light.dirColor.z += g_pad[0]->GetLStickXF() * 0.5f;

	}
	Light g_light;
}