#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <cstdlib>
#include "Player.h"
#include "Monster.h"
#include "CombatSystem.h"
#include "EventSystem.h"
#include "MapSystem.h"
#include "ShopSystem.h"
#include "Relic.h"
#include "RandomGenerator.h"
#include "Register.h"
#include "Card.h"
#include "Buff.h"

using namespace std;

class Game {
private:
    shared_ptr<Player> player;
    shared_ptr<MapSystem> mapSystem;
    shared_ptr<ShopSystem> shopSystem;
    shared_ptr<EventSystem> eventSystem;
    shared_ptr<CombatSystem> combatSystem;
    
    void clearScreen() {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
    }
    
    void displayTitle() {
        cout << "\n";
        cout << "  ╔═══════════════════════════════════════╗\n";
        cout << "  ║                                       ║\n";
        cout << "  ║       鼠 鼠 大 学 生 存               ║\n";
        cout << "  ║       ShuShu's College Life          ║\n";
        cout << "  ║                                       ║\n";
        cout << "  ║       ~ 一款杀戮尖塔风格游戏 ~        ║\n";
        cout << "  ║                                       ║\n";
        cout << "  ╚═══════════════════════════════════════╝\n\n";
    }
    
    void initializeGame() {
        cout << "正在初始化游戏...\n\n";
        
        // 初始化随机数生成器
        RandomGenerator::getInstance()->initializeGenerators();
        
        // 初始化玩家
        player = Player::getInstance();
        eventSystem = EventSystem::getInstance();
        
        eventSystem->fullHealth_ = 80;
        eventSystem->health_ = 80;
        eventSystem->coins_ = 100;
        
        player->init();
        
        // 初始化其他系统
        mapSystem = MapSystem::getInstance();
        shopSystem = ShopSystem::getInstance();
        combatSystem = CombatSystem::getInstance();
        
        // 生成地图
        mapSystem->generateMap();
        
        cout << "\n========================================\n";
        cout << "  欢迎鼠鼠！新学期开始了...\n";
        cout << "========================================\n";
        cout << "初始生命: " << eventSystem->health_ << "/" << eventSystem->fullHealth_ << "\n";
        cout << "初始金币: " << eventSystem->coins_ << "\n";
        cout << "初始能量: " << player->getMaxEnergy() << "/回合\n";
        cout << "初始牌组: " << combatSystem->deck.size() << " 张\n";
        cout << "========================================\n";
        
        cout << "\n按回车开始游戏...";
        cin.get();
    }
    
    void displayPlayerStatus() {
        cout << "\n┌─────────── 鼠鼠状态 ───────────┐\n";
        cout << "│ 生命: " << eventSystem->health_ << "/" << eventSystem->fullHealth_;
        cout << "  金币: " << eventSystem->coins_;
        cout << "  能量: " << player->getMaxEnergy() << "/回合 │\n";
        
        if (!player->getRelics().empty()) {
            cout << "│ 遗物: ";
            for (const auto& relic : player->getRelics()) {
                cout << relic->getName() << " ";
            }
            cout << "│\n";
        }
        
        cout << "│ 牌组: " << combatSystem->deck.size() << " 张";
        cout << "                        │\n";
        cout << "└─────────────────────────────────┘\n";
    }
    
    void handleMonsterNode(bool isElite, bool isBoss) {
        cout << "\n========================================\n";
        if (isBoss) {
            cout << "     ★★★ BOSS战 - 期末考试周 ★★★\n";
        } else if (isElite) {
            cout << "     ★ 精英战斗 ★\n";
        } else {
            cout << "     战斗开始!\n";
        }
        cout << "========================================\n";
        
        // 创建怪物
        shared_ptr<Monster> monster;
        string monsterName;
        
        if (isBoss) {
            monster = MonsterRegistry::createMonster("Exam");
            monsterName = "期末考试";
        } else if (isElite) {
            vector<string> eliteMonsters = {"DDL", "PhysicalTest"};
            int idx = RandomGenerator::getInstance()->getRandomNumber(0, eliteMonsters.size() - 1);
            monster = MonsterRegistry::createMonster(eliteMonsters[idx]);
            monsterName = monster ? monster->getName() : "精英怪物";
        } else {
            vector<string> normalMonsters = {"Zaoba", "MathHomework"};
            int idx = RandomGenerator::getInstance()->getRandomNumber(0, normalMonsters.size() - 1);
            monster = MonsterRegistry::createMonster(normalMonsters[idx]);
            monsterName = monster ? monster->getName() : "普通怪物";
        }
        
        if (!monster) {
            cout << "怪物创建失败，跳过战斗。\n";
            return;
        }
        
        cout << "遭遇: " << monster->getName() << "\n";
        cout << "敌人生命: " << monster->getHealth() << "/" << monster->getMaxHealth() << "\n\n";
        
        // 初始化战斗
        combatSystem->init(player, monster);
        
        // 战斗循环
        bool victory = runCombat(monster);
        
        if (victory) {
            int goldReward = isBoss ? 100 : (isElite ? 50 : RandomGenerator::getInstance()->getRandomNumber(15, 30));
            eventSystem->coins_ += goldReward;
            
            cout << "\n========================================\n";
            cout << "       ★ 战 斗 胜 利 ★\n";
            cout << "========================================\n";
            cout << "获得 " << goldReward << " 金币!\n";
            
            if (!isBoss) {
                offerCardReward(isElite);
            }
        } else {
            cout << "\n========================================\n";
            cout << "       ✖ 战 斗 失 败 ✖\n";
            cout << "  你被学业的压力击垮了...\n";
            cout << "========================================\n";
        }
    }
    
