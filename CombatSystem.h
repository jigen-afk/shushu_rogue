#pragma once

#include "Buff.h"
#include "Creature.h"
#include <memory>
#include <vector>
#include <iostream>

class CombatDeck;
class RandomGenerator;
class EventSystem;
class Player;
class Card;
class Monster;

class CombatSystem {
public:
    static std::shared_ptr<CombatSystem> getInstance();
    
    // 初始化战斗
    void init(std::shared_ptr<Player> player, std::shared_ptr<Monster> monster);
    
    // 卡牌操作 - 委托给 CombatDeck
    void drawCards(int count);
    void discardHand();
    void addToHand(std::shared_ptr<Card> card);
    void removeFromHand(std::shared_ptr<Card> card);
    std::shared_ptr<Card> removeFromHand(int index);
    void addToDiscardPile(std::shared_ptr<Card> card);
    void addToExhaustPile(std::shared_ptr<Card> card);
    void addToDeck(std::shared_ptr<Card> card);
    bool exhaustCard(std::shared_ptr<Card> card);
    bool exhaustCard(int index);
    bool discardCard(std::shared_ptr<Card> card);
    bool discardCard(int index);
    
    // 战斗操作
    void onAttack(std::shared_ptr<Creature> source, std::shared_ptr<Creature> target, 
                  int damage, const std::string& cardName = "");
    void Addblock(std::shared_ptr<Creature> target, int block);
    void takeDamage(std::shared_ptr<Creature> target, int damage);
    void addBuff(std::shared_ptr<Buff> buff, int value, std::shared_ptr<Creature> target);
    void addEnergy(std::shared_ptr<Creature> user, int value);
    void addHealth(std::shared_ptr<Creature> target, int value);
    
    // 回合控制
    void startTurn(std::shared_ptr<Creature> creature);
    void endTurn(std::shared_ptr<Creature> creature);
    void updateBuff(std::shared_ptr<Creature> creature);
    
    // 卡牌打出
    void cardPlayed(std::shared_ptr<Card> card);
    void cardPlayed(std::shared_ptr<Card> card, std::shared_ptr<Creature> target);
    void upgradeCard(std::shared_ptr<Card> card);
    
    // 战斗流程
    void combatStart();
    void combatEnd();
    void onDeath(std::shared_ptr<Creature> creature);
    
    // 洗牌
    void shuffleDeck();
    
    // 获取信息
    std::vector<std::shared_ptr<Card>> getHand() const;
    std::shared_ptr<Card> getHandCard(int index) const;
    int getHandSize() const;
    int getDeckSize() const;
    int getDiscardPileNumber() const;
    int getDrawPileNumber() const;
    std::shared_ptr<Monster> getMonster() const { return monster_; }
    std::shared_ptr<Player> getPlayer() const { return player_; }
    
    // 获取CombatDeck实例（用于初始化）
    std::shared_ptr<CombatDeck> getCombatDeck() const { return combatDeck_; }
    
    // 兼容性：访问牌堆（仅用于显示）
    std::vector<std::shared_ptr<Card>> deck;  // 主牌组（战斗外）
    
private:
    CombatSystem();
    static std::shared_ptr<CombatSystem> instance_;
    
    std::shared_ptr<Player> player_;
    std::shared_ptr<Monster> monster_;
    std::shared_ptr<RandomGenerator> randomGen_;
    std::shared_ptr<CombatDeck> combatDeck_;  // 战斗牌组管理器
};