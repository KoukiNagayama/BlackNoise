#include "k2EngineLowPreCompile.h"
#include "FontRender.h"
#include "graphics/font/Font.h"

namespace nsK2EngineLow {
	void FontRender::Draw(RenderContext& rc)
	{
		if (m_text == nullptr)
		{
			return;
		}

		if (m_initCheck == false) {
			// �e��p�����[�^���t�H���g�p�\���֑̂��
			swprintf_s(m_sfont.s_text, m_text);		// �e�L�X�g
			m_sfont.s_position.x = m_position.x;	// x���W
			m_sfont.s_position.y = m_position.y;	// y���W
			m_sfont.s_rotation = m_rotation;		// ��]
			m_sfont.s_color = m_color;				// �F
			m_sfont.s_scale = m_scale;				// �g��
			m_sfont.s_pivot = m_pivot;				// ��_

			// �\���̂�z��ɑ��
			m_fontArray.push_back(&m_sfont);
			m_initCheck = true;
		}

		RenderFont(rc);
	}

	void FontRender::RenderFont(RenderContext& rc)
	{
		Font font;
		int	 drawNum = 0;
		font.Begin(rc);
		for (auto& fontNum : m_fontArray) {
			
			font.Draw(m_fontArray[drawNum]->s_text,
				m_fontArray[drawNum]->s_position,
				m_fontArray[drawNum]->s_color,
				m_fontArray[drawNum]->s_rotation,
				m_fontArray[drawNum]->s_scale,
				m_fontArray[drawNum]->s_pivot
			);
			drawNum += 1;
		}
		font.End(rc);
	}
}
