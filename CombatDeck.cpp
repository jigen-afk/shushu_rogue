#include "CombatDeck.h"
#include "Card.h"
#include "Register.h"
#include "RandomGenerator.h"
#include "Player.h"



std::unique_ptr<CombatDeck> CombatDeck::create(std::shared_ptr<RandomGenerator> randomGen)
{
    std::unique_ptr<CombatDeck> deck(new CombatDeck(randomGen));
    return deck;
}

CombatDeck::CombatDeck(std::shared_ptr<RandomGenerator> randomGen)
    : randomGen_(randomGen)
{
}

CombatDeck::~CombatDeck()
{
}

bool CombatDeck::init(std::shared_ptr<Player> player)
{
    if (!player) {
        //std::cout << "[CombatDeck] Error: Player instance is null during deck initialization" << std::endl;
        return false;
    }

    //std::cout << "[CombatDeck] Initializing deck for player: " << player->getName() << std::endl;

    try {
        // Clear existing cards
        while (!drawPile_.empty())
            drawPile_.pop();
        while (!discardPile_.empty())
            discardPile_.pop();
        hand_.clear();

        // Load initial deck from CardRegistry
        // In a real scenario, this should come from Player's inventory
        std::vector<std::string> tempDeck = CardRegistry::getAllCardNames();
        if (tempDeck.empty()) {
            //std::cout << "[CombatDeck] Warning: No cards found in CardRegistry for initialization" << std::endl;
        }

        // Add 4 copies of each card for testing (matching original logic)
        for (int i = 0; i < 4; i++) {
            for (auto name : tempDeck) {
                auto card = CardRegistry::createCard(name);
                if (card) {
                    // Add to discard pile first so shuffle() will move them to draw pile and shuffle
                    addToDiscardPile(card);
                    //std::cout << "[CombatDeck] Added card to discard pile for init: " << name << std::endl;
                }
            }
        }

        //std::cout << "[CombatDeck] Initialized successfully - Draw pile: " << drawPile_.size() << ", Discard pile: " << discardPile_.size() << std::endl;

        // Shuffle the deck initially (moves discard -> draw and shuffles)
        shuffle();

        return true;

    } catch (const std::exception& e) {
        //std::cerr << "[CombatDeck] Exception during initialization: " << e.what() << std::endl;
        return false;
    }
}

int CombatDeck::drawCards(int count)
{
    //std::cout << "[CombatDeck] Drawing " << count << " cards" << std::endl;

    int drawnCount = 0;

    for (int i = 0; i < count; i++) {
        // 检查抽牌堆是否为空
        if (drawPile_.empty()) {
            // 如果抽牌堆为空，洗牌
            if (!discardPile_.empty()) {
                //std::cout << "[CombatDeck] Draw pile empty, shuffling discard pile" << std::endl;
                shuffle();
            } else {
                //std::cout << "[CombatDeck] No cards to draw" << std::endl;
                break;
            }
        }

        // 抽一张卡
        if (!drawPile_.empty()) {
            auto card = drawPile_.front();
            drawPile_.pop();

            // 添加到手牌
            if (addToHand(card)) {
                drawnCount++;
            } else {
                // 添加到手牌失败，放回抽牌堆
                //std::cout << "[CombatDeck] Warning: Failed to add card to hand" << std::endl;
                drawPile_.push(card);
                break;
            }
        } else {
            break;
        }
    }

    //std::cout << "[CombatDeck] Drew " << drawnCount << " cards (hand size: " << hand_.size() << ")" << std::endl;
    return drawnCount;
}

void CombatDeck::addToDrawPile(std::shared_ptr<Card> card, int count)
{
    if (!card) {
        //std::cout << "[CombatDeck] Warning: Attempting to add null card to draw pile" << std::endl;
        return;
    }

    for (int i = 0; i < count; i++) {
        drawPile_.push(card);
    }

    //std::cout << "[CombatDeck] Added " << count << " copies of " << card->getName() << " to draw pile (total draw pile: " << drawPile_.size() << ")" << std::endl;

    //notifyCardChanged("ADDED_TO_DRAW_PILE", card);
}

void CombatDeck::shuffle()
{
    //std::cout << "[CombatDeck] Shuffling discard pile into draw pile" << std::endl;

    // 将弃牌堆转移到临时向量
    std::vector<std::shared_ptr<Card>> tempCards;
    while (!discardPile_.empty()) {
        tempCards.push_back(discardPile_.front());
        discardPile_.pop();
    }

    // 洗牌
    if (randomGen_ && !tempCards.empty()) {
        randomGen_->shuffleVector(tempCards);
    }

    // 加入抽牌堆
    for (const auto& card : tempCards) {
        drawPile_.push(card);
    }

    //std::cout << "[CombatDeck] Shuffle complete (draw pile: " << drawPile_.size() << ", discard pile: " << discardPile_.size() << ")" << std::endl;

    //notifyCardChanged(CardEvent::SHUFFLED, nullptr);
}

bool CombatDeck::addToHand(std::shared_ptr<Card> card)
{
    if (!card) {
        std::cout << "[CombatDeck] Warning: Attempting to add null card to hand" << std::endl;
        return false;
    }

    hand_.push_back(card);
    //std::cout << "[CombatDeck] Added " << card->getName() << " to hand (hand size: " << hand_.size() << ")" << std::endl;

    //notifyCardChanged(CardEvent::ADDED_TO_HAND, card);
    return true;
}

bool CombatDeck::removeFromHand(std::shared_ptr<Card> card)
{
    if (!card) {
        return false;
    }

    bool removed = removeCardFromVector(hand_, card);
    if (removed) {
        std::cout << "[CombatDeck] Removed " << card->getName() << " from hand (hand size: " << hand_.size() << ")" << std::endl;
       // notifyCardChanged(CardEvent::REMOVED_FROM_HAND, card);
    }

    return removed;
}

