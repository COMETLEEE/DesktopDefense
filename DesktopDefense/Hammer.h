#pragma once

#include "Miner.h"
#include "Weapon.h"

#define HAMMER_DAMAGE 5

class Hammer : public Weapon
{
public:
	Hammer(GameObject* _gameObject);

	bool AttackTick() override;

	void Attack(Miner* _miner) override;
};
