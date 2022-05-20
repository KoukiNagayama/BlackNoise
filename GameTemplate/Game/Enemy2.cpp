#include "stdafx.h"
#include "Enemy2.h"
#include "EnemyPath.h"
#include "Player.h"
#include "Bell.h"

namespace
{
	const float WALK_SPEED = 6.5f;					// 歩く速さ
	const float RUN_SPEED = 9.5f;					// 走る速さ
	const float SEARCH_RANGE_CAUTION = 1500.0f;		// 警戒時に音が聞こえる範囲
	const float SEARCH_RANGE_NORMAL = 800.0f;		// 通常時に音が聞こえる範囲
	const float SCREAM_VOLUME = 1.0f;				// 咆哮の音量
	const float SCREAM_RANGE = 1300.0f;				// 咆哮時に輪郭線が適用される範囲
	const float EDGE_FADE_IN_DELTA_VALUE = 0.07f;	// エッジがフェードインするときの変位量
	const float EDGE_FADE_OUT_DELTA_VALUE = 0.01f;	// エッジがフェードアウトするときの変位量
	const float RATE_BY_TIME_MAX_VALUE = 1.00f;		// 時間による影響率の最大値
	const float RATE_BY_TIME_MIN_VALUE = 0.00f;		// 時間による影響率の最小値
	const float MINIMUM_CHASE_TIME = 2.0f;			// 最低限追跡する時間
	const float ENEMY_RADIUS = 30.0f;				// エネミーの半径
	const float ENEMY_HEIGHT = 200.0f;				// エネミーの高さ
}

bool Enemy2::Start()
{

	// アニメーション
	// 歩き時のアニメーションをロード
	m_animationClips[enAnimationClip_Walk].Load("Assets/animData/enemy/walk.tka");
	m_animationClips[enAnimationClip_Walk].SetLoopFlag(true);
	// 走り時のアニメーションをロード
	m_animationClips[enAnimationClip_Run].Load("Assets/animData/enemy/run.tka");
	m_animationClips[enAnimationClip_Run].SetLoopFlag(true);
	// 叫び時のアニメーションをロード
	m_animationClips[enAnimationClip_Scream].Load("Assets/animData/enemy/lookaround.tka");
	m_animationClips[enAnimationClip_Scream].SetLoopFlag(false);
	// 見回し時のアニメーションをロード
	m_animationClips[enAnimationClip_Survey].Load("Assets/animData/enemy/lookaround.tka");
	m_animationClips[enAnimationClip_Survey].SetLoopFlag(true);
	// 攻撃時のアニメーションをロード
	m_animationClips[enAnimationClip_Attack].Load("Assets/animData/enemy/attack.tka");
	m_animationClips[enAnimationClip_Attack].SetLoopFlag(false);


	// モデルの初期化
	m_modelRender.Init("Assets/modelData/enemy/enemy.tkm", m_animationClips, enAnimationClip_Num, false, enModelUpAxisZ, false, 0, 2);


	// エネミーのパス移動の情報を初期化
	std::string filePath;
	filePath = "Assets/enemypath/enemypath";
	filePath += std::to_string(m_enemyNumber);
	filePath += ".tkl";
	// 作成したファイルパスをもとに初期化
	m_enemyPath.Init(filePath.c_str());
	// 最初のパス移動のポイント情報を取得
	m_point = m_enemyPath.GetFirstPoint();


	// モデルの初期の座標、回転、拡大率を設定
	m_modelRender.SetTRS(m_position, m_rotation, m_scale);

	// プレイヤー情報を取得
	m_player = FindGO<Player>("player");
	m_bell = FindGO<Bell>("bell");

	// 音源の登録
	g_soundEngine->ResistWaveFileBank(4, "Assets/sound/enemy/heart_beat/Heart_Beat/heart_beat_caution.wav");
	g_soundEngine->ResistWaveFileBank(5, "Assets/sound/enemy/heart_beat/Heart_Beat/heart_beat_warning.wav");
	g_soundEngine->ResistWaveFileBank(6, "Assets/sound/enemy/heart_beat/Heart_Beat/heart_beat_danger.wav");
	g_soundEngine->ResistWaveFileBank(7, "Assets/sound/enemy/scream/scream.wav");

	// 輪郭線情報の初期化
	g_infoForEdge.InitForSound(9, m_position, SCREAM_RANGE, 0, m_screamRateByTime);

	// ナビゲーションメッシュの初期化
	m_nvmMesh.Init("Assets/modelData/enemy/stage2_mesh.tkn");

	return true;
}

