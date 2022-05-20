#pragma once
#include "sound/SoundSource.h"
#include "LevelRender.h"

class Player;
class GameCamera;
class BackGround;
class Bell;
class Toy;
class Enemy;
class Gramophone;
class Record;
class Item;
class Door;
class BrokenDoor;
class Hammer;
class GroundFloor;
class FloorGimmick;
class Crowbar;
class UI;

class GameStart : public IGameObject
{
public:
	GameStart();
	~GameStart();
	bool Start();
	void Update();

	/// <summary>
	/// àÍäKÇ∆êÿÇËë÷Ç¶
	/// </summary>
	void SwitchFirstFloor();
private:
	LevelRender m_levelRender;
	Player* m_player = nullptr;
	GameCamera* m_gamecam = nullptr;
	BackGround* m_bg = nullptr;
	Bell* m_bell = nullptr;
	Toy* m_toy = nullptr;
	Enemy* m_enemy = nullptr;
	BrokenDoor* m_brokendoor = nullptr;
	Hammer* m_hammer = nullptr;
	std::vector<Door*>			m_door;
	std::vector<Gramophone*>	m_gramophone;
	std::vector<Record*>		m_record;
	Item* m_item = nullptr;;
	GroundFloor* m_groundfloor = nullptr;
	FloorGimmick* m_floorgimmick = nullptr;
	Crowbar* m_crowbar = nullptr;
	UI* m_ui = nullptr;

	Vector3 m_switchPos;
};

