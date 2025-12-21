#pragma once
#include "Effect.h"
#include "CombatSystem.h"

class TurnEndDamageEffectAdapter : public Effect {
public:
    explicit TurnEndDamageEffectAdapter(int damage) : damage_(damage) {}

    void execute(std::shared_ptr<Creature> source = nullptr,
                std::shared_ptr<Creature> target = nullptr,
                std::shared_ptr<Card> card = nullptr,
                int& numeric_value) override {
        CombatSystem::getInstance()->takeDamage(Player::getInstance(), damage_);
    }

private:
    int damage_;
};