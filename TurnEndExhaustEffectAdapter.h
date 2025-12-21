#pragma once
#include "Effect.h"
#include "CombatSystem.h"

class TurnEndExhaustEffectAdapter : public Effect {
public:
    void execute(std::shared_ptr<Creature> source = nullptr,
                std::shared_ptr<Creature> target = nullptr,
                std::shared_ptr<Card> card = nullptr,
                int& numeric_value = 0) override {
        if (card) {
            CombatSystem::getInstance()->exhaustCard(card);
        }
    }
};