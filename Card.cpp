//#include "IncludeAll.h"
#include "Card.h"
#include "Enum.h"
#include "AttackEffectAdapter.h"
#include "BlockEffectAdapter.h"
#include "SecondWindEffectAdapter.h"
#include "ApotheosisEffectAdapter.h"
#include "TurnEndExhaustEffectAdapter.h"
#include "TurnEndDamageEffectAdapter.h"
#include "TrounceEffectAdapter.h"
#include "Register.h"

/** 
* @class Attack
* @brief 对于选中的敌人造成6点伤害
 */
class Attack : public Card
{
public:
    Attack() : Card("Attack", "Deal 6 damage", 1, 20, PLAYABLE, ATTACK, YES, NO, NO) {
        addEffect(std::make_shared<AttackEffectAdapter>(6));
    }
    void upgrade() {
        is_upgraded_ = 1;
        name_ += '+';
        description_ = "Deal 9 damage";
        // 移除旧效果，添加新效果
        clearPlayEffects();
        addEffect(std::make_shared<AttackEffectAdapter>(9));
    }
    void takeEffect(std::shared_ptr<Creature> target)
    {
        executePlayEffects(target);
    }
};
//进行卡牌注册
AUTO_REGISTER_CARD(Attack)

/** 
* @class Defense
* @brief 增加5点防御
 */
class Defense : public Card
{
public:
    Defense() : Card("Defense", "Gain 5 block", 1, 20, PLAYABLE, SKILL, NO, NO, NO) {
        addEffect(std::make_shared<BlockEffectAdapter>(5));
    }
    void upgrade() {
        is_upgraded_ = 1;
        name_ += '+';
        description_ = "Gain 8 block";
        // 移除旧效果，添加新效果
        clearPlayEffects();
        addEffect(std::make_shared<BlockEffectAdapter>(8));
    }
    void takeEffect()
    {
        cout << "Defense played!" << endl;
        executePlayEffects();
    }
};
//进行卡牌注册
AUTO_REGISTER_CARD(Defense)


/** 
* @class Second_wind
* @brief 消耗所有非攻击牌，每张获得5点格挡
*/
class Second_wind : public Card
{
public:
    Second_wind() : Card("Second_wind", "Consume all non attack cards and receive 5 grid blocks per card", 
        1, 20, PLAYABLE, SKILL, NO, NO, NO) {
        addEffect(std::make_shared<SecondWindEffectAdapter>(5));
    }
    void upgrade() {
        is_upgraded_ = 1;
        name_ += '+';
        description_ = "Consume all non attack cards and receive 7 grid blocks per card";
        // 移除旧效果，添加新效果
        clearPlayEffects();
        addEffect(std::make_shared<SecondWindEffectAdapter>(7));
    }
    void takeEffect()
    {
        executePlayEffects();
    }
};
// 进行卡牌注册
AUTO_REGISTER_CARD(Second_wind)



/*
* 卡牌名称：神话
* 效果：临时升级当前战斗中的所有牌
*/
class Apotheosis :public Card
{
public:
    Apotheosis() : Card("Apotheosis", "Upgrade ALL of your cards for the rest of combat.Exhaust.",
        2, 180, PLAYABLE, SKILL, NO, YES, NO) {
        addEffect(std::make_shared<ApotheosisEffectAdapter>());
    };
    void upgrade() {
        is_upgraded_ = 1;
        name_ += '+';
        energy_cost_ -= 1;
    }
    void takeEffect()
    {
        executePlayEffects();
    }
};
AUTO_REGISTER_CARD(Apotheosis)


/** 
* @class dazed
* @brief 不能被打出，虚无
*/
class dazed :public Card
{
public:
    dazed() : Card("dazed", "Cannot be hit",
        0, 0, NOT_PLAYABLE, ABILITY, NO, NO, NO) {
        addTurnEndEffect(std::make_shared<TurnEndExhaustEffectAdapter>());
    };
    void takeeffectonturnend(std::shared_ptr<Card> card) {
        cout << "discard dazed" << endl;
        executeTurnEndEffects(card);
    }
};
AUTO_REGISTER_CARD(dazed)


/**
* @class burn
* @brief 不能被打出，回合结束造成两点伤害
*/
class burn :public Card
{
public:
    burn() : Card("burn", "Cannot be hit, causing two points of damage at the end of the turn",
        0, 0, NOT_PLAYABLE, ABILITY, NO, NO, NO) {
        addTurnEndEffect(std::make_shared<TurnEndDamageEffectAdapter>(2));
    };
    void takeeffectonturnend(std::shared_ptr<Card> card) {
        executeTurnEndEffects(card);
    }
};
AUTO_REGISTER_CARD(burn)


/** 
* @class trounce
* @brief 造成8点伤害。给予2层易伤。
*/
class trounce :public Card
{
public:
    trounce() : Card("trounce", "Causing 8 points of damage. Give 2 layers of vulnerability.",
        2, 40, PLAYABLE, ATTACK, YES, NO, NO) {
        addEffect(std::make_shared<TrounceEffectAdapter>(8, 2, 2, 1));
    };
    void upgrade() {
        is_upgraded_ = 1;
        name_ += '+';
        description_ = "Causing 10 points of damage. Give 3 layers of vulnerability.";
        clearPlayEffects();
        addEffect(std::make_shared<TrounceEffectAdapter>(8, 2, 2, 1));
    }
    void takeEffect(std::shared_ptr<Creature> target)
    {
        executePlayEffects(target);
    }
};
AUTO_REGISTER_CARD(trounce)