    bool runCombat(shared_ptr<Monster> monster) {
        int turn = 1;
        
        while (eventSystem->health_ > 0 && monster->getHealth() > 0) {
            cout << "\n╔══════════════════════════════════════╗\n";
            cout << "║          第 " << turn << " 回合                     ║\n";
            cout << "╚══════════════════════════════════════╝\n";
            
            // 玩家回合开始
            player->startTurn();
            combatSystem->drawCards(5);
            
            // 显示战斗状态
            displayCombatState(monster);
            
            // 玩家行动阶段
            playerTurn(monster);
            
            if (monster->getHealth() <= 0) {
                return true;
            }
            
            // 玩家回合结束
            player->endTurn();
            combatSystem->discardHand();
            
            // 怪物回合
            cout << "\n--- " << monster->getName() << " 的回合 ---\n";
            monster->startTurn();
            monster->takeEffect();
            monster->endTurn();
            
            cout << "怪物行动完毕。\n";
            cout << "你受到了伤害，当前生命: " << eventSystem->health_ << "/" << eventSystem->fullHealth_ << "\n";
            
            if (eventSystem->health_ <= 0) {
                return false;
            }
            
            turn++;
            monster->round_num++;
            
            cout << "\n按回车继续下一回合...";
            cin.ignore();
            cin.get();
        }
        
        return eventSystem->health_ > 0;
    }
    
    void displayCombatState(shared_ptr<Monster> monster) {
        // 怪物状态
        cout << "\n┌─────────── 敌方 ───────────┐\n";
        cout << "│ " << monster->getName() << "\n";
        cout << "│ 生命: " << monster->getHealth() << "/" << monster->getMaxHealth();
        cout << "  格挡: " << monster->getBlock() << "\n";
        cout << "│ 意图: ";
        displayMonsterIntent(monster);
        cout << "└────────────────────────────┘\n";
        
        // 玩家状态
        cout << "\n┌─────────── 我方 ───────────┐\n";
        cout << "│ 鼠鼠\n";
        cout << "│ 生命: " << eventSystem->health_ << "/" << eventSystem->fullHealth_;
        cout << "  格挡: " << player->getBlock() << "\n";
        cout << "│ 能量: " << player->getCurrentEnergy() << "/" << player->getMaxEnergy() << "\n";
        
        // 显示buff
        if (!player->buffs_.empty()) {
            cout << "│ 状态: ";
            for (const auto& buff : player->buffs_) {
                cout << buff->name_ << "(" << buff->effect_layers << ") ";
            }
            cout << "\n";
        }
        cout << "└────────────────────────────┘\n";
        
        // 手牌
        displayHand();
    }
    
    void displayMonsterIntent(shared_ptr<Monster> monster) {
        monster->intentionDisplay();
        if (monster->attack_times > 0 && monster->attack_numeric_value > 0) {
            cout << " (攻击 " << monster->attack_numeric_value;
            if (monster->attack_times > 1) {
                cout << "x" << monster->attack_times;
            }
            cout << ")";
        }
        cout << "\n";
    }
    
    void displayHand() {
        cout << "\n┌─────────── 手牌 ───────────┐\n";
        auto hand = combatSystem->getHand();  // 使用 getHand() 方法
        if (hand.empty()) {
            cout << "│ (空)\n";
        } else {
            for (size_t i = 0; i < hand.size(); i++) {
                cout << "│ " << (i + 1) << ". " << hand[i]->getName() 
                     << " [" << hand[i]->getEnergyCost() << "费]";
                if (hand[i]->getEnergyCost() <= player->getCurrentEnergy()) {
                    cout << " ✓";
                }
                cout << "\n";
                cout << "│    " << hand[i]->getDescription() << "\n";
            }
        }
        cout << "└────────────────────────────┘\n";
        cout << "抽牌堆: " << combatSystem->getDrawPileNumber()
             << " | 弃牌堆: " << combatSystem->getDiscardPileNumber() << "\n";
    }
    