std::shared_ptr<Card> CombatDeck::removeFromHand(int index)
{
    if (index < 0 || index >= static_cast<int>(hand_.size())) {
        std::cout << "[CombatDeck] Warning: Invalid hand index " << index << " (size: " << hand_.size() << ")" << std::endl;
        return nullptr;
    }

    auto card = hand_[index];
    hand_.erase(hand_.begin() + index);

    //std::cout << "[CombatDeck] Removed " << card->getName() << " from hand at index " << index << std::endl;
    //notifyCardChanged(CardEvent::REMOVED_FROM_HAND, card);

    return card;
}

void CombatDeck::clearHand()
{
    //std::cout << "[CombatDeck] Clearing all hand cards (count: " << hand_.size() << ")" << std::endl;

    for (const auto& card : hand_) {
       // notifyCardChanged(CardEvent::REMOVED_FROM_HAND, card);
    }

    hand_.clear();
}



void CombatDeck::addToDiscardPile(std::shared_ptr<Card> card, int count)
{
    if (!card) {
        std::cout << "[CombatDeck] Warning: Attempting to add null card to discard pile" << std::endl;
        return;
    }

    for (int i = 0; i < count; i++) {
        discardPile_.push(card);
    }

   // std::cout << "[CombatDeck] Added " << count << " copies of " << card->getName() << " to discard pile (total: " << discardPile_.size() << ")" << std::endl;

    //notifyCardChanged("ADDED_TO_DISCARD_PILE", card);
}

bool CombatDeck::discardCard(std::shared_ptr<Card> card)
{
    if (!card) {
        return false;
    }

    // 先从手牌移除
    if (!removeFromHand(card)) {
        return false;
    }

    // 加入弃牌堆
    moveCardToDiscard(card);

    std::cout << "[CombatDeck] Discarded " << card->getName() << std::endl;
    return true;
}

bool CombatDeck::discardCard(int index)
{
    auto card = removeFromHand(index);
    if (card) {
        moveCardToDiscard(card);
        std::cout << "[CombatDeck] Discarded card at index " << index << std::endl;
        return true;
    }
    return false;
}

bool CombatDeck::exhaustCard(std::shared_ptr<Card> card)
{
    if (!card) {
        return false;
    }

    // 从手牌移除但不加入弃牌堆
    if (removeFromHand(card)) {
        std::cout << "[CombatDeck] Exhausted " << card->getName() << std::endl;
        //notifyCardChanged(CardEvent::EXHAUSTED, card);
        return true;
    }

    return false;
}

bool CombatDeck::exhaustCard(int index)
{
    auto card = removeFromHand(index);
    if (card) {
        std::cout << "[CombatDeck] Exhausted card at index " << index << std::endl;
        //notifyCardChanged(CardEvent::EXHAUSTED, card);
        return true;
    }
    return false;
}

bool CombatDeck::upgradeCard(std::shared_ptr<Card> card)
{
    if (!card) {
        return false;
    }

    // 查找卡牌在手牌中的位置
    for (auto& handCard : hand_) {
        if (handCard == card) {
            if (!card->isUpgraded()) {
                card->upgrade();
                std::cout << "[CombatDeck] Upgraded " << card->getName() << std::endl;
                //notifyCardChanged(CardEvent::UPGRADED, card);
            } else {
                //std::cout << "[CombatDeck] Card " << card->getName() << " is already upgraded" << std::endl;
            }
            return true;
        }
    }

    std::cout << "[CombatDeck] Warning: Card not found in hand for upgrade" << std::endl;
    return false;
}

bool CombatDeck::upgradeCard(int index)
{
    if (index < 0 || index >= static_cast<int>(hand_.size())) {
        std::cout << "[CombatDeck] Warning: Invalid hand index for upgrade: " << index << std::endl;
        return false;
    }

    return upgradeCard(hand_[index]);
}

bool CombatDeck::hasCardInHand(std::shared_ptr<Card> card) const
{
    if (!card) {
        return false;
    }

    for (const auto& handCard : hand_) {
        if (handCard == card) {
            return true;
        }
    }

    return false;
}

int CombatDeck::getCardIndexInHand(std::shared_ptr<Card> card) const
{
    if (!card) {
        return -1;
    }

    for (int i = 0; i < static_cast<int>(hand_.size()); i++) {
        if (hand_[i] == card) {
            return i;
        }
    }

    return -1;
}

int CombatDeck::getTotalCardCount() const
{
    return drawPile_.size() + hand_.size() + discardPile_.size();
}

/*void CombatDeck::notifyCardChanged(const std::string& event, std::shared_ptr<Card> card)
{
    if (cardChangedCallback_) {
        try {
            cardChangedCallback_(event, card);
        } catch (const std::exception& e) {
            std::cerr << "[CombatDeck] Exception in card changed callback: " << e.what() << std::endl;
        }
    }
}*/

bool CombatDeck::moveCardToDiscard(std::shared_ptr<Card> card)
{
    if (!card) {
        return false;
    }

    discardPile_.push(card);
    //notifyCardChanged(CardEvent::DISCARDED, card);
    return true;
}

bool CombatDeck::removeCardFromVector(std::vector<std::shared_ptr<Card>>& vec, std::shared_ptr<Card> card)
{
    if (!card) {
        return false;
    }

    auto it = std::find(vec.begin(), vec.end(), card);
    if (it != vec.end()) {
        vec.erase(it);
        return true;
    }

    return false;
}