#pragma once
#include<vector>
#include "Creature.h"
//#include "IncludeAll.h"
//#include "Relic.h"
using namespace std;

class Player :public Creature
{
public:
    int coins_;
    static shared_ptr<Player> getInstance();

    void startTurn(); //开始回合，拿牌，回能量，失去格挡，buff结算

    void endTurn(); //结束回合，失去卡牌，失去能量，buff结算

    int getMaxEnergy() const {   //返回最大能量
        return maxEnergy_;
    }

    int getCurrentEnergy() const {     //返回当前能量
        return currentEnergy_;
    }
    void energyChange(int changeValue);//改变角色当前能量

    //void addPotion(Potion* potion);    

    void init();  //初始化角色

    virtual ~Player() {};
private:
    static shared_ptr<Player> instance_;
    int maxEnergy_;
    int currentEnergy_;
};
