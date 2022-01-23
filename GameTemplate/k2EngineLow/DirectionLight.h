#pragma once
namespace nsK2EngineLow {
	class DirectionLight
	{
		struct SDirectionLight {
			Vector3 ligDirection;	// ���C�g�̕���
			float   pad;
			Vector3 ligColor;		// ���C�g�̃J���[
			Vector3 eyePos;
		};
	public:
		void Init();
		SDirectionLight* GetDirecitionLigData()
		{
			return &m_directionLig;
		}

	private:
		// �f�B���N�V�������C�g�̃f�[�^���쐬
		SDirectionLight m_directionLig;
	};

	extern DirectionLight g_directionLight;
}




