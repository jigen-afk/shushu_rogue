#pragma once
#include "Creature.h"
#include "string"
#include <memory>


//class MonsterStateMachine;

class Monster : public Creature
{
protected:
	// Refactored with State Pattern
	// 状态机（可选，使用State模式的怪物可以使用此成员）
	//std::shared_ptr<MonsterStateMachine> stateMachine_;

public:
	Monster(int type, int fullhealth, std::string name, int monster_num) :
		type_(type) {
		max_health_ = fullhealth;
		health_ = fullhealth;
		name_ = name;
		block_ = 0;
		monster_num_ = monster_num;
	};
	void startTurn();

	void endTurn();

	int getType() const{
		return type_;
	};


	int getAttackTime()  const {
		return attack_times;
	};

	int getAttackValue() const {
		return attack_numeric_value;
	};

	int getMonsterNum() const {
		return monster_num_;
	}

	virtual void takeEffect();

	virtual string intentionDisplay();

	int type_;
	int monster_num_ = 1;
	int tag = 0; // 用于记录怪物当前的行动意图
	int round_num = 1;
	int attack_times = 0;
	int attack_numeric_value = 0;
	virtual ~Monster() {};
	
};