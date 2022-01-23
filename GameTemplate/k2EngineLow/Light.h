#pragma once
namespace nsK2EngineLow {
	class Light
	{
		// ライト
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
			Vector3 eyePos;			// 視点の位置
			float pad3;
			Vector3 ambientLight;   // アンビエントライト
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


