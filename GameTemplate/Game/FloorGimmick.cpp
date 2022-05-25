#include "stdafx.h"
#include "FloorGimmick.h"
#include "GameCamera.h"
#include "Item.h"

namespace {
	const float DISTANCE = 120.0f;					//ドアとの距離
	const float EDGE_FADE_IN_DELTA_VALUE = 0.07f;	// エッジがフェードインするときの変位量
	const float EDGE_FADE_OUT_DELTA_VALUE = 0.005f;	// エッジがフェードアウトするときの変位量
	const float RATE_BY_TIME_MAX_VALUE = 1.00f;		// 時間による影響率の最大値
	const float RATE_BY_TIME_MIN_VALUE = 0.00f;		// 時間による影響率の最小値
	const float SOUND_RANGE = 900.0f;				//影響する範囲
}

FloorGimmick::~FloorGimmick()
{
	g_infoForEdge.SetIsSound(5, 0);
	g_infoForEdge.SetRate(5, 0.00f);
	DeleteGO(m_sound);
}

bool FloorGimmick::Start()
{
	//モデルを読み込む。
	m_modelRender.Init("Assets/modelData/stage/wood1.tkm");
	//座標を設定する。
	m_modelRender.SetTRS(m_position, m_rotation, m_scale);
	//モデルの更新。
	m_modelRender.Update();
	//当たり判定を作成する。
	m_physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());
	//ゲームカメラ
	m_gamecam = FindGO<GameCamera>("gamecamera");
	//アイテム
	m_item = FindGO<Item>("item");
	//破壊音の読み込み
	g_soundEngine->ResistWaveFileBank(9, "Assets/sound/door/door_break.wav");
	//輪郭線描写の初期化
	g_infoForEdge.InitForSound(5, m_position, SOUND_RANGE, 0, m_rateByTime);


	return true;
}

void FloorGimmick::CreatePhysicsObject()
{
	m_physicsStaticObject.CreateFromModel(m_modelRender.GetModel(), m_modelRender.GetModel().GetWorldMatrix());
}

void FloorGimmick::ReleasePhysicsObject()
{
	m_physicsStaticObject.Release();
}

void FloorGimmick::Update()
{
	NearGimmick();

	m_modelRender.Update();
}

void FloorGimmick::MakeSound()
{
	if (m_sound != nullptr)
	{
		DeleteGO(m_sound);
	}
	m_sound = NewGO<SoundSource>(9);
	m_sound->Init(9);
	m_sound->SetVolume(1.0f);
	m_sound->Play(false);
}

void FloorGimmick::CheckRate()
{
	int check1;
	if (m_sound != nullptr) {
		if (m_sound->IsPlaying() == true)
		{
			check1 = 1;
			if (m_rateByTime < RATE_BY_TIME_MAX_VALUE) {
				m_rateByTime += EDGE_FADE_IN_DELTA_VALUE;
			}
		}
		else {
			check1 = 0;
			if (m_rateByTime > RATE_BY_TIME_MIN_VALUE && check1 == 0) {
				m_rateByTime -= EDGE_FADE_OUT_DELTA_VALUE;
				if (m_rateByTime <= RATE_BY_TIME_MIN_VALUE) {
					m_rateByTime = RATE_BY_TIME_MIN_VALUE;
					m_sound = nullptr;
				}
			}
		}
		g_infoForEdge.SetInfoForSound(5, m_position, SOUND_RANGE, check1, m_rateByTime);
	}
}

void FloorGimmick::NearGimmick()
{
	Vector3 cameraPos = m_gamecam->GetPosition();
	cameraPos.y = m_position.y;
	Vector3 disToPlayer = m_position - cameraPos;
	//バールを持っていて、ギミックに近い時
	if (m_item->GetHaveCrowbar() != false && disToPlayer.Length() <= DISTANCE)
	{
		switch (m_attack)
		{
		case 2:
			if (g_pad[0]->IsTrigger(enButtonA))
			{
				MakeSound();
				//モデルの切り替え
				m_modelRender.Init("Assets/modelData/stage/wood2.tkm");
				m_attack -= 1;
			}
			break;
		case 1:
			if (g_pad[0]->IsTrigger(enButtonA))
			{
				ReleasePhysicsObject();
				MakeSound();
				//モデルの切り替え
				m_modelRender.Init("Assets/modelData/stage/wood.tkm");
				m_attack -= 1;
			}
			break;
		case 0:
			break;
		default:
			break;
		}
	}
}

void FloorGimmick::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}