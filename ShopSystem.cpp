#include "ShopSystem.h"
#include "Card.h"
#include "Relic.h"
#include "Player.h"
#include "EventSystem.h"
#include "RandomGenerator.h"
#include "Register.h"
#include "CombatSystem.h"
#include <iostream>
#include <algorithm>

using namespace std;

shared_ptr<ShopSystem> ShopSystem::instance_ = nullptr;

shared_ptr<ShopSystem> ShopSystem::getInstance() {
    if (!instance_) {
        instance_ = shared_ptr<ShopSystem>(new ShopSystem());
    }
    return instance_;
}

ShopSystem::ShopSystem() {
}

void ShopSystem::generateShopItems() {
    items_.clear();
    auto rng = RandomGenerator::getInstance();
    
    // 生成5张卡牌
    for (int i = 0; i < 5; i++) {
        ShopItem item;
        item.type = ITEM_CARD;
        item.card = rng->getRandomCard(ALL);
        if (item.card) {
            item.name = item.card->getName();
            item.description = item.card->getDescription();
            item.price = calculateCardPrice(item.card);
            item.sold = false;
            items_.push_back(item);
        }
    }
    
    // 生成2个遗物
    for (int i = 0; i < 2; i++) {
        ShopItem item;
        item.type = ITEM_RELIC;
        item.relic = rng->getRandomRelic(ALL);
        if (item.relic) {
            item.name = item.relic->getName();
            item.description = item.relic->getDescription();
            item.price = calculateRelicPrice(item.relic);
            item.sold = false;
            items_.push_back(item);
        }
    }
    
    // 移除卡牌服务
    ShopItem removeCard;
    removeCard.type = ITEM_REMOVE_CARD;
    removeCard.name = "移除卡牌";
    removeCard.description = "从牌组中移除一张卡牌";
    removeCard.price = 75;
    removeCard.sold = false;
    items_.push_back(removeCard);
    
    // 升级卡牌服务 (暂时隐藏，功能待实现)
    // ShopItem upgradeCard;
    // upgradeCard.type = ITEM_UPGRADE_CARD;
    // upgradeCard.name = "升级卡牌";
    // upgradeCard.description = "升级牌组中的一张卡牌";
    // upgradeCard.price = 50;
    // upgradeCard.sold = false;
    // items_.push_back(upgradeCard);
    
    // 回复生命服务
    ShopItem healItem;
    healItem.type = ITEM_HEAL;
    healItem.name = "营养快餐";
    healItem.description = "恢复25%最大生命值";
    healItem.price = 60;
    healItem.sold = false;
    items_.push_back(healItem);
}

int ShopSystem::calculateCardPrice(shared_ptr<Card> card) {
    if (!card) return 50;
    
    int basePrice = 50;
    // 简化价格计算，直接使用基础价格
    
    // 添加一点随机浮动
    int variation = RandomGenerator::getInstance()->getRandomNumber(-10, 10);
    return max(20, basePrice + variation);
}

int ShopSystem::calculateRelicPrice(shared_ptr<Relic> relic) {
    if (!relic) return 100;
    
    int basePrice = 100;
    int rarity = relic->getRarity();
    
    switch (rarity) {
        case 0: basePrice = 80; break;   // 普通
        case 1: basePrice = 150; break;  // 罕见
        case 2: basePrice = 250; break;  // 稀有
        default: basePrice = 100;
    }
    
    int variation = RandomGenerator::getInstance()->getRandomNumber(-15, 15);
    return max(50, basePrice + variation);
}

