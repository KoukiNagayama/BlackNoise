#pragma once
namespace nsK2EngineLow {
	class Light
	{
		// ライト構造体
		struct SLight {
			// ディレクションライト用のメンバ
			Vector3 dirDirection;	// 方向
			float   pad0;			// パディング
			Vector3 dirColor;		// カラー
			float   pad1;

			// ポイントライト用のメンバ
			Vector3 ptPosition;		// 座標 
			float pad2;
			Vector3 ptColor;		// カラー
			float ptRange;			// 影響範囲
			
			// スポットライト用のメンバ
			Vector3 spPosition;		// 座標
			float pad3;
			Vector3 spColor;		// カラー
			float spRange;			// 影響範囲
			Vector3 spDirection;	// 射出方向
			float spAngle;			// 射出角度   */
			Vector3 eyePos;			// 視点の位置
			float pad4;
			Vector3 ambientLight;   // アンビエントライト
			float pad5;

			// リムライト用のメンバ
			Vector3 limDirection;
			float pad6;
			Vector3 limColor;
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
		/// ディレクションライト初期化
		/// </summary>
		void InitDirectionLight();
		/// <summary>
		/// ポイントライト初期化
		/// </summary>
		void InitPointLight();
		/// <summary>
		/// スポットライト初期化
		/// </summary>
		void InitSpotLight();
		/// <summary>
		/// リムライト初期化
		/// </summary>
		void InitLimLight();
		/// <summary>
		/// 環境光初期化
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


