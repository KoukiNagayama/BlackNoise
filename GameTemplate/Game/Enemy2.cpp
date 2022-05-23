#include "stdafx.h"
#include "Enemy2.h"
#include "EnemyPath.h"
#include "Player.h"
#include "Bell.h"
#include "GameCamera.h"

namespace
{
	const float WALK_SPEED = 6.5f;									// ��������
	const float RUN_SPEED = 9.5f;									// ���鑬��
	const float SEARCH_RANGE_TO_BELL = 1000.0f;						// �x���̉�����������͈�
	const float SEARCH_RANGE_TO_FOOTSTEP = 200.0f;					// ��������������͈�
	const float SCREAM_VOLUME = 1.0f;								// ���K�̉���
	const float SCREAM_RANGE = 1300.0f;								// ���K���ɗ֊s�����K�p�����͈�
	const float EDGE_FADE_IN_DELTA_VALUE = 0.07f;					// �G�b�W���t�F�[�h�C������Ƃ��̕ψʗ�
	const float EDGE_FADE_OUT_DELTA_VALUE = 0.01f;					// �G�b�W���t�F�[�h�A�E�g����Ƃ��̕ψʗ�
	const float RATE_BY_TIME_MAX_VALUE = 1.00f;						// ���Ԃɂ��e�����̍ő�l
	const float RATE_BY_TIME_MIN_VALUE = 0.00f;						// ���Ԃɂ��e�����̍ŏ��l
	const float MINIMUM_CHASE_TIME = 2.0f;							// �Œ���ǐՂ��鎞��
	const float ENEMY_RADIUS = 30.0f;								// �G�l�~�[�̔��a
	const float ENEMY_HEIGHT = 200.0f;								// �G�l�~�[�̍���
	const float ATTACKING_RANGE = 120.0f;							// �U���\�ȋ���
	const float DISTANCE_TO_TARGET_WHILE_WALKING = 10.0f;			// �������̖ڕW�n�_�܂ł̋���
	const float DISTANCE_TO_TARGET_WHILE_RETURNING = 30.0f;			// ���Ԓ��̖ڕW�n�_�܂ł̋���
	const float TIME_TO_LOSE_SIGHT = 5.0f;							// �v���C���[��������������
	const float INTERPOLATION_TIME_FOR_ANIMATION = 0.5f;			// �A�j���[�V�����̕�Ԏ���
	const float TIME_TO_FORCE_STATE_TRANSITION = 6.0f;				// �����I�ɃX�e�[�g�J�ڂ��鎞��
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
	m_animationClips[enAnimationClip_Attack].SetLoopFlag(true);


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
	m_gameCamera = FindGO<GameCamera>("gamecamera");

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
	// �X�e�[�g�ɂ�鏈��
	ProcessByState();
	// �X�e�[�g�Ǘ�
	ManageState();
	// �A�j���[�V�����Đ�
	PlayAnimation();
	// ���W�̍X�V
	m_modelRender.SetPosition(m_position);
	// ���f���̍X�V
	m_modelRender.Update();
}

void Enemy2::SearchSoundOfPlayer()
{
	// ���G�͈�
	float searchRange = 0.0f;

	Vector3 playerPos = m_player->GetPosition();
	// �v���C���[�Ƃ̋���
	Vector3 distance = m_position - playerPos;

	// �x�������Ă���Ȃ��
	if (m_bell->IsRing()) {
		// ���G�͈͂��x���p�ɐݒ�
		searchRange = SEARCH_RANGE_TO_BELL;
	}
	// ���������Ă���Ȃ��
	else if (m_gameCamera->IsSound()) {
		// ���G�͈͂𑫉��p�ɐݒ�
		searchRange = SEARCH_RANGE_TO_FOOTSTEP;
	}

	// �v���C���[�Ƃ̋������ݒ肳�ꂽ���G�͈͓��ł����
	if (distance.Length() <= searchRange) {
		// �v���C���[�𔭌�����
		m_isFound = true;
		return;
	}
	// �v���C���[�𔭌��ł��Ȃ�����
	m_isFound = false;
}