    void playerTurn(shared_ptr<Monster> monster) {
        while (true) {
            cout << "\n操作: 输入卡牌编号使用 | 0 结束回合 | -1 查看牌组\n";
            cout << "请选择: ";
            
            int choice;
            cin >> choice;
            
            if (cin.fail()) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "无效输入!\n";
                continue;
            }
            
            if (choice == 0) {
                cout << "结束回合。\n";
                break;
            }
            
            if (choice == -1) {
                displayDeck();
                continue;
            }
            
            auto hand = combatSystem->getHand();
            if (choice < 1 || choice > static_cast<int>(hand.size())) {
                cout << "无效的卡牌编号!\n";
                continue;
            }
            
            int cardIndex = choice - 1;
            auto card = hand[cardIndex];
            
            if (player->getCurrentEnergy() < card->getEnergyCost()) {
                cout << "能量不足! 需要 " << card->getEnergyCost() << " 费\n";
                continue;
            }
            
            // 使用卡牌
            player->energyChange(-card->getEnergyCost());
            cout << "\n使用【" << card->getName() << "】!\n";
            
            if (card->needTarget()) {
                card->takeEffect(monster);
            } else {
                card->takeEffect();
            }
            
            // 使用 CombatDeck 的方法处理卡牌
            auto usedCard = combatSystem->removeFromHand(cardIndex);
            if (usedCard) {
                if (usedCard->isExhaust()) {
                    combatSystem->exhaustCard(usedCard);
                } else {
                    combatSystem->addToDiscardPile(usedCard);
                }
            }
            
            // 检查怪物是否死亡
            if (monster->getHealth() <= 0) {
                cout << monster->getName() << " 被击败了!\n";
                break;
            }
            
            // 显示更新后的状态
            displayCombatState(monster);
        }
    }
    
    void displayDeck() {
        cout << "\n========== 牌组 ==========\n";
        auto& deck = combatSystem->deck;
        for (size_t i = 0; i < deck.size(); i++) {
            cout << (i + 1) << ". " << deck[i]->getName() 
                 << " [" << deck[i]->getEnergyCost() << "费] - " 
                 << deck[i]->getDescription() << "\n";
        }
        cout << "===========================\n";
    }
    
    void offerCardReward(bool isElite) {
        cout << "\n========== 卡牌奖励 ==========\n";
        cout << "选择一张卡牌加入牌组:\n\n";
        
        vector<shared_ptr<Card>> rewards;
        int rewardCount = isElite ? 4 : 3;
        
        for (int i = 0; i < rewardCount; i++) {
            auto card = RandomGenerator::getInstance()->getRandomCard(ALL);
            if (card) {
                rewards.push_back(card);
            }
        }
        
        for (size_t i = 0; i < rewards.size(); i++) {
            cout << (i + 1) << ". " << rewards[i]->getName() 
                 << " [" << rewards[i]->getEnergyCost() << "费]\n";
            cout << "   " << rewards[i]->getDescription() << "\n\n";
        }
        cout << (rewards.size() + 1) << ". 跳过\n";
        cout << "\n请选择: ";
        
        int choice;
        cin >> choice;
        
        if (choice >= 1 && choice <= static_cast<int>(rewards.size())) {
            combatSystem->addToDeck(rewards[choice - 1]);
            cout << "\n获得【" << rewards[choice - 1]->getName() << "】!\n";
        } else {
            cout << "\n跳过奖励。\n";
        }
    }
    
    void handleEventNode() {
        eventSystem->triggerRandomEvent();
    }
    
    void handleShopNode() {
        shopSystem->enterShop();
    }
    
    void handleRestNode() {
        cout << "\n========================================\n";
        cout << "         寝室休息\n";
        cout << "========================================\n";
        cout << "当前生命: " << eventSystem->health_ << "/" << eventSystem->fullHealth_ << "\n\n";
        cout << "1. 休息 (恢复30%最大生命值)\n";
        cout << "2. 冥想 (升级一张牌) [暂未实现]\n";
        cout << "3. 离开\n";
        cout << "\n请选择: ";
        
        int choice;
        cin >> choice;
        
        switch (choice) {
            case 1: {
                int healAmount = eventSystem->fullHealth_ * 30 / 100;
                int actualHeal = min(healAmount, eventSystem->fullHealth_ - eventSystem->health_);
                eventSystem->health_ += actualHeal;
                cout << "\n你美美地睡了一觉，恢复了 " << actualHeal << " 点生命!\n";
                cout << "当前生命: " << eventSystem->health_ << "/" << eventSystem->fullHealth_ << "\n";
                break;
            }
            case 2:
                cout << "\n功能暂未实现。\n";
                break;
            case 3:
                cout << "\n你决定不休息，继续前进。\n";
                break;
            default:
                cout << "\n无效选择。\n";
        }
    }
    
    void processNode(MapNode* node) {
        if (!node) return;
        
        switch (node->type) {
            case NODE_MONSTER:
                handleMonsterNode(false, false);
                break;
            case NODE_ELITE:
                handleMonsterNode(true, false);
                break;
            case NODE_BOSS:
                handleMonsterNode(false, true);
                break;
            case NODE_EVENT:
                handleEventNode();
                break;
            case NODE_SHOP:
                handleShopNode();
                break;
            case NODE_REST:
                handleRestNode();
                break;
        }
    }
    
    /*void advanceToNextLayer() {
        auto& nodes = mapSystem->getCurrentLayerNodes();
        bool anyVisited = false;
        for (const auto& node : nodes) {
            if (node.visited && (node.type == NODE_MONSTER || node.type == NODE_ELITE || node.type == NODE_BOSS)) {
                anyVisited = true;
                break;
            }
        }
        
        // 只有完成战斗节点才能进入下一层
        // 这里简化处理，访问任何节点后都可以选择进入下一层
    }*/
    
