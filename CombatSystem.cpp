#include "CombatSystem.h"
#include "CombatDeck.h"
#include "Card.h"
#include "Player.h"
#include "Monster.h"
#include "Relic.h"
#include "RandomGenerator.h"
#include "EventSystem.h"
#include "Register.h"
#include "Buff.h"
#include <algorithm>
#include <iostream>

using namespace std;

shared_ptr<CombatSystem> CombatSystem::instance_ = nullptr;

shared_ptr<CombatSystem> CombatSystem::getInstance() {
    if (!instance_) {
        instance_ = shared_ptr<CombatSystem>(new CombatSystem());
    }
    return instance_;
}

CombatSystem::CombatSystem() : player_(nullptr), monster_(nullptr) {
    randomGen_ = shared_ptr<RandomGenerator>(RandomGenerator::getInstance());
    combatDeck_ = CombatDeck::create(randomGen_);
}

void CombatSystem::init(shared_ptr<Player> player, shared_ptr<Monster> monster) {
    player_ = player;
    monster_ = monster;
    
    //cout << "[CombatSystem] 初始化战斗..." << endl;
    
    // 初始化玩家
    if (player_) {
        player_->init();
    }
    
    // 初始化战斗牌组
    if (combatDeck_) {
        combatDeck_->init(player_);
        
        // 将主牌组中的卡牌添加到战斗抽牌堆
        for (const auto& card : deck) {
            combatDeck_->addToDrawPile(card);
        }
        
        // 洗牌
        combatDeck_->shuffle();
        
        cout << "[CombatSystem] 战斗牌组初始化完成 - 抽牌堆: " 
             << combatDeck_->getDrawPileSize() << endl;
    } else {
        cerr << "[CombatSystem] 错误：CombatDeck 未创建" << endl;
    }
}

// ==================== 卡牌操作方法 ====================

void CombatSystem::drawCards(int count) {
    if (!combatDeck_) {
        cerr << "[CombatSystem] 错误：CombatDeck 未初始化" << endl;
        return;
    }
    
    int drawn = combatDeck_->drawCards(count);
    
    // 触发抽牌相关的 Buff 和 Relic 效果
    for (int i = 0; i < drawn; i++) {
        // 触发玩家 Buff
        for (auto& buff : player_->buffs_) {
            if (buff) {
                int temp = 1;
                buff->onDrawCard(temp);
            }
        }
        
        // 触发遗物效果
        for (auto& relic : player_->getRelics()) {
            if (relic) {
                int temp = 1;
                relic->onDrawCard(temp);
            }
        }
    }
    
    //cout << "[CombatSystem] 抽了 " << drawn << " 张牌" << endl;
}

void CombatSystem::discardHand() {
    if (!combatDeck_) {
        cerr << "[CombatSystem] 错误：CombatDeck 未初始化" << endl;
        return;
    }
    
    auto hand = combatDeck_->getHand();
    for (auto& card : hand) {
        combatDeck_->addToDiscardPile(card);
    }
    combatDeck_->clearHand();
    
    //cout << "[CombatSystem] 弃置所有手牌" << endl;
}

void CombatSystem::addToHand(shared_ptr<Card> card) {
    if (!combatDeck_) {
        cerr << "[CombatSystem] 错误：CombatDeck 未初始化" << endl;
        return;
    }
    
    combatDeck_->addToHand(card);
}

void CombatSystem::removeFromHand(shared_ptr<Card> card) {
    if (!combatDeck_) {
        cerr << "[CombatSystem] 错误：CombatDeck 未初始化" << endl;
        return;
    }
    
    combatDeck_->removeFromHand(card);
}

shared_ptr<Card> CombatSystem::removeFromHand(int index) {
    if (!combatDeck_) {
        cerr << "[CombatSystem] 错误：CombatDeck 未初始化" << endl;
        return nullptr;
    }
    
    return combatDeck_->removeFromHand(index);
}

void CombatSystem::addToDiscardPile(shared_ptr<Card> card) {
    if (!combatDeck_) {
        cerr << "[CombatSystem] 错误：CombatDeck 未初始化" << endl;
        return;
    }
    
    combatDeck_->addToDiscardPile(card);
}

void CombatSystem::addToExhaustPile(shared_ptr<Card> card) {
    //cout << "[CombatSystem] 卡牌【" << card->getName() << "】被消耗" << endl;
}