void Enemy2::Walk()
{
	// �ړI�n�܂ł̋���
	Vector3 distance = m_point->s_position - m_position;

	// �ړI�n�܂ł̋������߂����
	if (distance.Length() <= DISTANCE_TO_TARGET_WHILE_WALKING) {
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
	// ���񂵏��
	case enEnemyState_Survey:
		ProcessSurveyStateTransition();
		break;
	// �p�X�ړ��ւ̋A�ҏ��
	case enEnemyState_ReturnToPath:
		ProcessReturnToPathStateTransition();
		break;
	// �U�����
	case enEnemyState_Attack:
		ProcessAttackStateTransition();
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
	// �ǐՏ��
	case enEnemyState_Chase:
		Chase();
		SearchSoundOfPlayer();
		break;
	// ���񂵏��
	case enEnemyState_Survey:
		Survey();
		SearchSoundOfPlayer();
		break;
	// �p�X�ړ��ւ̋A�ҏ��
	case enEnemyState_ReturnToPath:
		ReturnToPath();
		SearchSoundOfPlayer();
		break;
	// �U�����
	case enEnemyState_Attack:

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
			else {
				// ���K���I��
				m_isEndScream = true;
			}
		}
		g_infoForEdge.SetPosition(2, m_position);
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
	m_chaseTime -= g_gameTime->GetFrameDeltaTime();
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

	// ����
	// ���O�̍��W�ƌ��݂̍��W���r���Ĉړ��������������߂�
	Vector3 momentDist = m_position - m_lastPosition;
	momentDist.Normalize();

	// �ړ����������ݒ�
	m_moveVector = momentDist;
	
	// �v���C���[�Ƃ̋���
	Vector3 distanceToPlayer = m_position - playerPos;
	// �v���C���[�Ƃ̋������߂����
	if (distanceToPlayer.Length() <= ATTACKING_RANGE) {
		// �U�����s��
		m_isAttackable = true;
	}

}

void Enemy2::Survey()
{
	// ���񂵂����Ԃ����Z
	m_surveyTimer += g_gameTime->GetFrameDeltaTime();
}

