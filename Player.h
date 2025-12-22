#pragma once
#include <vector>
#include <memory>
#include "Creature.h"

using namespace std;

class Relic;

class Player : public Creature
{
public:
    int coins_;
    static shared_ptr<Player> getInstance();

    void startTurn() override;
    void endTurn() override;

    int getMaxEnergy() const { return maxEnergy_; }
    int getCurrentEnergy() const { return currentEnergy_; }
    void energyChange(int changeValue);
    void setMaxEnergy(int value) { maxEnergy_ = value; }

    void init();

    void loseHealth(int numeric_value) override;
    
    virtual ~Player() {}
    
private:
    Player() : maxEnergy_(3), currentEnergy_(0), coins_(0) {
        name_ = "鼠鼠";
    }
    static shared_ptr<Player> instance_;
    int maxEnergy_;
    int currentEnergy_;
};