#pragma once
#include "Effect.h"
#include "CombatSystem.h"
#include "Card.h"

class SecondWindEffectAdapter : public Effect {
public:
    explicit SecondWindEffectAdapter(int blockPerCard) : block_per_card_(blockPerCard) {}
    
    void execute(std::shared_ptr<Creature> source,
                std::shared_ptr<Creature> target,
                std::shared_ptr<Card> card,
                int& numeric_value) override {
        if (source) {
            auto hand = CombatSystem::getInstance()->getHand();
            int num = 0;
            
            // 收集所有非攻击牌
            std::vector<int> toExhaust;
            for (size_t i = 0; i < hand.size(); ++i) {
                if (hand[i]->getType() != ATTACK) {
                    toExhaust.push_back(i);
                }
            }
            
            // 从后往前消耗（避免索引问题）
            for (int i = toExhaust.size() - 1; i >= 0; --i) {
                CombatSystem::getInstance()->exhaustCard(toExhaust[i]);
                CombatSystem::getInstance()->Addblock(source, block_per_card_);
                num++;
            }
            
            std::cout << "消耗了 " << num << " 张非攻击牌，获得 " << (num * block_per_card_) << " 点格挡\n";
        }
    }
    
private:
    int block_per_card_;
};