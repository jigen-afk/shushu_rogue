#pragma once
#include <vector>
#include <string>
#include <memory>

using namespace std;

class Card;
class Relic;

// 商品类型
enum ItemType {
    ITEM_CARD,
    ITEM_RELIC,
    ITEM_REMOVE_CARD,
    ITEM_UPGRADE_CARD,
    ITEM_HEAL
};

// 商品结构
struct ShopItem {
    ItemType type;
    string name;
    string description;
    int price;
    bool sold;
    shared_ptr<Card> card;
    shared_ptr<Relic> relic;
    
    ShopItem() : type(ITEM_CARD), price(0), sold(false), card(nullptr), relic(nullptr) {}
};

class ShopSystem {
public:
    static shared_ptr<ShopSystem> getInstance();
    
    // 生成商店物品
    void generateShopItems();
    
    // 显示商店界面
    void displayShop();
    
    // 购买物品
    bool purchaseItem(int index);
    
    // 获取商品列表
    vector<ShopItem>& getItems() { return items_; }
    
    // 进入商店
    void enterShop();
    
private:
    ShopSystem();
    static shared_ptr<ShopSystem> instance_;
    
    vector<ShopItem> items_;
    
    // 计算卡牌价格
    int calculateCardPrice(shared_ptr<Card> card);
    
    // 计算遗物价格
    int calculateRelicPrice(shared_ptr<Relic> relic);
};