void CombatSystem::addToDeck(shared_ptr<Card> card) {
    deck.push_back(card);
    //cout << "[CombatSystem] 添加【" << card->getName() << "】到主牌组" << endl;
}

bool CombatSystem::exhaustCard(shared_ptr<Card> card) {
    if (!combatDeck_) {
        cerr << "[CombatSystem] 错误：CombatDeck 未初始化" << endl;
        return false;
    }
    
    // 触发消耗卡牌的 Buff 和 Relic 效果
    for (auto& buff : player_->buffs_) {
        if (buff) {
            buff->onExhaustCard();
        }
    }
    
    for (auto& relic : player_->getRelics()) {
        if (relic) {
            relic->onExhaustCard();
        }
    }
    
    return combatDeck_->exhaustCard(card);
}

bool CombatSystem::exhaustCard(int index) {
    if (!combatDeck_) {
        cerr << "[CombatSystem] 错误：CombatDeck 未初始化" << endl;
        return false;
    }
    
    auto hand = combatDeck_->getHand();
    if (index < 0 || index >= static_cast<int>(hand.size())) {
        return false;
    }
    
    auto card = hand[index];
    if (!card) {
        return false;
    }
    
    // 触发消耗效果
    for (auto& buff : player_->buffs_) {
        if (buff) {
            buff->onExhaustCard();
        }
    }
    
    for (auto& relic : player_->getRelics()) {
        if (relic) {
            relic->onExhaustCard();
        }
    }
    
    return combatDeck_->exhaustCard(index);
}

bool CombatSystem::discardCard(shared_ptr<Card> card) {
    if (!combatDeck_) {
        cerr << "[CombatSystem] 错误：CombatDeck 未初始化" << endl;
        return false;
    }
    
    return combatDeck_->discardCard(card);
}

bool CombatSystem::discardCard(int index) {
    if (!combatDeck_) {
        cerr << "[CombatSystem] 错误：CombatDeck 未初始化" << endl;
        return false;
    }
    
    return combatDeck_->discardCard(index);
}

// ==================== 战斗操作方法 ====================

void CombatSystem::onAttack(shared_ptr<Creature> source, shared_ptr<Creature> target, 
                            int damage, const string& cardName) {
    if (!source || !target) {
        cerr << "[CombatSystem] 错误：攻击的源或目标为空" << endl;
        return;
    }
    
    int finalDamage = damage;
    
    // 先遍历使用者的 buff 列表，调用所有 buff 的 onAttack 效果
    for (auto& buff : source->buffs_) {
        if (buff) {
            buff->onAttack(finalDamage, cardName, source, target);
        }
    }
    
    // 再遍历目标的 buff 列表，调用所有 buff 的 onAttacked 效果
    for (auto& buff : target->buffs_) {
        if (buff) {
            buff->onAttacked(finalDamage, source, target);
        }
    }
    
    // 如果是玩家发起攻击，触发遗物的 onAttack 效果
    if (source == player_) {
        for (auto& relic : player_->getRelics()) {
            if (relic) {
                relic->onAttack(finalDamage, cardName, source, target);
            }
        }
    }
    
    // 如果玩家是目标，触发遗物的 onAttacked 效果
    if (target == player_) {
        for (auto& relic : player_->getRelics()) {
            if (relic) {
                relic->onAttacked(finalDamage, source, target);
            }
        }
    }
    
    // 防止负数伤害
    finalDamage = max(finalDamage, 0);
    
    // 造成伤害
    takeDamage(target, finalDamage);
}

