#include "Monster.h"
#include "Enum.h"
#include <string>
#include <random>
#include "Player.h"
#include "CombatSystem.h"
#include "Register.h"

using namespace std;

// 早八 - 第一关怪物
class Zaoba : public Monster
{
public:
    Zaoba() : Monster(NORMAL, 25, "早八", 1) {
        description_ = "每天早上8点的课程，让人痛不欲生";
        attack_times = 1;
        attack_numeric_value = 6;
    }
    
    void takeEffect() override {
        auto player = Player::getInstance();
        if (round_num % 2 == 1) {
            // 攻击
            cout << name_ << " 发动了【点名攻击】！\n";
            CombatSystem::getInstance()->onAttack(
                shared_ptr<Monster>(this, [](Monster*){}), 
                player, attack_numeric_value, "点名");
        } else {
            // 防御
            cout << name_ << " 进入【划水状态】，获得5点格挡\n";
            addBlock(5);
        }
    }
    
    string intentionDisplay() override {
        if (round_num % 2 == 0) {
            attack_times = 1;
            attack_numeric_value = 6;
            return "攻击 " + to_string(attack_numeric_value);
        } else {
            attack_times = 0;
            return "防御";
        }
    }
};
AUTO_REGISTER_MONSTER(Zaoba)

// 高数作业 - 第二关怪物
class MathHomework : public Monster
{
public:
    MathHomework() : Monster(NORMAL, 35, "高数作业", 1) {
        description_ = "密密麻麻的公式，看得人头皮发麻";
        attack_times = 1;
        attack_numeric_value = 8;
    }
    
    void takeEffect() override {
        auto player = Player::getInstance();
        cout << name_ << " 发动了【公式轰炸】！\n";
        CombatSystem::getInstance()->onAttack(
            shared_ptr<Monster>(this, [](Monster*){}), 
            player, attack_numeric_value, "公式轰炸");
        
        // 每3回合加强
        if (round_num % 3 == 0) {
            attack_numeric_value += 2;
            cout << name_ << " 的难度增加了！攻击力+" << 2 << "\n";
        }
    }
    
    string intentionDisplay() override {
        attack_times = 1;
        return "攻击 " + to_string(attack_numeric_value);
    }
};
AUTO_REGISTER_MONSTER(MathHomework)

// 体测 - 第三关怪物
class PhysicalTest : public Monster
{
public:
    PhysicalTest() : Monster(NORMAL, 45, "体测800米", 1) {
        description_ = "跑完感觉灵魂都要出窍了";
        attack_times = 2;
        attack_numeric_value = 5;
    }
    
    void takeEffect() override {
        auto player = Player::getInstance();
        cout << name_ << " 发动了【连续冲刺】！攻击2次！\n";
        for (int i = 0; i < attack_times; i++) {
            CombatSystem::getInstance()->onAttack(
                shared_ptr<Monster>(this, [](Monster*){}), 
                player, attack_numeric_value, "冲刺");
        }
    }
    
    string intentionDisplay() override {
        attack_times = 2;
        attack_numeric_value = 5;
        return "攻击 " + to_string(attack_numeric_value) + " x" + to_string(attack_times);
    }
};
AUTO_REGISTER_MONSTER(PhysicalTest)

// DDL - 第四关怪物
class DDL : public Monster
{
public:
    DDL() : Monster(ELITE, 55, "DDL压力", 1) {
        description_ = "Deadline是第一生产力，也是第一杀手";
        attack_times = 1;
        attack_numeric_value = 12;
    }
    
    void takeEffect() override {
        auto player = Player::getInstance();
        if (round_num <= 2) {
            cout << name_ << " 进入【倒计时】状态，获得8点格挡\n";
            addBlock(8);
            attack_numeric_value += 3;
            cout << "攻击力增加到 " << attack_numeric_value << "！\n";
        } else {
            cout << name_ << " 发动了【最后通牒】！\n";
            CombatSystem::getInstance()->onAttack(
                shared_ptr<Monster>(this, [](Monster*){}), 
                player, attack_numeric_value, "最后通牒");
        }
    }
    
    string intentionDisplay() override {
        if (round_num <= 2) {
            attack_times = 0;
            return "蓄力 (攻击力+3)";
        } else {
            attack_times = 1;
            return "攻击 " + to_string(attack_numeric_value);
        }
    }
};
AUTO_REGISTER_MONSTER(DDL)

// 期末考试 - Boss
class Exam : public Monster
{
public:
    Exam() : Monster(BOSS, 100, "期末考试周", 1) {
        description_ = "所有科目同时袭来，这是最终的试炼！";
        attack_times = 1;
        attack_numeric_value = 10;
    }
    
    void takeEffect() override {
        auto player = Player::getInstance();
        int action = round_num % 4;
        
        switch(action) {
            case 1:
                cout << name_ << " 发动了【突击检查】！\n";
                CombatSystem::getInstance()->onAttack(
                    shared_ptr<Monster>(this, [](Monster*){}), 
                    player, attack_numeric_value, "突击检查");
                break;
            case 2:
                cout << name_ << " 进入【复习周】，获得12点格挡并恢复5点生命\n";
                addBlock(12);
                addHealth(5);
                break;
            case 3:
                cout << name_ << " 发动了【连环考试】！攻击3次！\n";
                for (int i = 0; i < 3; i++) {
                    CombatSystem::getInstance()->onAttack(
                        shared_ptr<Monster>(this, [](Monster*){}), 
                        player, 6, "考试");
                }
                break;
            case 0:
                cout << name_ << " 发动了【挂科警告】！造成巨额伤害！\n";
                CombatSystem::getInstance()->onAttack(
                    shared_ptr<Monster>(this, [](Monster*){}), 
                    player, 20, "挂科警告");
                break;
        }
    }
    
    string intentionDisplay() override {
        int action = (round_num + 1) % 4;
        switch(action) {
            case 1:
                attack_times = 1;
                attack_numeric_value = 10;
                return "攻击 " + to_string(attack_numeric_value);
            case 2:
                attack_times = 0;
                return "防御并恢复";
            case 3:
                attack_times = 3;
                attack_numeric_value = 6;
                return "攻击 6 x3";
            case 0:
                attack_times = 1;
                attack_numeric_value = 20;
                return "强力攻击 20";
            default:
                return "未知";
        }
    }
};
AUTO_REGISTER_MONSTER(Exam)