void ShopSystem::displayShop() {
    auto eventSys = EventSystem::getInstance();
    
    cout << "\n========================================\n";
    cout << "           校 园 超 市\n";
    cout << "========================================\n";
    cout << "你的金币: " << eventSys->coins_ << "\n";
    cout << "----------------------------------------\n\n";
    
    cout << "【卡牌】\n";
    int index = 1;
    for (auto& item : items_) {
        if (item.type == ITEM_CARD) {
            if (item.sold) {
                cout << index << ". [已售出]\n";
            } else {
                cout << index << ". " << item.name << " - " << item.price << "金币\n";
                cout << "   " << item.description << "\n";
            }
            index++;
        }
    }
    
    cout << "\n【遗物】\n";
    for (auto& item : items_) {
        if (item.type == ITEM_RELIC) {
            if (item.sold) {
                cout << index << ". [已售出]\n";
            } else {
                cout << index << ". " << item.name << " - " << item.price << "金币\n";
                cout << "   " << item.description << "\n";
            }
            index++;
        }
    }
    
    cout << "\n【服务】\n";
    for (auto& item : items_) {
        if (item.type != ITEM_CARD && item.type != ITEM_RELIC) {
            if (item.sold) {
                cout << index << ". [已售出]\n";
            } else {
                cout << index << ". " << item.name << " - " << item.price << "金币\n";
                cout << "   " << item.description << "\n";
            }
            index++;
        }
    }
    
    cout << "\n" << index << ". 离开商店\n";
    cout << "----------------------------------------\n";
}

bool ShopSystem::purchaseItem(int index) {
    if (index < 0 || index >= static_cast<int>(items_.size())) {
        return false;
    }
    
    auto& item = items_[index];
    auto eventSys = EventSystem::getInstance();
    auto player = Player::getInstance();
    auto combatSys = CombatSystem::getInstance();
    
    if (item.sold) {
        cout << "\n该物品已售出！\n";
        return false;
    }
    
    if (eventSys->coins_ < item.price) {
        cout << "\n金币不足！需要 " << item.price << " 金币。\n";
        return false;
    }
    
    // 扣除金币
    eventSys->coins_ -= item.price;
    item.sold = true;
    
    switch (item.type) {
        case ITEM_CARD:
            if (item.card) {
                combatSys->addToDeck(item.card);
                cout << "\n购买成功！获得卡牌【" << item.name << "】\n";
            }
            break;
            
        case ITEM_RELIC:
            if (item.relic) {
                player->addRelic(item.relic);
                cout << "\n购买成功！获得遗物【" << item.name << "】\n";
            }
            break;
            
        case ITEM_REMOVE_CARD: {
            cout << "\n选择要移除的卡牌:\n";
            auto& deck = combatSys->deck;
            for (size_t i = 0; i < deck.size(); i++) {
                cout << (i + 1) << ". " << deck[i]->getName() << "\n";
            }
            cout << "请选择 (0取消): ";
            int choice;
            cin >> choice;
            if (choice > 0 && choice <= static_cast<int>(deck.size())) {
                cout << "移除了【" << deck[choice - 1]->getName() << "】\n";
                deck.erase(deck.begin() + choice - 1);
            } else {
                eventSys->coins_ += item.price;
                item.sold = false;
                cout << "取消移除，已退款。\n";
            }
            break;
        }
            
        case ITEM_HEAL: {
            int healAmount = eventSys->fullHealth_ / 4;
            int actualHeal = min(healAmount, eventSys->fullHealth_ - eventSys->health_);
            eventSys->health_ += actualHeal;
            cout << "\n恢复了 " << actualHeal << " 点生命！\n";
            break;
        }
            
        default:
            cout << "\n功能暂未实现。\n";
            eventSys->coins_ += item.price;
            item.sold = false;
            return false;
    }
    
    return true;
}

void ShopSystem::enterShop() {
    generateShopItems();
    
    while (true) {
        displayShop();
        cout << "\n请选择购买 (输入编号): ";
        
        int choice;
        cin >> choice;
        
        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "无效输入！\n";
            continue;
        }
        
        if (choice == static_cast<int>(items_.size()) + 1) {
            cout << "\n离开了商店。\n";
            break;
        }
        
        if (choice >= 1 && choice <= static_cast<int>(items_.size())) {
            purchaseItem(choice - 1);
        } else {
            cout << "无效选择！\n";
        }
        
        cout << "\n按回车继续...";
        cin.ignore();
        cin.get();
    }
}