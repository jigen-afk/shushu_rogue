#pragma once
#include <vector>
#include <iostream>
#include <memory>
#include <string>

using namespace std;

class Buff;
class Relic;

class Creature
{
protected:
    string name_;
    string description_;
    int health_;
    int max_health_;
    int block_;
    int can_end_turn_;
    vector<shared_ptr<Relic>> relics_;
    
public:
    vector<shared_ptr<Buff>> buffs_;
    
    Creature() : health_(0), max_health_(0), block_(0), can_end_turn_(1) {}
    virtual ~Creature() = default;
    
    string getName() const { return name_; }
    string getDescription() const { return description_; }
    
    void addBlock(int numeric_value) { block_ += numeric_value; }
    void loseBlock(int numeric_value) { 
        block_ -= numeric_value; 
        if (block_ < 0) block_ = 0;
    }
    
     virtual void loseHealth(int numeric_value) { 
        health_ -= numeric_value; 
        if (health_ < 0) health_ = 0;
    }
    
    void addHealth(int numeric_value) { 
        health_ += numeric_value; 
        if (health_ > max_health_) health_ = max_health_;
    }
    
    int getBlockValue() const { return block_; }
    int getBlock() const { return block_; }
    int getHealth() const { return health_; }
    int getMaxHealth() const { return max_health_; }
    
    bool isAlive() const { return health_ > 0; }
    
    void takeDamage(int damage) { loseHealth(damage); }
    
    vector<shared_ptr<Buff>>& getBuffs() { return buffs_; }
    const vector<shared_ptr<Buff>>& getBuffs() const { return buffs_; }
    
    vector<shared_ptr<Relic>>& getRelics() { return relics_; }
    const vector<shared_ptr<Relic>>& getRelics() const { return relics_; }
    
    void addBuff(shared_ptr<Buff> buff) { buffs_.push_back(buff); }
    void addRelic(shared_ptr<Relic> relic) { relics_.push_back(relic); }
    
    virtual void startTurn() { block_ = 0; }
    virtual void endTurn() {}
};