#include "k2EngineLowPreCompile.h"
#include "InfoForEdge.h"

namespace nsK2EngineLow {
	void InfoForEdge::InitForSound(
		int number,
		Vector3 position,
		float range,
		int  isSound,
		float rate
	)
	{
		m_cbSoundSource.s_soundSourceData[number].s_position = position;
		m_cbSoundSource.s_soundSourceData[number].s_range = range;
		m_cbSoundSource.s_soundSourceData[number].s_isSound = isSound;
		m_cbSoundSource.s_soundSourceData[number].s_rate = rate;
		m_cbSoundSource.s_numSoundSource += 1;
	}

	void InfoForEdge::Update()
	{
		for (int i = 0; i < m_cbSoundSource.s_numSoundSource; i++)
		{
			if (m_cbSoundSource.s_soundSourceData[i].s_rate <= 0.0f) {
				continue;
			}
			else if (m_cbSoundSource.s_soundSourceData[i].s_currentRate < m_cbSoundSource.s_soundSourceData[i].s_rate) {
				m_cbSoundSource.s_soundSourceData[i].s_currentRate += 0.05f;
			}
		}
	}
	CBSoundSource g_cbSoundSource;
	InfoForEdge	 g_infoForEdge;
}
