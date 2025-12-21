#include "Monster.h"
#include "Enum.h"
#include "string"
#include "random"
#include "Player.h"
#include "CombatSystem.h"
#include "Register.h"

using namespace std;



class Zaoba : public Monster
{
public:
    Zaoba() : Monster(NORMAL, 20, "早八", 2) {
    }
    void takeEffect() {
        
        shared_ptr<Creature> thisMonster = CombatSystem::getInstance()->getMonsterPointer(this);
       
        
        random_device rd;               // 用于生成随机种子
        mt19937 gen(rd());              // 随机数引擎
        uniform_real_distribution<> dist(0.0, 1.0); // [0.0, 1.0) 均匀分布
        double randomValue = dist(gen); // 生成随机数

        if (tag == 0) {

            int basic_attack_value = 9;
            CombatSystem::getInstance()->onAttack(thisMonster, Player::getInstance(),
                basic_attack_value, "");
            if (randomValue < 0.6) {
                tag = 1; // 60% 的概率赋值为 1
            }
            else {
                tag = 2; // 40% 的概率赋值为 2
            }
        }
        else if (tag == 1) {
            CombatSystem::getInstance()->addBuff(BuffRegistry::createBuff("StrengthBuff"), 3, thisMonster);
            if (randomValue < 0.45) {
                tag = 0; // 45% 的概率赋值为 0
            }
            else {
                tag = 2; // 55% 的概率赋值为 2
            }
        }
        else{
            int basic_attack_value = 7;
            CombatSystem::getInstance()->onAttack(thisMonster, Player::getInstance(),
                basic_attack_value, "");
            CombatSystem::getInstance()->Addblock(thisMonster,5 );
            if (randomValue < 0.36) {
                tag = 0; // 36% 的概率赋值为 0
            }
            else {
                tag = 1; // 64% 的概率赋值为 1
            }
        }
    }
    string intentionDisplay() {
        shared_ptr<Creature> thisMonster = CombatSystem::getInstance()->getMonsterPointer(this);
        if (tag == 0) {
            attack_numeric_value = 9;
            CombatSystem::getInstance()->onAttack(thisMonster, Player::getInstance(),
                attack_numeric_value, "", true);
            attack_times = 1;
            //return "attack.png";
        }
        else if (tag == 1) {
            //return "defendBuff.png";
        }
        else {
            //return "attackDefend.png";
        }
    }
};

AUTO_REGISTER_MONSTER(Zaoba)

/*
class Sentinel : public Monster
{
public:
    //赋予两层人工制品
    Sentinel() : Monster(ELITE, 40, "Sentinel", 3) {
        std::shared_ptr<Buff> buff = BuffRegistry::createBuff("ArtificialProducts");
        buff->effect_layers = 3;
        buffs_.push_back(buff);
    }
    
    void takeEffect() {
        if (tag == 0) {
            std::shared_ptr<Creature> thisMonster = CombatSystem::getInstance()->getMonsterPointer(this);
            int basic_attack_value = 9;
            CombatSystem::getInstance()->onAttack(thisMonster, Player::getInstance(),
                basic_attack_value, "");
            tag = 1;
            
        }
        else if (tag == 1) {
            //玩家手中塞牌
            CombatSystem::getInstance()->addToDiscardPile(CardRegistry::createCard("dazed"), 2);
            tag = 0;
        }
    }

    std::string intentionDisplay() {
        std::shared_ptr<Creature> thisMonster = CombatSystem::getInstance()->getMonsterPointer(this);
        if (tag == 0) {
            attack_numeric_value = 9;
            CombatSystem::getInstance()->onAttack(thisMonster, Player::getInstance(),
                attack_numeric_value, "", true);
            attack_times = 1;
            //return "attack.png";
        }
        else {
            //return "debuff1.png";
        }
    }
};

AUTO_REGISTER_MONSTER(Sentinel)
*/


class Exam : public Monster
{
public:
    Exam() : Monster(BOSS, 250, "考试", 1) {}
    void takeEffect() {
        std::shared_ptr<Creature> thisMonster = CombatSystem::getInstance()->getMonsterPointer(this);
        if (tag == 0) {
            int basic_attack_value = 6;
            CombatSystem::getInstance()->onAttack(thisMonster, Player::getInstance(),
                basic_attack_value, "");
            //塞入一张虚无
            CombatSystem::getInstance()->addToDiscardPile(CardRegistry::createCard("dazed"), 1);
            round_num++;
        }
        else if (tag == 1) {
            int basic_attack_value = 6;
            CombatSystem::getInstance()->onAttack(thisMonster, Player::getInstance(),
                basic_attack_value, "");
            round_num++;
        }
        else if (tag == 2) {
            CombatSystem::getInstance()->Addblock(thisMonster, 12);
            //获得力量Buff
            CombatSystem::getInstance()->addBuff(BuffRegistry::createBuff("StrengthBuff"), 2, thisMonster);
            round_num++;
        }
        else if (tag == 3) {
            for (int i = 0; i < 6; i++) {
                int basic_attack_value = Player::getInstance()->getHealth() / 12 + 1;
                CombatSystem::getInstance()->onAttack(std::make_shared<Exam>(), Player::getInstance(),
                    basic_attack_value, "");
            }
            //塞入三张虚无
            CombatSystem::getInstance()->addToDiscardPile(CardRegistry::createCard("dazed"), 3);
            round_num++;
        }
        if (round_num % 7 == 1 || round_num % 7 == 3 || round_num % 7 == 6) {
            tag = 0;
        }
        else if (round_num % 7 == 2 || round_num % 7 == 5) {
            tag = 1;
        }
        else if (round_num % 7 == 4) {
            tag = 2;
        }
        else if (round_num % 7 == 0) {
            tag = 3;
        }
    }
    string intentionDisplay() 
    {
        shared_ptr<Creature> thisMonster = CombatSystem::getInstance()->getMonsterPointer(this);
        // 在修改意图的时候计算造成的伤害数值和段数
        if (tag == 0) 
        {
            attack_numeric_value = 6;
            CombatSystem::getInstance()->onAttack(thisMonster, Player::getInstance(),
                attack_numeric_value, "", true);
            attack_times = 1;
            //return "attack.png";
        }
        else if(tag == 1) 
        {
            attack_numeric_value = 6;
            CombatSystem::getInstance()->onAttack(thisMonster, Player::getInstance(),
                attack_numeric_value, "", true);
            attack_times = 2;
            //return "attack.png";
        }
        else if (tag == 2) 
        {
            //return "defendBuff.png";
        }
        else if (tag == 3) 
        {
            attack_numeric_value = Player::getInstance()->getHealth() / 12 + 1;
            CombatSystem::getInstance()->onAttack(thisMonster, Player::getInstance(),
                attack_numeric_value, "", true);
            attack_times = 6;
            //return "attack.png";
        }
        else
        {
            return "";
        }
    }
};
AUTO_REGISTER_MONSTER(Exam)