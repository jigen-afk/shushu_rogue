#include "RandomGenerator.h"

using namespace std;


//0.卡牌：需要一个生成器返回一张随机卡牌,可以制定类型
//1.战斗：用于战斗中的随机数，比如洗牌
//2.其余：用于生成其他资源

/*
* 需要功能：
* 1.返回一张随机卡牌->1
* 2.返回一个随机药水->3
* 3.返回一个怪物->3
* 4.返回一个随机遗物->3
* 5.返回一个可以指定上下限的数字->3
* 6.给一个数组，返回一个元素被打乱顺序的数组->2
*/
RandomGenerator* RandomGenerator::getInstance()
{
	static RandomGenerator instance;
	return &instance;
}

//给定一个字符串，根据字符串设定随机种子并初始化随机种子生成器
void RandomGenerator::initializeGenerators(string seeds)
{
	hash<string> hasher;
	initialSeed_ = static_cast<unsigned int>(hasher(seeds));
	rng[0].seed(initialSeed_);
	rng[1].seed(initialSeed_);
	rng[2].seed(initialSeed_);
}

void RandomGenerator::initializeGenerators()
{
	srand(time(nullptr));
	initialSeed_ = rand() % INT_MAX;
	rng[0].seed(initialSeed_);
	rng[1].seed(initialSeed_);
	rng[2].seed(initialSeed_);
}

//返回设定的种子
unsigned int RandomGenerator::getInitialSeed()
{
	return initialSeed_;
}

//返回一个[min，max]的数
int RandomGenerator::getRandomNumber(int min, int max)
{
	//使用 OTHER 随机生成器
	uniform_int_distribution<int> dist(min, max);
	return dist(rng[2]);
}

/** 
* @brief 从所有卡牌中返回一个可指定类型的卡牌
* @param 卡牌种类，默认为不指定
*/
shared_ptr<Card> RandomGenerator::getRandomCard(int type)
{
	// 使用CARD随机生成器
	vector<string> allCardNames = CardRegistry::getAllCardNames();

	if (allCardNames.empty()) {
		throw runtime_error("No cards registered");
	}
	while (1) //循环直到找到合适的卡牌
	{
		// 使用均匀分布来生成一个随机索引
		uniform_int_distribution<> dist(0, allCardNames.size() - 1);
		int randomIndex = dist(rng[0]);

		// 获取随机卡牌名称
		string randomCardName = allCardNames[randomIndex];
		// 创建对应的卡牌对象
		auto card = CardRegistry::createCard(randomCardName);

		//如果满足要求则进行返回
		if ((type == ALL || type == card->getType())  && card->getType() != STATUS) //所有或指定类型，非状态牌
		{
			return card;
		}
	}
}

shared_ptr<Monster> RandomGenerator::getRandomMonster(int type)
{
	
	vector<string> allMonsterNames = MonsterRegistry::getAllMonsterNames();

	if (allMonsterNames.empty()) {
		throw runtime_error("No Monster registered");
	}
	while (1)
	{
		// 使用均匀分布来生成一个随机索引
		uniform_int_distribution<> dist(0, allMonsterNames.size() - 1);
		int randomIndex = dist(rng[2]);

		// 获取随机卡牌名称
		string randomMonsterName = allMonsterNames[randomIndex];    
		// 创建对应的卡牌对象
		auto monster = MonsterRegistry::createMonster(randomMonsterName);

		//如果满足要求则进行返回
		if ((type == ALL || type == monster->getType()))
		{
			return monster;
		}
	}
}

shared_ptr<Relic> RandomGenerator::getRandomRelic(int rarity)
{
	// 使用OTHER随机生成器
	vector<string> allRelicNames = RelicRegistry::getAllRelicNames();

	if (allRelicNames.empty()) {
		throw runtime_error("No cards registered");
	}
	while (1)
	{
		// 使用均匀分布来生成一个随机索引
		uniform_int_distribution<> dist(0, allRelicNames.size() - 1);
		int randomIndex = dist(rng[2]);

		// 获取随机遗物名称
		string randomRelicName = allRelicNames[randomIndex];
		// 创建对应的遗物对象
		auto relic = RelicRegistry::createRelic(randomRelicName);

		// 如果满足要求则进行返回
		if ((rarity == ALL || rarity == relic->getRarity()))
		{
			return relic;
		}
	}
}


template<typename Elemtype>
void RandomGenerator::shuffleVector(std::vector<Elemtype>& vec)
{
	for (int i = vec.size()- 1; i > 0; i--)
	{
		uniform_int_distribution<> dist(0, i);
		int j = dist(rng[1]);
		swap(vec[i], vec[j]);
	}
}
template void RandomGenerator::shuffleVector<shared_ptr<Card>>(vector<shared_ptr<Card>>& vec);