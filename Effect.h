#pragma once
#include <memory>

class Creature;
class Card;

class Effect {
public:
    virtual ~Effect() = default;
    virtual void execute(std::shared_ptr<Creature> source = nullptr,
                       std::shared_ptr<Creature> target = nullptr,
                       std::shared_ptr<Card> card = nullptr,
                       int& numeric_value) = 0;
};