void Enemy2::ReturnToPath()
{
	// ���݂̍��W�����ԋ߂����W�̃|�C���g���擾
	m_point = m_enemyPath.GetNearPoint(m_position);

	bool isEnd;

	// �ړ����O�̍��W���L�^
	m_lastPosition = m_position;

	// �A�ҖڕW�̃p�X�܂ł̃p�X������
	m_pathFinding.Execute(
		m_path,
		m_nvmMesh,
		m_position,
		m_point->s_position,
		PhysicsWorld::GetInstance(),
		ENEMY_RADIUS,
		ENEMY_HEIGHT
	);
	// �ݒ肳�ꂽ�p�X�����ƂɈړ�
	m_position = m_path.Move(
		m_position,
		WALK_SPEED,
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
	// ���K�����Z�b�g
	m_screamSound = nullptr;
	m_screamRateByTime = 0.0f;
	m_isEndScream = false;
	// �X�e�[�g����K��Ԃɂ���
	m_enemyState = enEnemyState_Scream;
}

void Enemy2::ProcessScreamStateTransition()
{
	// ���K���I�����Ă��Ȃ��Ȃ��
	if (m_isEndScream == false) {
		return;
	}
	// �Œ���ǐՂ��鎞�Ԃ��w��
	m_chaseTime = MINIMUM_CHASE_TIME;
	// �X�e�[�g��ǐՏ�Ԃɂ���
	m_enemyState = enEnemyState_Chase;
}

void Enemy2::ProcessChaseStateTransition()
{
	// �v���C���[���U���\�ȋ����Ȃ��
	if (m_isAttackable == true) {
		m_enemyState = enEnemyState_Attack;
	}
	// �G��ǐՂ����Ԃ��ێ�����Ă���Ȃ��
	if (m_chaseTime > 0.0f || m_isFound == true) {
		return;
	}
	// �v���C���[���������Ă����Ȃ��
	if (m_isFound == false) {
		// ���񂷎��Ԃ����Z�b�g
		m_surveyTimer = 0.0f;
		// �X�e�[�g�����񂵏�Ԃɂ���
		m_enemyState = enEnemyState_Survey;
	}
		
}

void Enemy2::ProcessSurveyStateTransition()
{
	// ��莞�Ԉȓ��Ƀv���C���[�𔭌�
	if (m_isFound == true) {
		// �Œ���ǐՂ��鎞�Ԃ��w��
		m_chaseTime = MINIMUM_CHASE_TIME;
		// �ǐՂ��J�n
		m_enemyState = enEnemyState_Chase;
	}
	// ��莞�ԃv���C���[���������Ă�����
	if (m_surveyTimer > TIME_TO_LOSE_SIGHT) {
		// �A�ҏ�Ԃ̎��Ԃ��w��
		m_timeToReturn = TIME_TO_FORCE_STATE_TRANSITION;
		// �X�e�[�g���p�X�ւ̋A�ҏ�Ԃɂ���
		m_enemyState = enEnemyState_ReturnToPath;
	}

}

void Enemy2::ProcessReturnToPathStateTransition()
{
	// �ڕW�̍��W�ƌ��݂̍��W�̋����𑪂�
	Vector3 distance = m_point->s_position - m_position;

	m_timeToReturn -= g_gameTime->GetFrameDeltaTime();

	// �v���C���[�𔭌�������
	if (m_isFound == true) {
		// ���K�����Z�b�g
		m_screamRateByTime = 0.0f;
		m_isEndScream = false;
		m_screamSound = nullptr;
		// �X�e�[�g����K��Ԃɂ���
		m_enemyState = enEnemyState_Scream;
	}
	// �ڕW�̍��W�ɋ߂��Ȃ�����
	else if (distance.Length() < DISTANCE_TO_TARGET_WHILE_RETURNING) {
		// �X�e�[�g�������Ԃɂ���
		m_enemyState = enEnemyState_Walk;
	}
	// ��莞�Ԉȓ��ɋA�҂ł��Ȃ������ꍇ
	else if (m_timeToReturn <= 0.0f) {
		// �����I�ɃX�e�[�g�������Ԃɂ���
		m_enemyState = enEnemyState_Walk;
	}
}

void Enemy2::ProcessAttackStateTransition()
{
	// �Q�[���I�[�o�[
	m_isGameOver = true;
}

void Enemy2::PlayAnimation()
{
	switch (m_enemyState) {
	// ����
	case enEnemyState_Walk:
		m_modelRender.PlayAnimation(enAnimationClip_Walk, INTERPOLATION_TIME_FOR_ANIMATION);
		break;
	// ���K
	case enEnemyState_Scream:
		m_modelRender.PlayAnimation(enAnimationClip_Scream, INTERPOLATION_TIME_FOR_ANIMATION);
		break;
	// �ǐ�
	case enEnemyState_Chase:
		m_modelRender.PlayAnimation(enAnimationClip_Run, INTERPOLATION_TIME_FOR_ANIMATION);
		break;
	// ����
	case enEnemyState_Survey:
		m_modelRender.PlayAnimation(enAnimationClip_Survey, INTERPOLATION_TIME_FOR_ANIMATION);
		break;
	// �p�X�ւ̋A��
	case enEnemyState_ReturnToPath:
		m_modelRender.PlayAnimation(enAnimationClip_Walk, INTERPOLATION_TIME_FOR_ANIMATION);
		break;
	// �U��
	case enEnemyState_Attack:
		m_modelRender.PlayAnimation(enAnimationClip_Attack, INTERPOLATION_TIME_FOR_ANIMATION);
		break;
	}
}

void Enemy2::Render(RenderContext& rc)
{
	// �`��
	m_modelRender.Draw(rc);
}
