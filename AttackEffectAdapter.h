#pragma once
#include "Effect.h"
#include "CombatSystem.h"
#include "Card.h"

class AttackEffectAdapter : public Effect {
public:
    explicit AttackEffectAdapter(int damage) : damage_(damage) {}
    
    void execute(std::shared_ptr<Creature> source,
                std::shared_ptr<Creature> target,
                std::shared_ptr<Card> card,
                int& numeric_value ) override {
        if (source && target) {
            CombatSystem::getInstance()->onAttack(source, target, damage_, 
                                               card ? card->getName() : "");
        }
    }
    
private:
    int damage_;
};