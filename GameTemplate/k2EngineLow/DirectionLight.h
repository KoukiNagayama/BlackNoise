#pragma once
namespace nsK2EngineLow {
	class DirectionLight
	{
		struct DirectionalLight {
			Vector3 ligDirection;	// ライトの方向
			float   pad;
			Vector3 ligColor;		// ライトのカラー
		};
	public:
		void Init();
		DirectionalLight* GetDirecitionLigData()
		{
			return &m_directionLig;
		}

	private:
		// ディレクションライトのデータを作成
		DirectionalLight m_directionLig;
	};

	extern DirectionLight g_directionLight;
}




