#include "stdafx.h"
#include "Enemy2.h"
#include "EnemyPath.h"
#include "Player.h"
#include "Bell.h"

namespace
{
	const float WALK_SPEED = 6.5f;					// ��������
	const float RUN_SPEED = 9.5f;					// ���鑬��
	const float SEARCH_RANGE_CAUTION = 1500.0f;		// �x�����ɉ�����������͈�
	const float SEARCH_RANGE_NORMAL = 800.0f;		// �ʏ펞�ɉ�����������͈�
	const float SCREAM_VOLUME = 1.0f;				// ���K�̉���
	const float SCREAM_RANGE = 1300.0f;				// ���K���ɗ֊s�����K�p�����͈�
	const float EDGE_FADE_IN_DELTA_VALUE = 0.07f;	// �G�b�W���t�F�[�h�C������Ƃ��̕ψʗ�
	const float EDGE_FADE_OUT_DELTA_VALUE = 0.01f;	// �G�b�W���t�F�[�h�A�E�g����Ƃ��̕ψʗ�
	const float RATE_BY_TIME_MAX_VALUE = 1.00f;		// ���Ԃɂ��e�����̍ő�l
	const float RATE_BY_TIME_MIN_VALUE = 0.00f;		// ���Ԃɂ��e�����̍ŏ��l
	const float MINIMUM_CHASE_TIME = 2.0f;			// �Œ���ǐՂ��鎞��
	const float ENEMY_RADIUS = 30.0f;				// �G�l�~�[�̔��a
	const float ENEMY_HEIGHT = 200.0f;				// �G�l�~�[�̍���
}

bool Enemy2::Start()
{

	// �A�j���[�V����
	// �������̃A�j���[�V���������[�h
	m_animationClips[enAnimationClip_Walk].Load("Assets/animData/enemy/walk.tka");
	m_animationClips[enAnimationClip_Walk].SetLoopFlag(true);
	// ���莞�̃A�j���[�V���������[�h
	m_animationClips[enAnimationClip_Run].Load("Assets/animData/enemy/run.tka");
	m_animationClips[enAnimationClip_Run].SetLoopFlag(true);
	// ���ю��̃A�j���[�V���������[�h
	m_animationClips[enAnimationClip_Scream].Load("Assets/animData/enemy/lookaround.tka");
	m_animationClips[enAnimationClip_Scream].SetLoopFlag(false);
	// ���񂵎��̃A�j���[�V���������[�h
	m_animationClips[enAnimationClip_Survey].Load("Assets/animData/enemy/lookaround.tka");
	m_animationClips[enAnimationClip_Survey].SetLoopFlag(true);
	// �U�����̃A�j���[�V���������[�h
	m_animationClips[enAnimationClip_Attack].Load("Assets/animData/enemy/attack.tka");
	m_animationClips[enAnimationClip_Attack].SetLoopFlag(false);


	// ���f���̏�����
	m_modelRender.Init("Assets/modelData/enemy/enemy.tkm", m_animationClips, enAnimationClip_Num, false, enModelUpAxisZ, false, 0, 2);


	// �G�l�~�[�̃p�X�ړ��̏���������
	std::string filePath;
	filePath = "Assets/enemypath/enemypath";
	filePath += std::to_string(m_enemyNumber);
	filePath += ".tkl";
	// �쐬�����t�@�C���p�X�����Ƃɏ�����
	m_enemyPath.Init(filePath.c_str());
	// �ŏ��̃p�X�ړ��̃|�C���g�����擾
	m_point = m_enemyPath.GetFirstPoint();


	// ���f���̏����̍��W�A��]�A�g�嗦��ݒ�
	m_modelRender.SetTRS(m_position, m_rotation, m_scale);

	// �v���C���[�����擾
	m_player = FindGO<Player>("player");
	m_bell = FindGO<Bell>("bell");

	// �����̓o�^
	g_soundEngine->ResistWaveFileBank(4, "Assets/sound/enemy/heart_beat/Heart_Beat/heart_beat_caution.wav");
	g_soundEngine->ResistWaveFileBank(5, "Assets/sound/enemy/heart_beat/Heart_Beat/heart_beat_warning.wav");
	g_soundEngine->ResistWaveFileBank(6, "Assets/sound/enemy/heart_beat/Heart_Beat/heart_beat_danger.wav");
	g_soundEngine->ResistWaveFileBank(7, "Assets/sound/enemy/scream/scream.wav");

	// �֊s�����̏�����
	g_infoForEdge.InitForSound(9, m_position, SCREAM_RANGE, 0, m_screamRateByTime);

	// �i�r�Q�[�V�������b�V���̏�����
	m_nvmMesh.Init("Assets/modelData/enemy/stage2_mesh.tkn");

	return true;
}

