#pragma once
#include "Creature.h"
#include "Buff.h"
#include <string>
#include <memory>

class Monster : public Creature
{
protected:
    int type_;
    int monster_num_;
    
public:
    int tag = 0;
    int round_num = 1;
    int attack_times = 0;
    int attack_numeric_value = 0;
    
    Monster(int type, int fullhealth, std::string name, int monster_num) 
        : type_(type), monster_num_(monster_num) {
        max_health_ = fullhealth;
        health_ = fullhealth;
        name_ = name;
        block_ = 0;
    }
    
    virtual ~Monster() = default;
    
    void startTurn() override {
        block_ = 0;
        round_num++;
    }
    
    void endTurn() override {
        // 处理buff
        for (auto it = buffs_.begin(); it != buffs_.end(); ) {
            (*it)->onTurnEnd();
            if ((*it)->effect_layers <= 0) {
                it = buffs_.erase(it);
            } else {
                ++it;
            }
        }
    }
    
    int getType() const { return type_; }
    int getAttackTime() const { return attack_times; }
    int getAttackValue() const { return attack_numeric_value; }
    
    virtual void takeEffect() = 0;
    virtual std::string intentionDisplay() = 0;
};