#include "k2EngineLowPreCompile.h"
#include "PointLight.h"

namespace nsK2EngineLow {
	void PointLight::Init()
	{
		// ���C�g�̍��W
		m_pointLig.ptPosition.x = 0.0f;
		m_pointLig.ptPosition.y = 50.0f;
		m_pointLig.ptPosition.z = 50.0f;

		// ���C�g�̃J���[
		m_pointLig.ptColor.x = 15.0f;
		m_pointLig.ptColor.y = 0.0f;
		m_pointLig.ptColor.z = 0.0f;

		// ���C�g�̉e���͈�
		m_pointLig.ptRange = 100.0f;
	}
	PointLight g_pointLight;
}
