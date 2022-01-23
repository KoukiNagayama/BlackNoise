#pragma once
namespace nsK2EngineLow {
	class Light
	{
		// ���C�g
		struct SLight {
			// �f�B���N�V�������C�g�p�̃����o
			Vector3 dirDirection;	// ����
			float   pad0;			// �p�f�B���O
			Vector3 dirColor;		// �J���[
			float   pad1;

			// �|�C���g���C�g�p�̃����o
			Vector3 ptPosition;		// ���W 
			float pad2;
			Vector3 ptColor;		// �J���[
			float ptRange;			// �e���͈�
			Vector3 eyePos;			// ���_�̈ʒu
			float pad3;
			Vector3 ambientLight;   // �A���r�G���g���C�g
		};

	public:
		void Init();
		void Update();
		SLight* GetLightData()
		{
			return &m_light;
		}
		void SetPosition(const Vector3& position)
		{
			m_ptPosition = position;
		}

	private:
		SLight m_light;
		Vector3 m_ptPosition;
	};

	extern Light g_light;
}


