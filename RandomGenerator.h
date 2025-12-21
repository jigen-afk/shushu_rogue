#pragma once

#include <iostream>
#include <random>
#include <string>
#include <memory>
#include <vector>
#include <time.h>
#include <functional>
#include "Enum.h"
using namespace std;

/*enum RandomType {
    CARD = 0,
    COMBAT = 1,
    OTHER = 2
};*/

class Card;
class Relic;
class Monster;


class RandomGenerator
{
public:
    static RandomGenerator* getInstance();

    //保护单例性
    RandomGenerator(const RandomGenerator&) = delete;
    RandomGenerator& operator=(const RandomGenerator&) = delete;

    void initializeGenerators(string seeds);
    void initializeGenerators();
    unsigned int getInitialSeed();
    int getRandomNumber(int min, int max) ;
    shared_ptr<Card> getRandomCard(int type) ;
    shared_ptr<Monster> getRandomMonster(int type = NORMAL) ;
    shared_ptr<Relic> getRandomRelic(int rarity) ;

    //洗牌
    template<typename Elemtype>
    void shuffleVector(vector<Elemtype>& vec);

private:
    RandomGenerator() = default;
    unsigned int initialSeed_;
    mt19937 rng[3]; // 0: CARD, 1: COMBAT, 2: OTHER
};