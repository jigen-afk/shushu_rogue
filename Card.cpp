#include "Card.h"
#include "Enum.h"
#include "AttackEffectAdapter.h"
#include "BlockEffectAdapter.h"
#include "SecondWindEffectAdapter.h"


#include "Register.h"
#include "CombatSystem.h"
#include "Player.h"
#include "Creature.h"

void Card::executePlayEffects(shared_ptr<Creature> target) {
    auto source = Player::getInstance();
    int dummy = 0;
    for (auto& effect : playEffects_) {
        effect->execute(source, target, shared_from_this(), dummy);
    }
}

// ============ 基础卡牌 ============

/** 
* @class Attack (打击)
* @brief 造成6点伤害
*/
class Attack : public Card
{
public:
    Attack() : Card("打击", "造成6点伤害", 1, 0, true, ATTACK, true, false, false) {
        addEffect(make_shared<AttackEffectAdapter>(6));
    }
    
    void upgrade() override {
        is_upgraded_ = true;
        name_ = "打击+";
        description_ = "造成9点伤害";
        clearPlayEffects();
        addEffect(make_shared<AttackEffectAdapter>(9));
    }
    
    void takeEffect(shared_ptr<Creature> target) override {
        executePlayEffects(target);
    }
};
AUTO_REGISTER_CARD(Attack)

/** 
* @class Defense (防御)
* @brief 获得5点格挡
*/
class Defense : public Card
{
public:
    Defense() : Card("防御", "获得5点格挡", 1, 0, true, SKILL, false, false, false) {
        addEffect(make_shared<BlockEffectAdapter>(5));
    }
    
    void upgrade() override {
        is_upgraded_ = true;
        name_ = "防御+";
        description_ = "获得8点格挡";
        clearPlayEffects();
        addEffect(make_shared<BlockEffectAdapter>(8));
    }
    
    void takeEffect() override {
        executePlayEffects();
    }
};
AUTO_REGISTER_CARD(Defense)

// ============ 大学主题卡牌 ============

/** 
* @class NightStudy (熬夜学习)
* @brief 造成8点伤害，失去2点生命
*/
class NightStudy : public Card
{
public:
    NightStudy() : Card("熬夜学习", "造成8点伤害，失去2点生命", 1, 0, true, ATTACK, true, false, false) {
        addEffect(make_shared<AttackEffectAdapter>(8));
    }
    
    void upgrade() override {
        is_upgraded_ = true;
        name_ = "熬夜学习+";
        description_ = "造成12点伤害，失去2点生命";
        clearPlayEffects();
        addEffect(make_shared<AttackEffectAdapter>(12));
    }
    
    void takeEffect(shared_ptr<Creature> target) override {
        executePlayEffects(target);
        Player::getInstance()->takeDamage(2);
        cout << "熬夜太累了，失去2点生命\n";
    }
};
AUTO_REGISTER_CARD(NightStudy)

/** 
* @class LayFlat (摆烂防御)
* @brief 获得7点格挡
*/
class LayFlat : public Card
{
public:
    LayFlat() : Card("摆烂防御", "获得7点格挡", 1, 0, true, SKILL, false, false, false) {
        addEffect(make_shared<BlockEffectAdapter>(7));
    }
    
    void upgrade() override {
        is_upgraded_ = true;
        name_ = "摆烂防御+";
        description_ = "获得10点格挡";
        clearPlayEffects();
        addEffect(make_shared<BlockEffectAdapter>(10));
    }
    
    void takeEffect() override {
        executePlayEffects();
    }
};
AUTO_REGISTER_CARD(LayFlat)

/** 
* @class CopyHomework (抄作业)
* @brief 造成4点伤害，抽1张牌
*/
class CopyHomework : public Card
{
public:
    CopyHomework() : Card("抄作业", "造成4点伤害，抽1张牌", 1, 0, true, ATTACK, true, false, false) {
        addEffect(make_shared<AttackEffectAdapter>(4));
    }
    
    void upgrade() override {
        is_upgraded_ = true;
        name_ = "抄作业+";
        description_ = "造成6点伤害，抽2张牌";
        clearPlayEffects();
        addEffect(make_shared<AttackEffectAdapter>(6));
    }
    
    void takeEffect(shared_ptr<Creature> target) override {
        executePlayEffects(target);
        int drawCount = is_upgraded_ ? 2 : 1;
        CombatSystem::getInstance()->drawCards(drawCount);
        cout << "抽了" << drawCount << "张牌\n";
    }
};
AUTO_REGISTER_CARD(CopyHomework)

/** 
* @class CoffeePower (咖啡续命)
* @brief 获得1点能量，抽1张牌
*/
class CoffeePower : public Card
{
public:
    CoffeePower() : Card("咖啡续命", "获得1点能量，抽1张牌", 0, 0, true, SKILL, false, false, false) {}
    
