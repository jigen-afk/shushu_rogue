#include "Player.h"
#include "EventSystem.h"
#include "Enum.h"
#include "CombatSystem.h"
#include "Card.h"
#include "Register.h"

using namespace std;

shared_ptr<Player> Player::instance_ = nullptr;

shared_ptr<Player> Player::getInstance() {
    if (!instance_) {
        instance_ = shared_ptr<Player>(new Player());
    }
    return instance_;
}

void Player::startTurn() {
    block_ = 0;
    currentEnergy_ = maxEnergy_;
    
    // 处理回合开始时的buff效果
    for (auto& buff : buffs_) {
        buff->onTurnStart();
    }
}

void Player::endTurn() {
    // 处理回合结束时的buff效果
    for (auto it = buffs_.begin(); it != buffs_.end(); ) {
        (*it)->onTurnEnd();
        if ((*it)->effect_layers <= 0) {
            it = buffs_.erase(it);
        } else {
            ++it;
        }
    }
}

void Player::energyChange(int changeValue) {
    currentEnergy_ += changeValue;
    if (currentEnergy_ < 0) currentEnergy_ = 0;
}

void Player::init() {
    health_ = EventSystem::getInstance()->health_;
    max_health_ = EventSystem::getInstance()->fullHealth_;
    block_ = 0;
    maxEnergy_ = 3;
    currentEnergy_ = 0;
    coins_ = EventSystem::getInstance()->coins_;
    buffs_.clear();
    
    // 初始化初始牌组
    auto combatSystem = CombatSystem::getInstance();
    combatSystem->deck.clear();
    
    // 添加初始卡牌：5张打击，5张防御
    for (int i = 0; i < 5; i++) {
        combatSystem->addToDeck(CardRegistry::createCard("Attack"));
    }
    for (int i = 0; i < 5; i++) {
        combatSystem->addToDeck(CardRegistry::createCard("Defense"));
    }
}

void Player::loseHealth(int numeric_value) {
    health_ -= numeric_value;
    if (health_ < 0) health_ = 0;
    EventSystem::getInstance()->health_ = health_;
}
