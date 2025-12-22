#pragma once
#include "Effect.h"
#include "CombatSystem.h"

class BlockEffectAdapter : public Effect {
public:
    explicit BlockEffectAdapter(int block) : block_(block) {}
    
    void execute(std::shared_ptr<Creature> source,
                std::shared_ptr<Creature> target,
                std::shared_ptr<Card> card,
                int& numeric_value) override {
        if (source) {
            CombatSystem::getInstance()->Addblock(source, block_);
        }
    }
    
private:
    int block_;
};