#include "k2EngineLowPreCompile.h"
#include "InfoForEdge.h"

namespace nsK2EngineLow {
	void InfoForEdge::InitForSound(
		int number,
		Vector3 position,
		float range,
		int  isSound
	)
	{
		m_cbSoundSource.s_soundSourceData[number].s_position = position;
		m_cbSoundSource.s_soundSourceData[number].s_range = range;
		m_cbSoundSource.s_soundSourceData[number].s_isSound = isSound;
		m_cbSoundSource.s_numSoundSource += 1;
	}
	CBSoundSource g_cbSoundSource;
	InfoForEdge	 g_infoForEdge;
}