public:
    void run() {
        displayTitle();
        
        cout << "1. 开始新游戏\n";
        cout << "2. 退出\n";
        cout << "\n请选择: ";
        
        int choice;
        cin >> choice;
        cin.ignore();
        
        if (choice != 1) {
            cout << "再见!\n";
            return;
        }
        
        initializeGame();
        
        // 主游戏循环
        while (eventSystem->health_ > 0) {
            displayPlayerStatus();
            mapSystem->displayMap();
            
            cout << "\n请选择路径 (输入编号): ";
            int nodeChoice;
            cin >> nodeChoice;
            nodeChoice;
            
            if (cin.fail()) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "无效输入!\n";
                continue;
            }
            
            auto& currentNodes = mapSystem->getCurrentLayerNodes();
            if (nodeChoice < 1 || nodeChoice > static_cast<int>(currentNodes.size())) {
                cout << "无效选择!\n";
                continue;
            }
            
            // 选择并处理节点
            mapSystem->selectNode(--nodeChoice);
            MapNode* selectedNode = mapSystem->getSelectedNode();
            
            processNode(selectedNode);
            
            // 检查是否死亡
            if (eventSystem->health_ <= 0) {
                cout << "\n========================================\n";
                cout << "         GAME OVER\n";
                cout << "   你被大学生活击败了...\n";
                cout << "========================================\n";
                break;
            }
            
            // 如果完成了战斗节点，进入下一层
            if (selectedNode /*&& (selectedNode->type == NODE_MONSTER || 
                                  selectedNode->type == NODE_ELITE ||
                                  selectedNode->type == NODE_BOSS)*/) {
                
                if (mapSystem->isBossLayer() /*&& selectedNode->type == NODE_BOSS*/) {
                    // 通关!
                    cout << "\n╔═══════════════════════════════════════╗\n";
                    cout << "║                                       ║\n";
                    cout << "║       ★★★ 恭 喜 存 活 ★★★        ║\n";
                    cout << "║                                       ║\n";
                    cout << "║          真好!又水了一次作业~            ║\n";
                    cout << "║                                       ║\n";
                    cout << "╚═══════════════════════════════════════╝\n\n";
                    cout << "最终统计:\n";
                    cout << "剩余生命: " << eventSystem->health_ << "/" << eventSystem->fullHealth_ << "\n";
                    cout << "剩余金币: " << eventSystem->coins_ << "\n";
                    cout << "牌组数量: " << combatSystem->deck.size() << " 张\n";
                    cout << "遗物数量: " << player->getRelics().size() << " 个\n";
                    break;
                }
                
                // 进入下一层
                if (mapSystem->getCurrentLayer() < mapSystem->getTotalLayers() - 1) {
                    mapSystem->advanceLayer();
                    cout << "\n进入下一周...\n";
                    cout << "按回车继续...";
                    cin.ignore();
                    cin.get();
                    
                }
            }
            
            
        }
        
        cout << "\n感谢游玩《鼠鼠大学生存》!\n";
    }
};

int main() {
    // 设置控制台编码支持中文 (Windows)
    #ifdef _WIN32
        system("chcp 65001 > nul");
    #endif
    
    Game game;
    game.run();
    
    return 0;
}