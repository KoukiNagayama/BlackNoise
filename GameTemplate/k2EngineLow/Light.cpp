#include "k2EngineLowPreCompile.h"
#include "Light.h"

namespace nsK2EngineLow {
	void Light::Init()
	{
		// �f�B���N�V�������C�g
		InitDirectionLight();

		// �|�C���g���C�g
		InitPointLight();

		// �X�|�b�g���C�g
		InitSpotLight();

		// �������C�g
		InitLimLight();

		// ���_�̈ʒu
		m_light.eyePos = g_camera3D->GetPosition();

		// ����
		InitAmbientLight();
	}

	void Light::InitDirectionLight()
	{
		// ���C�g�̕���
		m_light.dirDirection.x = 0.0f;
		m_light.dirDirection.y = 0.0f;
		m_light.dirDirection.z = -1.0f;
		// ���K��
		m_light.dirDirection.Normalize();
		// �J���[
		m_light.dirColor.x = 0.8f;
		m_light.dirColor.y = 0.8f;
		m_light.dirColor.z = 0.8f;
	}

	void Light::InitPointLight()
	{
		// ���W
		m_light.ptPosition.x = 0.0f;
		m_light.ptPosition.y = 50.0f;
		m_light.ptPosition.z = 50.0f;
		// �J���[
		m_light.ptColor.x = 0.0f;
		m_light.ptColor.y = 0.0f;
		m_light.ptColor.z = 0.0f;
		// �e���͈�
		m_light.ptRange = 100.0f;
	}

	void Light::InitSpotLight()
	{
		// ���W
		m_light.spPosition.x = 0.0f;
		m_light.spPosition.z = 0.0f;
		m_light.spPosition.y = 50.0f;
		// �J���[
		m_light.spColor.x = 10.0f;
		m_light.spColor.y = 10.0f;
		m_light.spColor.z = 10.0f;
		// �ˏo����
		m_light.spDirection.x = 1.0f;
		m_light.spDirection.y = -1.0f;
		m_light.spDirection.z = -1.0f;
		m_light.spDirection.Normalize();
		// �ˏo�͈�
		m_light.spRange = 400.0f;
		// �ˏo�p�x
		m_light.spAngle = Math::DegToRad(20.0f);
	}

	void Light::InitLimLight()
	{
		// �ˏo����
		m_light.limDirection.x = 0.0f;
		m_light.limDirection.y = 0.0f;
		m_light.limDirection.z = 1.0f;
		m_light.limDirection.Normalize();
		// �J���[
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
		//�v�Z�����N�H�[�^�j�I���Ń��C�g�̕������񂷁B
		qRotY.Apply(m_light.spDirection);

		//X������̉�]�N�H�[�^�j�I�����v�Z����B
		Vector3 rotAxis;
		rotAxis.Cross(g_vec3AxisY, m_light.spDirection);*/
		/*Quaternion qRotY;
		qRotY.SetRotation(g_vec3AxisY, g_pad[0]->GetRStickXF() * 0.02f);
		//�v�Z�����N�H�[�^�j�I���Ń��C�g�̕������񂷁B
		qRotY.Apply(m_light.limDirection);*/

		//m_light.dirColor.x += g_pad[0]->GetLStickXF() * 0.5f;
		//m_light.dirColor.y += g_pad[0]->GetLStickXF() * 0.5f;
		//m_light.dirColor.z += g_pad[0]->GetLStickXF() * 0.5f;

	}
	Light g_light;
}