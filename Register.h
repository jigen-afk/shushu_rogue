#pragma once
#include <queue>
#include <vector>
#include <memory>
#include <iostream>
#include <map>
#include <functional>
#include "Card.h"
class Creature;
class Card;
class Relic;
class Buff;
class Monster;


class CardRegistry {
public:
    using CardFactory = std::function<std::shared_ptr<Card>()>;
    
    static void registerCard(const std::string& name, CardFactory factory) {
        getRegistry()[name] = factory;
    }

    
    static std::shared_ptr<Card> createCard(const std::string& name) 
    {
        auto newname = name;
        int upgraded = 0;
        if (name.back() == '+')
        {
            newname.pop_back();
            upgraded = 1;
        }
        auto& registry = getRegistry();
        auto it = registry.find(newname);
        if (it != registry.end()) {
            auto card = it->second();
            if (upgraded)
            {
                card->upgrade();
            }
            return card;
        }
        throw std::runtime_error("Card not found: " + name);
    }

    
    static std::vector<std::string> getAllCardNames() {
        std::vector<std::string> names;
        for (const auto& pair : getRegistry()) {
            names.push_back(pair.first);
        }
        return names;
    }

private:
    static std::map<std::string, CardFactory>& getRegistry() {
        static std::map<std::string, CardFactory> registry;
        return registry;
    }
};


class BuffRegistry {
public:
    using BuffFactory = std::function<std::shared_ptr<Buff>()>;
    
    static void registerBuff(const std::string& name, BuffFactory factory) {
        getRegistry()[name] = factory;
    }

    static std::shared_ptr<Buff> createBuff(const std::string& name) {
        auto& registry = getRegistry();
        auto it = registry.find(name);
        if (it != registry.end()) {
            return it->second();
        }
        throw std::runtime_error("Card not found: " + name);
    }

    
    static std::vector<std::string> getAllBuffNames() {
        std::vector<std::string> names;
        for (const auto& pair : getRegistry()) {
            names.push_back(pair.first);
        }
        return names;
    }

private:
    static std::map<std::string, BuffFactory>& getRegistry() {
        static std::map<std::string, BuffFactory> registry;
        return registry;
    }
};


class RelicRegistry {
public:
    using RelicFactory = std::function<std::shared_ptr<Relic>()>;
    static void registerRelic(const std::string& name, RelicFactory factory) {
        getRegistry()[name] = factory;
    }

    static std::shared_ptr<Relic> createRelic(const std::string& name) {
        auto& registry = getRegistry();
        auto it = registry.find(name);
        if (it != registry.end()) {
            return it->second();
        }
        throw std::runtime_error("Relic not found: " + name);
    }

    static std::vector<std::string> getAllRelicNames() {
        std::vector<std::string> names;
        for (const auto& pair : getRegistry()) {
            names.push_back(pair.first);
        }
        return names;
    }

private:
    static std::map<std::string, RelicFactory>& getRegistry() {
        static std::map<std::string, RelicFactory> registry;
        return registry;
    }
};




class MonsterRegistry {
public:
    using MonsterFactory = std::function<std::shared_ptr<Monster>()>;
    static void registerMonster(const std::string& name, MonsterFactory factory) {
        getRegistry()[name] = factory;
    }

    static std::shared_ptr<Monster> createMonster(const std::string& name) {
        auto& registry = getRegistry();
        auto it = registry.find(name);
        if (it != registry.end()) {
            return it->second();
        }
        throw std::runtime_error("Monster not found: " + name);
    }

    static std::vector<std::string> getAllMonsterNames() {
        std::vector<std::string> names;
        for (const auto& pair : getRegistry()) {
            names.push_back(pair.first);
        }
        return names;
    }

private:
    static std::map<std::string, MonsterFactory>& getRegistry() {
        static std::map<std::string, MonsterFactory> registry;
        return registry;
    }
};
