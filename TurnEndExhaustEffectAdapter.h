/*#pragma once
#include "Effect.h"
#include "CombatSystem.h"

class TurnEndExhaustEffectAdapter : public Effect {
public:
    void execute(std::shared_ptr<Creature> source,
                std::shared_ptr<Creature> target,
                std::shared_ptr<Card> card,
                int& numeric_value) override {
        if (card) {
            CombatSystem::getInstance()->exhaustCard(card);
        }
    }
};
*/