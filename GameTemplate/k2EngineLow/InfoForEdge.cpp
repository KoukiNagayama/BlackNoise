#include "k2EngineLowPreCompile.h"
#include "InfoForEdge.h"

namespace nsK2EngineLow {
	void InfoForEdge::InitForSound(
		int number,
		Vector3 position,
		float range,
		int  isSound,
		float rate,
		int color
	)
	{
		m_cbSoundSource.s_soundSourceData[number].s_position = position;
		m_cbSoundSource.s_soundSourceData[number].s_range = range;
		m_cbSoundSource.s_soundSourceData[number].s_isSound = isSound;
		m_cbSoundSource.s_soundSourceData[number].s_rate = rate;
		m_cbSoundSource.s_soundSourceData[number].s_color = color;
		m_cbSoundSource.s_numSoundSource += 1;
	}

	void InfoForEdge::Update()
	{
		if (m_sizeChenge == 0) {
			m_colorRatio += 0.01f;
			if (m_colorRatio >= 1.0f) {
				m_sizeChenge = 1;
			}
		}
		else if (m_sizeChenge == 1) {
			m_colorRatio -= 0.01f;
			if (m_colorRatio <= 0.0f) {
				m_sizeChenge = 0;
			}
		}
		m_cbSoundSource.s_colorRatio = m_colorRatio;
	}
	CBSoundSource g_cbSoundSource;
	InfoForEdge	 g_infoForEdge;
}
