#ifndef EVENTSYSTEM_H
#define EVENTSYSTEM_H

#include "Player.h"
#include <memory>

class EventSystem {
public:
    static std::shared_ptr<EventSystem> getInstance();
    
    void triggerRandomEvent();
    void changeCoins(int amount) { coins_ += amount; }
    
    // 全局状态
    int health_;
    int fullHealth_;
    int coins_;
    
private:
    EventSystem() : health_(80), fullHealth_(80), coins_(100) {} //全局变量
    static std::shared_ptr<EventSystem> instance_;
};

#endif