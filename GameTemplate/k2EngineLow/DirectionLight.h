#pragma once
namespace nsK2EngineLow {
	class DirectionLight
	{
		struct DirectionalLight {
			Vector3 ligDirection;	// ���C�g�̕���
			float   pad;
			Vector3 ligColor;		// ���C�g�̃J���[
		};
	public:
		void Init();
		DirectionalLight* GetDirecitionLigData()
		{
			return &m_directionLig;
		}

	private:
		// �f�B���N�V�������C�g�̃f�[�^���쐬
		DirectionalLight m_directionLig;
	};

	extern DirectionLight g_directionLight;
}