void CombatSystem::takeDamage(shared_ptr<Creature> target, int damage) {
    if (!target) {
        cerr << "[CombatSystem] 错误：伤害目标为空" << endl;
        return;
    }
    
    // 情况1：目标的格挡大于等于伤害值
    if (damage <= target->getBlockValue()) {
        // 触发失去格挡的 Buff 效果
        for (auto& buff : target->buffs_) {
            if (buff) {
                int temp = damage;
                buff->onLoseBlock(temp);
            }
        }
        
        if (target == player_) {
            for (auto& relic : player_->getRelics()) {
                if (relic) {
                    int temp = damage;
                    relic->onLoseBlock(temp);
                }
            }
        }
        
        target->loseBlock(damage);
        cout << target->getName() << " 格挡了 " << damage << " 点伤害" << endl;
    }
    // 情况2：目标的格挡不为0但小于伤害值
    else if (target->getBlockValue() > 0) {
        int blockValue = target->getBlockValue();
        // 触发失去格挡的效果
        for (auto& buff : target->buffs_) {
            if (buff) {
                int temp = blockValue;
                buff->onLoseBlock(temp);
            }
        }
        
        if (target == player_) {
            for (auto& relic : player_->getRelics()) {
                if (relic) {
                    int temp = blockValue;
                    relic->onLoseBlock(temp);
                }
            }
        }
        
        int healthLoss = damage - blockValue;
        target->loseBlock(blockValue);
        
        // 触发失去生命的效果
        for (auto& buff : target->buffs_) {
            if (buff) {
                int temp = healthLoss;
                buff->onLoseHealth(temp);
            }
        }
        
        if (target == player_) {
            for (auto& relic : player_->getRelics()) {
                if (relic) {
                    int temp = healthLoss;
                    relic->onLoseHealth(temp);
                }
            }
        }
        
        target->loseHealth(healthLoss);
        cout << target->getName() << " 格挡了 " << blockValue
             << " 点伤害，受到 " << healthLoss << " 点伤害" << endl;
    }
    // 情况3：目标的格挡为0
    else {
        int healthLoss = damage;
        
        // 触发失去生命的效果
        for (auto& buff : target->buffs_) {
            if (buff) {
                int temp = healthLoss;
                buff->onLoseHealth(temp);
            }
        }
        
        if (target == player_) {
            for (auto& relic : player_->getRelics()) {
                if (relic) {
                    int temp = healthLoss;
                    relic->onLoseHealth(temp);
                }
            }
        }
        
        target->loseHealth(healthLoss);
        cout << target->getName() << " 受到 " << healthLoss << " 点伤害" << endl;
    }
    
    // 检查死亡
    if (target->getHealth() <= 0) {
        onDeath(target);
    }
}

void CombatSystem::Addblock(shared_ptr<Creature> target, int block) {
    if (!target) {
        cerr << "[CombatSystem] 错误：格挡目标为空" << endl;
        return;
    }
    
    int tempBlock = block;
    
    // 遍历使用者的 buff 列表，调用所有 buff 的 onGetBlock 效果
    for (auto& buff : target->buffs_) {
        if (buff) {
            buff->onGetBlock(tempBlock);
        }
    }
    
    if (target == player_) {
        for (auto& relic : player_->getRelics()) {
            if (relic) {
                relic->onGetBlock(tempBlock);
            }
        }
    }
    
    // 防止负数格挡
    tempBlock = max(tempBlock, 0);
    
    target->addBlock(tempBlock);
    
    if (tempBlock > 0) {
        cout << target->getName() << " 获得 " << tempBlock << " 点格挡" << endl;
    }
}

void CombatSystem::addBuff(shared_ptr<Buff> buff, int value, shared_ptr<Creature> target) {
    if (!buff || !target) {
        return;
    }
    
    // 遍历目标的 buff 列表，调用所有 buff 的 addBuff 效果
    for (auto it = target->buffs_.begin(); it != target->buffs_.end();) {
        auto existingBuff = *it;
        existingBuff->addBuff(buff, value);
        
        // 如果 buff 层数和持续时间都为0，移除
        if (existingBuff->effect_layers == 0 && existingBuff->duration_ == 0) {
            it = target->buffs_.erase(it);
            continue;
        }
        ++it;
    }
    
    if (value > 0) {
        bool found = false;
        
        // 如果 buff 可以叠加
        if (buff->is_stackable_) {
            for (auto& existingBuff : target->buffs_) {
                // 已经有相同的 buff
                if (existingBuff->name_ == buff->name_) {
                    // 更新持续时间
                    if (existingBuff->stack_type_ == DURATION) {
                        existingBuff->duration_ += value;
                    }
                    // 效果层数叠加
                    else {
                        existingBuff->effect_layers += value;
                    }
                    found = true;
                    break;
                }
            }
            
            if (!found) {
                // 设置持续时间
                if (buff->stack_type_ == DURATION) {
                    buff->duration_ = value;
                }
                // 效果层数叠加name_
                else {
                    buff->effect_layers = value;
                }
                target->buffs_.push_back(buff);
            }
        }
    }
    
    cout << "[CombatSystem] 添加 Buff【" << buff->name_
         << "】x" << value << " 到 " << target->getName() << endl;
}

