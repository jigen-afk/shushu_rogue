#pragma once
#include <memory>

class Creature;
class Card;

class Effect {
public:
    virtual ~Effect() = default;
    virtual void execute(std::shared_ptr<Creature> source,
                       std::shared_ptr<Creature> target,
                       std::shared_ptr<Card> card,
                       int& numeric_value) = 0;
};