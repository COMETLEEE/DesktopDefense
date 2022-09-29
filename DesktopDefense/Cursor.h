#pragma once

#include "Weapon.h"
#define CURSOR_DAMAGE 5

class Cursor : public Weapon
{
public:
	Cursor(GameObject* _gameObject);

	bool AttackTick() override;

	void Attack(Miner* _miner) override;
};