#pragma once
namespace nsK2EngineLow {
	class Light
	{
		// ���C�g�\����
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
			
			// �X�|�b�g���C�g�p�̃����o
			Vector3 spPosition;		// ���W
			float pad3;
			Vector3 spColor;		// �J���[
			float spRange;			// �e���͈�
			Vector3 spDirection;	// �ˏo����
			float spAngle;			// �ˏo�p�x
			Vector3 eyePos;			// ���_�̈ʒu
			Vector3 ambientLight;   // �A���r�G���g���C�g
		};

	public:
		void Init();

		void Update();
		SLight* GetLightData()
		{
			return &m_light;
		}
		void SetPointLightPosition(const Vector3& position)
		{
			m_ptPosition = position;
		}
		void SetSpotLightPosition(const Vector3& position)
		{
			m_spPosition = position;
		}
		void SetRotationY(const Quaternion& qRotY)
		{
			m_qRotY = qRotY;
		}
		void SetRotationX(const Quaternion& qRotX)
		{
			m_qRotX = qRotX;
		}

	private:
		/// <summary>
		/// �f�B���N�V�������C�g������
		/// </summary>
		void InitDirectionLight();
		/// <summary>
		/// �|�C���g���C�g������
		/// </summary>
		void InitPointLight();
		/// <summary>
		/// �X�|�b�g���C�g������
		/// </summary>
		void InitSpotLight();
		/// <summary>
		/// ����������
		/// </summary>
		void InitAmbientLight();
		SLight m_light;
		Vector3 m_ptPosition;
		Vector3 m_spPosition;
		Quaternion m_qRotY;
		Quaternion m_qRotX;
		Vector3	m_rotAxis;
	};

	extern Light g_light;
}