void Enemy2::Update()
{
	// ��]
	Rotation();
	// �X�e�[�g�Ǘ�
	ManageState();
	// �X�e�[�g�ɂ�鏈��
	ProcessByState();
	// �A�j���[�V�����Đ�
	PlayAnimation();

	m_modelRender.SetPosition(m_position);

	m_modelRender.Update();
}

void Enemy2::SearchSoundOfPlayer()
{
	// ���G�͈�
	float searchRange;

	Vector3 playerPos = m_player->GetPosition();
	// �v���C���[�Ƃ̋���
	Vector3 distance = m_position - playerPos;

	// �ʏ펞
	if (m_enemyState == enEnemyState_Walk) {
		searchRange = SEARCH_RANGE_NORMAL;
	}
	// �x����
	else if (m_enemyState == enEnemyState_Chase) {
		searchRange = SEARCH_RANGE_CAUTION;
	}

	// �x�����Ȃ��Ă���Ȃ��
	if (m_bell->IsRing()) {
		if (distance.Length() <= searchRange) {
			// �v���C���[�𔭌�
			m_isFound = true;
		}
	}
}

void Enemy2::Walk()
{
	// �ړI�n�܂ł̋���
	Vector3 distance = m_point->s_position - m_position;

	// �ړI�n�܂ł̋������߂����
	if (distance.Length() <= 10.0f) {
		// ���ݍŌ�̃|�C���g�ɂ���Ȃ��
		if (m_point->s_number == m_enemyPath.GetPointListSize() - 1) {
			// �ŏ��̃|�C���g�����擾
			m_point = m_enemyPath.GetFirstPoint();
		}
		// �Ō�ȊO�̃|�C���g�ɂ���Ȃ��
		else {
			// ���̃|�C���g�����擾
			m_point = m_enemyPath.GetNextPoint(m_point->s_number);
		}

	}

	// �ړ����������ݒ�
	m_moveVector = distance;
	m_moveVector.Normalize();

	// �������̈ړ����x��ݒ�
	m_moveSpeed = m_moveVector * WALK_SPEED;
	
	// ���W�Ɉړ����x�����Z
	m_position += m_moveSpeed;
}

void Enemy2::Rotation()
{
	// ��]�p�x��ݒ�
	float angle = atan2(-m_moveVector.x, m_moveVector.z);
	
	// ��]��ݒ�
	m_rotation.SetRotationY(-angle);
	m_modelRender.SetRotation(m_rotation);
}

void Enemy2::ManageState()
{
	switch (m_enemyState) {
	// �������
	case enEnemyState_Walk:
		ProcessWalkStateTransition();
		break;
	// ���K���
	case enEnemyState_Scream:
		ProcessScreamStateTransition();
		break;
	// �ǐՏ��
	case enEnemyState_Chase:
		ProcessChaseStateTransition();
		break;
	// �U�����
	case enEnemyState_Attack:
		//ProcessAttackStateTransition();
		break;
	}
}

void Enemy2::ProcessByState()
{
	switch (m_enemyState) {
	// �������
	case enEnemyState_Walk:
		Walk();
		SearchSoundOfPlayer();
		break;
	// ���K���
	case enEnemyState_Scream:
		Scream();
		break;
	// ���ӏ��
	case enEnemyState_Chase:
		Chase();
		SearchSoundOfPlayer();
		break;
	}
}

void Enemy2::Scream()
{
	// ���K�̉�����炷
	if (m_screamSound == nullptr) {
		m_screamSound = NewGO<SoundSource>(0);
		m_screamSound->Init(7);
		m_screamSound->SetVolume(SCREAM_VOLUME);
		m_screamSound->Play(false);
	}

	// ���K�ɂ��֊s���ւ̉e��
	OutlineByScream();
}

