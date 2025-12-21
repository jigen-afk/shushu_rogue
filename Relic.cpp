//#include "IncludeAll.h"
#include "Enum.h"
#include "Relic.h"
#include "CombatSystem.h"
#include "EventSystem.h"
#include "Card.h"
#include "Player.h"
#include "Register.h"

int Relic::getRarity() const
{
    return rarity_;
}

std::string Relic::getName() const
{
    return name_;
}

std::string Relic::getDescription() const
{
    return description_;
}


class LeaveTip : public Relic {
public:
    LeaveTip()
        : Relic("LeaveTip",
            "不知道怎么搞来的假条,战斗结束时回复6点生命值",
            0) {}

    void onCombatEnd() override {
        CombatSystem::getInstance()->addHealth(Player::getInstance(), 6); 
    }
};
AUTO_REGISTER_RELIC(LeaveTip);


class RewardEverytime : public Relic {
public:
    RewardEverytime()
        : Relic("RewardEverytime",
            "大学生要学会奖励自己,战斗结束后加9块钱吃一顿拼好饭",
            1) {}

    void onAddCard(std::shared_ptr<Card> card) override {
        if (card) {
            int goldGain = 9;
            EventSystem::getInstance()->changeCoins(goldGain);
        }
    }
};
AUTO_REGISTER_RELIC(RewardEverytime);

class HistoricPaper : public Relic {
private:
    int attackCounter_; // 记录攻击牌数量
public:
    HistoricPaper()
        : Relic("HistoricPaper",
            "上古历年卷,每打出10张攻击牌,升级一张",
            1), attackCounter_(0) {}

    void onCardPlayed(std::shared_ptr<Card> card) override {
        if (card && card->getType()==ATTACK) { 
            attackCounter_++;
            if (attackCounter_ == 10) {
                CombatSystem::getInstance()->upgradeCard(card);
                attackCounter_ = 0;
            }
        }
    }
};
AUTO_REGISTER_RELIC(HistoricPaper);