void Enemy2::Update()
{
	// 回転
	Rotation();
	// ステート管理
	ManageState();
	// ステートによる処理
	ProcessByState();
	// アニメーション再生
	PlayAnimation();

	m_modelRender.SetPosition(m_position);

	m_modelRender.Update();
}

void Enemy2::SearchSoundOfPlayer()
{
	// 索敵範囲
	float searchRange;

	Vector3 playerPos = m_player->GetPosition();
	// プレイヤーとの距離
	Vector3 distance = m_position - playerPos;

	// 通常時
	if (m_enemyState == enEnemyState_Walk) {
		searchRange = SEARCH_RANGE_NORMAL;
	}
	// 警戒時
	else if (m_enemyState == enEnemyState_Chase) {
		searchRange = SEARCH_RANGE_CAUTION;
	}

	// ベルがなっているならば
	if (m_bell->IsRing()) {
		if (distance.Length() <= searchRange) {
			// プレイヤーを発見
			m_isFound = true;
		}
	}
}

void Enemy2::Walk()
{
	// 目的地までの距離
	Vector3 distance = m_point->s_position - m_position;

	// 目的地までの距離が近ければ
	if (distance.Length() <= 10.0f) {
		// 現在最後のポイントにいるならば
		if (m_point->s_number == m_enemyPath.GetPointListSize() - 1) {
			// 最初のポイント情報を取得
			m_point = m_enemyPath.GetFirstPoint();
		}
		// 最後以外のポイントにいるならば
		else {
			// 次のポイント情報を取得
			m_point = m_enemyPath.GetNextPoint(m_point->s_number);
		}

	}

	// 移動する方向を設定
	m_moveVector = distance;
	m_moveVector.Normalize();

	// 歩き時の移動速度を設定
	m_moveSpeed = m_moveVector * WALK_SPEED;
	
	// 座標に移動速度を加算
	m_position += m_moveSpeed;
}

void Enemy2::Rotation()
{
	// 回転角度を設定
	float angle = atan2(-m_moveVector.x, m_moveVector.z);
	
	// 回転を設定
	m_rotation.SetRotationY(-angle);
	m_modelRender.SetRotation(m_rotation);
}

void Enemy2::ManageState()
{
	switch (m_enemyState) {
	// 歩き状態
	case enEnemyState_Walk:
		ProcessWalkStateTransition();
		break;
	// 咆哮状態
	case enEnemyState_Scream:
		ProcessScreamStateTransition();
		break;
	// 追跡状態
	case enEnemyState_Chase:
		ProcessChaseStateTransition();
		break;
	// 攻撃状態
	case enEnemyState_Attack:
		//ProcessAttackStateTransition();
		break;
	}
}

void Enemy2::ProcessByState()
{
	switch (m_enemyState) {
	// 歩き状態
	case enEnemyState_Walk:
		Walk();
		SearchSoundOfPlayer();
		break;
	// 咆哮状態
	case enEnemyState_Scream:
		Scream();
		break;
	// 注意状態
	case enEnemyState_Chase:
		Chase();
		SearchSoundOfPlayer();
		break;
	}
}

