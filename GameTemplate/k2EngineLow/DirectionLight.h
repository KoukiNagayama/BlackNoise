#pragma once
namespace nsK2EngineLow {
	class DirectionLight
	{
		struct SDirectionLight {
			Vector3 ligDirection;	// ライトの方向
			float   pad;
			Vector3 ligColor;		// ライトのカラー
			Vector3 eyePos;
		};
	public:
		void Init();
		SDirectionLight* GetDirecitionLigData()
		{
			return &m_directionLig;
		}

	private:
		// ディレクションライトのデータを作成
		SDirectionLight m_directionLig;
	};

	extern DirectionLight g_directionLight;
}