void CombatSystem::addEnergy(shared_ptr<Creature> user, int value) {
    if (!user) {
        return;
    }
    
    int tempEnergy = value;
    
    // 触发 Buff 效果
    for (auto& buff : user->buffs_) {
        if (buff) {
            buff->onGetEnergy(tempEnergy);
        }
    }
    
    if (user == player_) {
        for (auto& relic : player_->getRelics()) {
            if (relic) {
                relic->onGetEnergy(tempEnergy);
            }
        }
        
        player_->energyChange(tempEnergy);
        cout << "[CombatSystem] 能量变化: " << (tempEnergy > 0 ? "+" : "") 
             << tempEnergy << " (当前: " << player_->getCurrentEnergy() << ")" << endl;
    }
}

void CombatSystem::addHealth(shared_ptr<Creature> target, int value) {
    if (!target) {
        return;
    }
    
    int tempHealthRestore = value;
    
    // 触发 Buff 效果
    for (auto& buff : target->buffs_) {
        if (buff) {
            buff->onAddHealth(tempHealthRestore);
        }
    }
    
    if (target == player_) {
        for (auto& relic : player_->getRelics()) {
            if (relic) {
                relic->onAddHealth(tempHealthRestore);
            }
        }
    }
    
    // 防止负数恢复
    tempHealthRestore = max(0, tempHealthRestore);
    target->addHealth(tempHealthRestore);
    
    cout << target->getName() << " 恢复 " << tempHealthRestore << " 点生命" << endl;
}

// ==================== 回合控制方法 ====================

void CombatSystem::startTurn(shared_ptr<Creature> creature) {
    if (!creature) {
        return;
    }
    
    // 失去所有格挡
    creature->loseBlock(creature->getBlockValue());
    
    // 触发回合开始的 Buff 效果
    for (auto& buff : creature->buffs_) {
        if (buff) {
            buff->onTurnStart();
        }
    }
    
    // 如果是玩家，还要触发遗物效果和其他操作
    if (creature == player_) {
        for (auto& relic : player_->getRelics()) {
            if (relic) {
                relic->onTurnStart();
            }
        }
        
        // 恢复能量到最大值
        const int energy = player_->getMaxEnergy();
        addEnergy(player_, energy);
        
        // 抽5张牌
        drawCards(5);
        
        cout << "[CombatSystem] 玩家回合开始" << endl;
    } else {
        cout << "[CombatSystem] " << creature->getName() << " 回合开始" << endl;
    }
}

void CombatSystem::endTurn(shared_ptr<Creature> creature) {
    if (!creature) {
        return;
    }
    
    // 触发回合结束的 Buff 效果
    for (auto& buff : creature->buffs_) {
        if (buff) {
            buff->onTurnEnd();
        }
    }
    
    // 更新 Buff（减少持续时间，移除过期的）
    updateBuff(creature);
    
    // 如果是玩家
    if (creature == player_) {
        // 触发遗物效果
        for (auto& relic : player_->getRelics()) {
            if (relic) {
                relic->onTurnEnd();
            }
        }
        
        // 清空能量
        const int energy = player_->getCurrentEnergy();
        addEnergy(player_, -energy);
        
        // 弃置所有手牌
        discardHand();
        
        cout << "[CombatSystem] 玩家回合结束" << endl;
    } else {
        cout << "[CombatSystem] " << creature->getName() << " 回合结束" << endl;
    }
}

void CombatSystem::updateBuff(shared_ptr<Creature> creature) {
    if (!creature) {
        return;
    }
    
    for (auto it = creature->buffs_.begin(); it != creature->buffs_.end();) {
        auto buff = *it;
        
        // 如果是持续时间类型的 buff
        if (buff->stack_type_ == DURATION) {
            buff->duration_--;
            if (buff->duration_ == 0) {
                it = creature->buffs_.erase(it);
                continue;
            }
        }
        ++it;
    }
}

void CombatSystem::cardPlayed(shared_ptr<Card> card) {
    if (!card) {
        return;
    }
    
    // 触发打出卡牌的 Buff 效果
    for (auto& buff : player_->buffs_) {
        if (buff) {
            buff->onCardPlayed(card);
        }
    }
    
    for (auto& relic : player_->getRelics()) {
        if (relic) {
            relic->onCardPlayed(card);
        }
    }
    
    // 执行卡牌效果
    card->takeEffect();
    
    // 扣除能量
    int tempEnergyCost = card->getEnergyCost();
    player_->energyChange(-tempEnergyCost);
    
    cout << "[CombatSystem] 打出卡牌【" << card->getName() << "】" << endl;
}

