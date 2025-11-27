#pragma once

class Stats
{
public:
	float HP = 100;
	float MaxHP = 100;

	float Heal = 10;
	float MaxHeal = 15;

	float Attack = 20;
	float MaxAttack = 30;

	void AttackTime(Stats& stats) // We are attacking another
	{
		stats.HP -= this->Attack;
		if (stats.HP < 0)
			stats.HP = 0;
	}

	void HealTime(Stats& stats) // We are healing another
	{
		stats.HP += this->Heal;
		if (stats.HP > stats.MaxHP)
			stats.HP = stats.MaxHP;
		
	}
};