void Enemy2::Scream()
{
	// 咆哮の音源を鳴らす
	if (m_screamSound == nullptr) {
		m_screamSound = NewGO<SoundSource>(0);
		m_screamSound->Init(7);
		m_screamSound->SetVolume(SCREAM_VOLUME);
		m_screamSound->Play(false);
	}

	// 咆哮による輪郭線への影響
	OutlineByScream();
}

void Enemy2::OutlineByScream()
{
	int check;

	if (m_screamSound != nullptr) {
		// 音源が再生中
		if (m_screamSound->IsPlaying() == true)
		{
			check = 1;
			// 影響率を増やす
			if (m_screamRateByTime < RATE_BY_TIME_MAX_VALUE) {
				m_screamRateByTime += EDGE_FADE_IN_DELTA_VALUE;
			}
		}
		// 音源が再生されていない時
		else {
			check = 0;
			// 影響率を減らす
			if (m_screamRateByTime > RATE_BY_TIME_MIN_VALUE && check == 0) {
				m_screamRateByTime -= EDGE_FADE_OUT_DELTA_VALUE;
				// 影響率を最低数値以下にならないように固定
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
	// ターゲットとなるプレイヤーの座標を取得
	Vector3	playerPos = m_player->GetPosition();
	// 指定した座標への移動が完了したか
	bool isEnd;
	// 移動直前の座標を記録
	m_lastPosition = m_position;

	// 最低追跡する残り時間
	// 追跡する残り時間を減らす
	m_chaseTime -= 1.0 * g_gameTime->GetFrameDeltaTime();
	// 追跡する残り時間が0になったなら
	if (m_chaseTime < 0.0f) {
		m_chaseTime = 0.0f;
	}

	// プレイヤーまでのパスを検索
	m_pathFinding.Execute(
		m_path,							
		m_nvmMesh,						
		m_position,						
		playerPos,						
		PhysicsWorld::GetInstance(),	
		ENEMY_RADIUS,					
		ENEMY_HEIGHT					
	);
	// 設定されたパスをもとに移動
	m_position = m_path.Move(
		m_position,
		RUN_SPEED,
		isEnd
	);

	// 直前の座標と現在の座標を比較して移動した方向を求める
	Vector3 distance = m_position - m_lastPosition;
	distance.Normalize();

	// 移動する方向を設定
	m_moveVector = distance;

}

void Enemy2::ProcessWalkStateTransition()
{
	// 敵を発見していないならば
	if (m_isFound == false) {
		return;
	}
	// ステートを咆哮状態にする
	m_enemyState = enEnemyState_Scream;
}

void Enemy2::ProcessScreamStateTransition()
{
	// 咆哮のアニメーションが再生中ならば
	if (m_screamRateByTime > 0.0f) {
		return;
	}
	// 最低限追跡する時間を指定
	m_chaseTime = MINIMUM_CHASE_TIME;
	// ステートを注意状態にする
	m_enemyState = enEnemyState_Chase;
}

void Enemy2::ProcessChaseStateTransition()
{
	// 敵を追跡する状態が維持されているならば
	if (m_chaseTime > 0.0f) {
		return;
	}
	// プレイヤーを攻撃可能な距離ならば
	if (m_isAttackable == true) {
		//m_enemyState = enEnemyState_Attack;
	}
	// プレイヤーを見失っていたならば
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
	// 歩き
	case enEnemyState_Walk:
		m_modelRender.PlayAnimation(enAnimationClip_Walk, 0.2f);
		break;
	// 咆哮
	case enEnemyState_Scream:
		m_modelRender.PlayAnimation(enAnimationClip_Scream, 0.2f);
		break;
	// 追跡
	case enEnemyState_Chase:
		m_modelRender.PlayAnimation(enAnimationClip_Run, 0.2f);
		break;
	// 攻撃
	case enEnemyState_Attack:
		m_modelRender.PlayAnimation(enAnimationClip_Attack, 0.2f);
		break;
	}
}

void Enemy2::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}