void CombatSystem::cardPlayed(shared_ptr<Card> card, shared_ptr<Creature> target) {
    if (!card) {
        return;
    }
    
    // 触发打出卡牌的 Buff 效果
    for (auto& buff : player_->buffs_) {
        if (buff) {
            buff->onCardPlayed(card);
        }
    }
    
    for (auto& relic : player_->getRelics()) {
        if (relic) {
            relic->onCardPlayed(card);
        }
    }
    
    // 执行卡牌效果（指定目标）
    card->takeEffect(target);
    
    // 扣除能量
    int tempEnergyCost = card->getEnergyCost();
    player_->energyChange(-tempEnergyCost);
    
    cout << "[CombatSystem] 对 " << target->getName() 
         << " 打出卡牌【" << card->getName() << "】" << endl;
}

void CombatSystem::upgradeCard(shared_ptr<Card> card) {
    if (!card) {
        return;
    }
    
    // 如果卡牌还没有进行过升级，那么对卡牌进行升级
    if (!card->isUpgraded()) {
        card->upgrade();
        cout << "[CombatSystem] 升级卡牌【" << card->getName() << "】" << endl;
    }
}

void CombatSystem::onDeath(shared_ptr<Creature> creature) {
    if (!creature) {
        return;
    }
    
    if (creature == player_) {
        for (auto& relic : player_->getRelics()) {
            if (relic) {
                relic->onDeath();
            }
        }
        
        cout << "[CombatSystem] 玩家死亡" << endl;
    } else {
        cout << "[CombatSystem] " << creature->getName() << " 死亡" << endl;
    }
}

void CombatSystem::combatStart() {
    auto player = Player::getInstance();
    
    // 触发战斗开始的 Buff 效果
    for (auto& buff : player->buffs_) {
        if (buff) {
            buff->onCombatStart(player);
        }
    }
    
    // 触发遗物效果
    for (auto& relic : player->getRelics()) {
        if (relic) {
            relic->onCombatStart();
        }
    }
    
    cout << "[CombatSystem] 战斗开始" << endl;
}

void CombatSystem::combatEnd() {
    auto player = Player::getInstance();
    
    // 先触发一次回合结束效果
    endTurn(player);
    
    if (player->getHealth() <= 0) {
        cout << "[CombatSystem] 战斗失败" << endl;
    } else {
        // 触发战斗结束的 Buff 效果
        for (auto& buff : player->buffs_) {
            if (buff) {
                buff->onCombatEnd(player);
            }
        }
        
        // 触发遗物效果
        for (auto& relic : player->getRelics()) {
            if (relic) {
                relic->onCombatEnd();
            }
        }
        
        cout << "[CombatSystem] 战斗胜利" << endl;
    }
}

// ==================== 获取信息方法 ====================

vector<shared_ptr<Card>> CombatSystem::getHand() const {
    if (!combatDeck_) {
        return vector<shared_ptr<Card>>();
    }
    
    return combatDeck_->getHand();
}

shared_ptr<Card> CombatSystem::getHandCard(int index) const {
    if (!combatDeck_) {
        return nullptr;
    }
    
    auto hand = combatDeck_->getHand();
    if (index < 0 || index >= static_cast<int>(hand.size())) {
        return nullptr;
    }
    
    return hand[index];
}

int CombatSystem::getHandSize() const {
    if (!combatDeck_) {
        return 0;
    }
    
    return combatDeck_->getHandSize();
}

int CombatSystem::getDeckSize() const {
    return deck.size();
}

int CombatSystem::getDiscardPileNumber() const {
    if (!combatDeck_) {
        return 0;
    }
    
    return combatDeck_->getDiscardPileSize();
}

int CombatSystem::getDrawPileNumber() const {
    if (!combatDeck_) {
        return 0;
    }
    
    return combatDeck_->getDrawPileSize();
}

void CombatSystem::shuffleDeck() {
    if (combatDeck_) {
        combatDeck_->shuffle();
        cout << "[CombatSystem] 洗牌" << endl;
    }
}