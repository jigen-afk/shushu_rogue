#pragma once
#include <vector>
#include <string>
#include <memory>
#include <iostream>

using namespace std;

class Creature;
class Effect;
class Player;

class Card : public enable_shared_from_this<Card>
{
public:
    Card() : energy_cost_(0), money_cost_(0), can_be_played_(true), 
             type_(0), need_target_(false), is_exhaust_(false), is_upgraded_(false) {}
    
    Card(string name, string desc, int cost, int money, bool playable, 
         int type, bool needTarget, bool exhaust, bool upgraded)
        : name_(name), description_(desc), energy_cost_(cost), money_cost_(money),
          can_be_played_(playable), type_(type), need_target_(needTarget),
          is_exhaust_(exhaust), is_upgraded_(upgraded) {}
    
    virtual ~Card() = default;
    
    // Getters
    string getName() const { return name_; }
    string getDescription() const { return description_; }
    int getEnergyCost() const { return energy_cost_; }
    int getMoneyCost() const { return money_cost_; }
    bool canBePlayed() const { return can_be_played_; }
    int getType() const { return type_; }
    bool needTarget() const { return need_target_; }
    bool isExhaust() const { return is_exhaust_; }
    bool isUpgraded() const { return is_upgraded_; }
    
    // 效果执行
    virtual void takeEffect() {
        executePlayEffects();
    }
    
    virtual void takeEffect(shared_ptr<Creature> target) {
        executePlayEffects(target);
    }
    
    virtual void upgrade() {}
    
    // 效果管理
    void addEffect(shared_ptr<Effect> effect) {
        playEffects_.push_back(effect);
    }
    
    void clearPlayEffects() {
        playEffects_.clear();
    }
    
    void executePlayEffects(shared_ptr<Creature> target = nullptr);
    
    int tag = 0;
    
protected:
    string name_;
    string description_;
    int energy_cost_;
    int money_cost_;
    bool can_be_played_;
    int type_;
    bool need_target_;
    bool is_exhaust_;
    bool is_upgraded_;
    
    vector<shared_ptr<Effect>> playEffects_;
    vector<shared_ptr<Effect>> turnEndEffects_;
    vector<shared_ptr<Effect>> discardEffects_;
    vector<shared_ptr<Effect>> exhaustEffects_;
};