    void upgrade() override {
        is_upgraded_ = true;
        name_ = "咖啡续命+";
        description_ = "获得1点能量，抽2张牌";
    }
    
    void takeEffect() override {
        Player::getInstance()->energyChange(1);
        cout << "咖啡因注入！获得1点能量\n";
        int drawCount = is_upgraded_ ? 2 : 1;
        CombatSystem::getInstance()->drawCards(drawCount);
        cout << "抽了" << drawCount << "张牌\n";
    }
};
AUTO_REGISTER_CARD(CoffeePower)

/** 
* @class LastMinuteReview (临时抱佛脚)
* @brief 造成15点伤害，消耗
*/
class LastMinuteReview : public Card
{
public:
    LastMinuteReview() : Card("临时抱佛脚", "造成15点伤害。消耗", 2, 0, true, ATTACK, true, true, false) {
        addEffect(make_shared<AttackEffectAdapter>(15));
    }
    
    void upgrade() override {
        is_upgraded_ = true;
        name_ = "临时抱佛脚+";
        description_ = "造成22点伤害。消耗";
        clearPlayEffects();
        addEffect(make_shared<AttackEffectAdapter>(22));
    }
    
    void takeEffect(shared_ptr<Creature> target) override {
        executePlayEffects(target);
    }
};
AUTO_REGISTER_CARD(LastMinuteReview)

/** 
* @class SkipClass (翘课)
* @brief 获得12点格挡，下回合少抽1张牌
*/
class SkipClass : public Card
{
public:
    SkipClass() : Card("翘课", "获得12点格挡", 1, 0, true, SKILL, false, false, false) {
        addEffect(make_shared<BlockEffectAdapter>(12));
    }
    
    void upgrade() override {
        is_upgraded_ = true;
        name_ = "翘课+";
        description_ = "获得16点格挡";
        clearPlayEffects();
        addEffect(make_shared<BlockEffectAdapter>(16));
    }
    
    void takeEffect() override {
        executePlayEffects();
    }
};
AUTO_REGISTER_CARD(SkipClass)

/** 
* @class StudyGroup (学习小组)
* @brief 造成6点伤害，获得6点格挡
*/
class StudyGroup : public Card
{
public:
    StudyGroup() : Card("学习小组", "造成6点伤害，获得6点格挡", 2, 0, true, ATTACK, true, false, false) {
        addEffect(make_shared<AttackEffectAdapter>(6));
        addEffect(make_shared<BlockEffectAdapter>(6));
    }
    
    void upgrade() override {
        is_upgraded_ = true;
        name_ = "学习小组+";
        description_ = "造成9点伤害，获得9点格挡";
        clearPlayEffects();
        addEffect(make_shared<AttackEffectAdapter>(9));
        addEffect(make_shared<BlockEffectAdapter>(9));
    }
    
    void takeEffect(shared_ptr<Creature> target) override {
        executePlayEffects(target);
    }
};
AUTO_REGISTER_CARD(StudyGroup)

/** 
* @class AllNighter (通宵)
* @brief 造成当前手牌数x3点伤害
*/
class AllNighter : public Card
{
public:
    AllNighter() : Card("通宵", "造成当前手牌数x3点伤害", 1, 0, true, ATTACK, true, false, false) {}
    
    void upgrade() override {
        is_upgraded_ = true;
        name_ = "通宵+";
        description_ = "造成当前手牌数x4点伤害";
    }
    
    void takeEffect(shared_ptr<Creature> target) override {
        int handSize = CombatSystem::getInstance()->getHand().size();
        int multiplier = is_upgraded_ ? 4 : 3;
        int damage = handSize * multiplier;
        cout << "手牌数: " << handSize << "，造成 " << damage << " 点伤害!\n";
        CombatSystem::getInstance()->onAttack(Player::getInstance(), target, damage, name_);
    }
};
AUTO_REGISTER_CARD(AllNighter)

/** 
* @class Second_wind (原有卡牌)
*/
class Second_wind : public Card
{
public:
    Second_wind() : Card("喘息", "消耗所有非攻击牌，每张获得5点格挡", 1, 0, true, SKILL, false, false, false) {
        addEffect(make_shared<SecondWindEffectAdapter>(5));
    }
    
    void upgrade() override {
        is_upgraded_ = true;
        name_ = "喘息+";
        description_ = "消耗所有非攻击牌，每张获得7点格挡";
        clearPlayEffects();
        addEffect(make_shared<SecondWindEffectAdapter>(7));
    }
    
    void takeEffect() override {
        executePlayEffects();
    }
};
AUTO_REGISTER_CARD(Second_wind)