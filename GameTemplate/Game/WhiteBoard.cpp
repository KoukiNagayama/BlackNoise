#include "stdafx.h"
#include "WhiteBoard.h"

bool WhiteBoard::Start()
{
	// ���f���̏�����
	m_modelRender.Init("Assets/modelData/whiteBoard.tkm", nullptr, 0, false, enModelUpAxisZ, false, 0, 4);
	// ���f���̍��W�A��]�A�g�嗦��ݒ�
	m_modelRender.SetTRS(m_position, m_rotation, m_scale);

	return true;
}

void WhiteBoard::Update()
{
	// ���f���̍X�V
	m_modelRender.Update();
}

void WhiteBoard::Render(RenderContext& rc)
{
	// ���f���̕`��
	m_modelRender.Draw(rc);
}