/*#pragma once
#include "Effect.h"
#include "CombatSystem.h"

class ApotheosisEffectAdapter : public Effect {
public:
    void execute(std::shared_ptr<Creature> source,
                std::shared_ptr<Creature> target,
                std::shared_ptr<Card> card,
                int& numeric_value) override {
        auto combatSystem = CombatSystem::getInstance();
        // Upgrade hand
        for (auto& handCard : combatSystem->hand) {
            combatSystem->upgradeCard(handCard);
        }

        // Upgrade discard pile (preserve order)
        for (int i = 0; i < combatSystem->getDiscardPileNumber(); i++) {
            auto discardCard = combatSystem->discardPile.front();
            combatSystem->upgradeCard(discardCard);
            combatSystem->discardPile.push(discardCard);
            combatSystem->discardPile.pop();
        }

        // Upgrade draw pile (preserve order)
        for (int i = 0; i < combatSystem->getDrawPileNumber(); i++) {
            auto drawCard = combatSystem->drawPile.front();
            combatSystem->upgradeCard(drawCard);
            combatSystem->drawPile.push(drawCard);
            combatSystem->drawPile.pop();
        }
    }
};
*/