#pragma once

#include "Buff.h"
#include "Creature.h"
#include <memory>
#include <vector>

class CombatDeck;
class RandomGenerator;
class EventSystem;
class Player;
class Card;


class CombatSystem {
public:
    static CombatSystem* getInstance();

    
    CombatSystem(std::shared_ptr<RandomGenerator> randomGen,
        std::shared_ptr<EventSystem> eventSystem,
        std::shared_ptr<Player> player);

    
    CombatSystem();

    std::vector<std::shared_ptr<Creature>> Monsters_;
    std::unique_ptr<CombatDeck> deckManager_;
    std::shared_ptr<Card> tem_card;

    void init(int type = -1);

    void onAttack(std::shared_ptr<Creature> attacker, std::shared_ptr<Creature> target,
        int& numeric_value_, std::string cardName = "", bool isForIntentionUpdate = false);

    void combatStart();
    void combatEnd();

    void takeDamage(std::shared_ptr<Creature> target, int numeric_value_,
        std::shared_ptr<Creature> attacker = nullptr);

    void Addblock(std::shared_ptr<Creature> target, int numeric_value_);
    void exhaustCard(int num, std::string cardName = "");
    void updateBuff(std::shared_ptr<Creature> creature);
    void exhaustCard();
    void exhaustCard(std::shared_ptr<Card> card);
    void startTurn(std::shared_ptr<Creature> creature);
    void endTurn(std::shared_ptr<Creature> creature);
    void cardPlayed(std::shared_ptr<Card> card);
    void endTurnCardPlayed();
    void cardPlayed(std::shared_ptr<Card> card, std::shared_ptr<Creature> creature);
    void discardCard(std::shared_ptr<Card> card);
    void upgradeCard(std::shared_ptr<Card> card);
    void addEnergy(std::shared_ptr<Creature> user, int numeric_value_);
    void addBuff(std::shared_ptr<Buff> buff, int numeric_value, std::shared_ptr<Creature> target);
    void addHealth(std::shared_ptr<Creature> target, int numeric_value);
    void shuffleDeck();
    void drawCard(int num);

    int getDrawPileNumber();
    int getHandNumber();
    int getDiscardPileNumber();

    void addToHand(std::shared_ptr<Card> card, int num = 1);
    void addToDrawPile(std::shared_ptr<Card> card, int num = 1);
    void addToDiscardPile(std::shared_ptr<Card> card, int num = 1);
    void onDeath(std::shared_ptr<Creature> creature);

    int getRoundNumber() const
    {
        return round_;
    }

    void tem_cardPlayed(std::shared_ptr<Card> card);
    std::shared_ptr<Creature> getMonsterPointer(Creature* creature);
    void use_tem_card();

    // Get CombatDeck instance (for backward compatibility)
    CombatDeck* getDeckManager() const { return deckManager_.get(); }

   

    ~CombatSystem() { };

private:
    static CombatSystem* instance_;

    // NEW: Injected dependencies
    std::shared_ptr<RandomGenerator> randomGen_;
    std::shared_ptr<EventSystem> eventSystem_;
    std::shared_ptr<Player> player_;


    int round_;
    int isLastCombat = 0;
};