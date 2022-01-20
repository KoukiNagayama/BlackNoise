#pragma once
class Game : public IGameObject
{
public:
	Game();
	~Game();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
private:
	// �A�j���[�V�����N���b�v
	enum EnAnimationClip {
		enAnimClip_Idle,					// �ҋ@�A�j���[�V����
		enAnimClip_Run,						// ����A�j���[�V����
		enAnimClip_Num,						// �A�j���[�V�����N���b�v�̐�
	};
	ModelRender m_modelRender;				// ���f�������_�[
	Vector3		m_position;					// ���W
	Vector3		m_scale = g_vec3One;		// �g�嗦
	Quaternion	m_rotation;					// ��]
	Animation   m_animation;				// �A�j���[�V����
	AnimationClip m_animationClipArray[enAnimClip_Num];	// �A�j���[�V�����N���b�v
};