void Enemy2::OutlineByScream()
{
	int check;

	if (m_screamSound != nullptr) {
		// �������Đ���
		if (m_screamSound->IsPlaying() == true)
		{
			check = 1;
			// �e�����𑝂₷
			if (m_screamRateByTime < RATE_BY_TIME_MAX_VALUE) {
				m_screamRateByTime += EDGE_FADE_IN_DELTA_VALUE;
			}
		}
		// �������Đ�����Ă��Ȃ���
		else {
			check = 0;
			// �e���������炷
			if (m_screamRateByTime > RATE_BY_TIME_MIN_VALUE && check == 0) {
				m_screamRateByTime -= EDGE_FADE_OUT_DELTA_VALUE;
				// �e�������Œᐔ�l�ȉ��ɂȂ�Ȃ��悤�ɌŒ�
				if (m_screamRateByTime <= RATE_BY_TIME_MIN_VALUE) {
					m_screamRateByTime = RATE_BY_TIME_MIN_VALUE;
				}
			}
		}
		g_infoForEdge.SetIsSound(2, check);
		g_infoForEdge.SetRate(2, m_screamRateByTime);
	}
}

void Enemy2::Chase()
{
	// �^�[�Q�b�g�ƂȂ�v���C���[�̍��W���擾
	Vector3	playerPos = m_player->GetPosition();
	// �w�肵�����W�ւ̈ړ�������������
	bool isEnd;
	// �ړ����O�̍��W���L�^
	m_lastPosition = m_position;

	// �Œ�ǐՂ���c�莞��
	// �ǐՂ���c�莞�Ԃ����炷
	m_chaseTime -= 1.0 * g_gameTime->GetFrameDeltaTime();
	// �ǐՂ���c�莞�Ԃ�0�ɂȂ����Ȃ�
	if (m_chaseTime < 0.0f) {
		m_chaseTime = 0.0f;
	}

	// �v���C���[�܂ł̃p�X������
	m_pathFinding.Execute(
		m_path,							
		m_nvmMesh,						
		m_position,						
		playerPos,						
		PhysicsWorld::GetInstance(),	
		ENEMY_RADIUS,					
		ENEMY_HEIGHT					
	);
	// �ݒ肳�ꂽ�p�X�����ƂɈړ�
	m_position = m_path.Move(
		m_position,
		RUN_SPEED,
		isEnd
	);

	// ���O�̍��W�ƌ��݂̍��W���r���Ĉړ��������������߂�
	Vector3 distance = m_position - m_lastPosition;
	distance.Normalize();

	// �ړ����������ݒ�
	m_moveVector = distance;

}

void Enemy2::ProcessWalkStateTransition()
{
	// �G�𔭌����Ă��Ȃ��Ȃ��
	if (m_isFound == false) {
		return;
	}
	// �X�e�[�g����K��Ԃɂ���
	m_enemyState = enEnemyState_Scream;
}

void Enemy2::ProcessScreamStateTransition()
{
	// ���K�̃A�j���[�V�������Đ����Ȃ��
	if (m_screamRateByTime > 0.0f) {
		return;
	}
	// �Œ���ǐՂ��鎞�Ԃ��w��
	m_chaseTime = MINIMUM_CHASE_TIME;
	// �X�e�[�g�𒍈ӏ�Ԃɂ���
	m_enemyState = enEnemyState_Chase;
}

void Enemy2::ProcessChaseStateTransition()
{
	// �G��ǐՂ����Ԃ��ێ�����Ă���Ȃ��
	if (m_chaseTime > 0.0f) {
		return;
	}
	// �v���C���[���U���\�ȋ����Ȃ��
	if (m_isAttackable == true) {
		//m_enemyState = enEnemyState_Attack;
	}
	// �v���C���[���������Ă����Ȃ��
	else if (m_isFound == false) {
		//m_enemyState = enEnemyState_Walk;
	}
		
}

void Enemy2::ProcessAttackStateTransition()
{

}

void Enemy2::PlayAnimation()
{
	switch (m_enemyState) {
	// ����
	case enEnemyState_Walk:
		m_modelRender.PlayAnimation(enAnimationClip_Walk, 0.2f);
		break;
	// ���K
	case enEnemyState_Scream:
		m_modelRender.PlayAnimation(enAnimationClip_Scream, 0.2f);
		break;
	// �ǐ�
	case enEnemyState_Chase:
		m_modelRender.PlayAnimation(enAnimationClip_Run, 0.2f);
		break;
	// �U��
	case enEnemyState_Attack:
		m_modelRender.PlayAnimation(enAnimationClip_Attack, 0.2f);
		break;
	}
}

void Enemy2::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}
