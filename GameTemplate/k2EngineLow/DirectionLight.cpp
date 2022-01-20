#include "k2EngineLowPreCompile.h"
#include "DirectionLight.h"

namespace nsK2EngineLow {
	void DirectionLight::Init()
	{
		// ���C�g�͎΂ߏォ�瓖�����Ă���
		m_directionLig.ligDirection.x = 1.0f;
		m_directionLig.ligDirection.y = -1.0f;
		m_directionLig.ligDirection.z = -1.0f;

		// ���K������
		m_directionLig.ligDirection.Normalize();

		//Ctrl+H
		//���C�g�̃J���[
		m_directionLig.ligColor.x =2.2f;
		m_directionLig.ligColor.y = 2.2f;
		m_directionLig.ligColor.z = 2.2f;

	}
	DirectionLight g_directionLight;
}
