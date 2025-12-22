/*#pragma once
#include "Effect.h"
#include "CombatSystem.h"
#include "Register.h"


class TrounceEffectAdapter : public Effect {
public:
    TrounceEffectAdapter(int baseDamage, int baseVulnerability,
                         int upgradeDamageDelta, int upgradeVulnerabilityDelta)
        : baseDamage_(baseDamage),
          baseVulnerability_(baseVulnerability),
          upgradeDamageDelta_(upgradeDamageDelta),
          upgradeVulnerabilityDelta_(upgradeVulnerabilityDelta) {}

    void execute(std::shared_ptr<Creature> source,
                std::shared_ptr<Creature> target,
                std::shared_ptr<Card> card,
                int& numeric_value) override {
        if (!target) {
            return;
        }
        int damage = baseDamage_;
        int vuln = baseVulnerability_;
        //升级后伤害和易伤都增加
        if (card && card->isUpgraded()) {
            damage += upgradeDamageDelta_;
            vuln += upgradeVulnerabilityDelta_;
        }
        CombatSystem::getInstance()->onAttack(Player::getInstance(), target, damage,
                                              card ? card->getName() : "");
        CombatSystem::getInstance()->addBuff(BuffRegistry::createBuff("vulnerability"), vuln, target);
    }

private:
    int baseDamage_;
    int baseVulnerability_;
    int upgradeDamageDelta_;
    int upgradeVulnerabilityDelta_;
};
*/