#pragma once
#include<vector>
#include<string>
#include<memory>
#include "Player.h"
using namespace std;
class Creature;
class Effect;
class Player;
class Card : public enable_shared_from_this<Card> //安全地在类内部获取自己的shared_ptr
{
public:
    Card() {};
    /**
     * @param temp_name 卡牌名称
     * @param temp_description 卡牌描述
     * @param temp_energy_cost 卡牌消耗能量
     * @param temp_money_cost 商店购买价格
     * @param temp_can_be_played 卡牌是否能被打出
     * @param temp_type 卡牌类型
     * @param temp_need_target 是否需要选中目标才能打出
     * @param temp_is_exhaust 是否为消耗牌
     * @param temp_is_upgraded 是否是升级的卡牌
    */
    Card(string temp_name, string temp_description, int temp_energy_cost, int temp_money_cost, bool temp_can_be_played, 
        int temp_type, bool temp_need_target_, bool temp_is_exhaust_ = 0,bool temp_is_upgraded_ = 0)
        :name_(temp_name),
        description_(temp_description),
        energy_cost_(temp_energy_cost),
        money_cost_(temp_money_cost),
        can_be_played_(temp_can_be_played),
        type_(temp_type),
        need_target_(temp_need_target_),
        is_exhaust_(temp_is_exhaust_) ,
        is_upgraded_(temp_is_upgraded_){};

    //executeplayeffects和takeeffect 前者执行效果后者卡牌接口，可能会有其他逻辑(?
    virtual void takeEffect() {   // 默认：执行所有出牌效果（无目标）
        executePlayEffects();
    }
    virtual void takeEffect(shared_ptr<Creature> target) {   // 默认：执行所有出牌效果（有目标）
        executePlayEffects(target);
    }

    virtual void takeeffectonturnend(shared_ptr<Card> card) {    // 回合结束效果
        executeTurnEndEffects(card);
    }

    virtual void takeEffectOnDiscard() {   // 被弃置时产生效果
        executeDiscardEffects();
    }

    virtual void takeEffectOnExhaust() {   // 被消耗时产生效果
        executeExhaustEffects();
    }

    virtual void upgrade() {};   //对卡牌进行升级
    
    string getName() const  //返回卡牌名称
    {
        return name_;
    }
    int getEnergyCost() const  //返回卡牌消耗能量值
    {
        return energy_cost_;
    }
    int getType() const  //返回卡牌类型（攻击/技能/能力）
    {
        return type_;
    }
    string getDescription() const  //返回卡牌描述
    {
        return description_;
    }
    int getMoneyCost() const {   //返回购买卡牌需要的金币
        return  money_cost_;
    }
    bool getCanBePlayed() const{   //返回卡牌是否能被打出
        return can_be_played_;
    }

    bool needTarget() const {   //返回卡牌是否需要选中目标才能打出
        return need_target_;
    }
    bool isExhaust() const {   //返回卡牌是否为消耗牌
        return is_exhaust_;
    }

    bool isUpgraded() const {   //返回卡牌是否是升级的卡牌
        return is_upgraded_;
    }

    virtual ~Card() {}                                              
    int tag = 0;
    
    // 添加/执行效果：出牌时
    void addEffect(shared_ptr<Effect> effect) { addPlayEffect(effect); }
    void addPlayEffect(shared_ptr<Effect> effect) {
        if (effect) {
            playEffects_.push_back(effect);
        }
    }
    void clearPlayEffects() { playEffects_.clear(); }
    void executeAllEffects(shared_ptr<Creature> target = nullptr) { executePlayEffects(target); }
    void executePlayEffects(shared_ptr<Creature> target = nullptr) {
        int numeric_value = 0;
        for (auto& effect : playEffects_) {
            effect->execute(Player::getInstance(), target, shared_from_this(), numeric_value);
        }
    }

    // 添加/执行效果：回合结束
    void addTurnEndEffect(shared_ptr<Effect> effect) {
        if (effect) {
            turnEndEffects_.push_back(effect);
        }
    }
    void executeTurnEndEffects(shared_ptr<Card> card) {
        int numeric_value = 0;
        for (auto& effect : turnEndEffects_) {
            effect->execute(Player::getInstance(), nullptr, card, numeric_value);
        }
    }

    // 添加/执行效果：弃置
    void addDiscardEffect(shared_ptr<Effect> effect) {
        if (effect) {
            discardEffects_.push_back(effect);
        }
    }
    void executeDiscardEffects() {
        int numeric_value = 0;
        for (auto& effect : discardEffects_) {
            effect->execute(Player::getInstance(), nullptr, shared_from_this(), numeric_value);
        }
    }

    // 添加/执行效果：消耗
    void addExhaustEffect(shared_ptr<Effect> effect) {
        if (effect) {
            exhaustEffects_.push_back(effect);
        }
    }
    void executeExhaustEffects() {
        int numeric_value = 0;
        for (auto& effect : exhaustEffects_) {
            effect->execute(Player::getInstance(), nullptr, shared_from_this(), numeric_value);
        }
    }
    
protected:
    string name_;                                                //卡牌名称
    string description_;                                         //卡牌描述
    int energy_cost_;                                                 //卡牌消耗能量
    int money_cost_;                                                  //商店购买价格
    //int rarity_;                                                      //稀有度
    bool can_be_played_;                                              //卡牌是否能被打出
    int type_;                                                        //卡牌类型
    bool need_target_;                                                //是否需要选中目标才能打出
    bool is_exhaust_;                                                 //是否为消耗牌
    bool is_upgraded_;                                                //是否是升级的卡牌
    vector<shared_ptr<Effect>> playEffects_;               // 出牌效果列表
    vector<shared_ptr<Effect>> turnEndEffects_;            // 回合结束效果列表
    vector<shared_ptr<Effect>> discardEffects_;            // 弃置效果列表
    vector<shared_ptr<Effect>> exhaustEffects_;            // 消耗效果列表
};