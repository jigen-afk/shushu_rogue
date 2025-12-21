#pragma once


#include "Player.h"
#include "Relic.h"
#include "Card.h"
#include <memory>
using namespace std;



class EventSystem {
private:
    static EventSystem* instance;

public:
    int level;
    string name_;
    string character_;
    int health_;
    int fullHealth_;
    int coins_;
    
    vector<std::shared_ptr<Relic>> relics_;
    vector<std::shared_ptr<Card>> cards_;

    EventSystem();
    ~EventSystem();

    bool init();

    
    static EventSystem* getInstance();

    // Setup methods
    void setPlayerInfo(const string& name, const string& character, int fullHealth, int coins);
    void setHealth(int health);
    void setCoins(int coins);
    void setLevel(int level);
    void setFullHealth(int health_);

    int getCurrentHealth() const;
    int getFullHealth() const;
    int getCoins() const;
    void changeHealth(int healthChange);
    int changeCoins(int coinChange);
    void changeMaxHealth(int maxHealthChange);
    void addCard(std::shared_ptr<Card> card);
    void addRelic(std::shared_ptr<Relic> relic);
    int upgradeCard(std::shared_ptr<Card> card);
    int deleteCard(std::shared_ptr<Card> card);

    // Overloaded methods (non-interface)
    int upgradeCard(Card* card);
    int deleteCard(Card* card);
};