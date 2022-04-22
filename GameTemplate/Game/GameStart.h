#pragma once
#include "sound/SoundSource.h"
#include "LevelRender.h"

class Player;
class GameCamera;
class BackGround;
class Bell;
class Toy;
class Key;
class Skey;
class Enemy;
class Gramophone;
class Record;
class Item;

class GameStart : public IGameObject
{
public:
	GameStart();
	~GameStart();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
private:
	LevelRender m_levelRender;
	Player* m_player;
	GameCamera* m_gamecam;
	BackGround* m_bg;
	Bell* m_bell;
	Toy* m_toy;
	Key* m_key;
	Skey* m_skey;
	Enemy* m_enemy;
	//Record*	m_record;
	std::vector<Gramophone*>	m_gramophone;
	std::vector<Record*>		m_record;
	Item* m_item;
};

