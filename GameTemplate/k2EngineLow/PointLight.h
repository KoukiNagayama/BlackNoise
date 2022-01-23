#pragma once
namespace nsK2EngineLow {
	class PointLight
	{
		struct SPointLight {
			Vector3 ptPosition;
			float pad;
			Vector3 ptColor;
			float ptRange;
		};

	public:
		void Init();
		SPointLight* GetPointLigData()
		{
			return &m_pointLig;
		}
	private:
		SPointLight m_pointLig;
	};

	extern PointLight g_pointLight;
}


