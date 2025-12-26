#include "EventSystem.h"
#include "RandomGenerator.h"
#include "Player.h"
#include "CombatSystem.h"
#include "Register.h"
#include "Relic.h"
#include <iostream>
#include <vector>
#include <functional>

using namespace std;

shared_ptr<EventSystem> EventSystem::instance_ = nullptr;

shared_ptr<EventSystem> EventSystem::getInstance() {
    if (!instance_) {
        instance_ = shared_ptr<EventSystem>(new EventSystem());
    }
    return instance_;
}

// 事件结构体定义:title, description, options, outcomes
struct Event {
    string title;
    string description;
    vector<string> options;
    vector<function<void(EventSystem*)>> outcomes; //每个选项对应的结果函数
};

void EventSystem::triggerRandomEvent() {
    vector<Event> events = {
        {
            "修路施工",
            "布豪！南区在施工，乌泱泱堵了一片！",
            {
                "1. 踩着铃声进教室 (无事发生)",
                "2. 干脆右转先去吾馨吃顿热的 (花5金币，30%概率恢复15点生命)"
            },
            {
                [](EventSystem* es) {  //单例模式，传入指针
                    
                    cout << "\n除了心率提升之外无事发生。\n";
                },
                [](EventSystem* es) {
                    if (es->coins_ < 5) {
                        cout << "\n金币不足！只能硬着头皮去上课...\n";
                        es->health_ -= 3;
                        return;
                    }
                    es->coins_ -= 10;
                    cout << "\n你决定先去食堂饱餐一顿...\n";
                    if (RandomGenerator::getInstance()->getRandomNumber(1, 100) <= 30) {
                        int heal = min(15, es->fullHealth_ - es->health_);
                        es->health_ += heal;
                        cout << "心情大好！恢复" << heal << "点生命！\n";
                    } else {
                        cout << "虽然吃得不错，但总觉得有点心虚...\n";
                    }
                }
            }
        },
        {
            "图书馆占座",
            "期末周本图、钱图一座难求！",
            {
                "1. 早起抢座 (失去5点生命，获得遗物【RewardEverytime】)",
                "2. 回家吧孩子回家吧 (恢复3点生命)"
            },
            {
                [](EventSystem* es) {
                    es->health_ -= 5;
                    if (es->health_ < 0) es->health_ = 0;
                    cout << "\n你早起抢座，虽然成功了但累得半死。失去5点生命。\n";
                    auto reward = RelicRegistry::createRelic("RewardEverytime");
                    if (reward) {
                        reward->onGetEnergy(1); 
                        cout << "获得遗物【RewardEverytime】芋儿鸡yami！永久增加1点能量\n";
                    }
                },
                [](EventSystem* es) {
                    int heal = min(3, es->fullHealth_ - es->health_);
                    es->health_ += heal;
                    cout << "\n回家复习效率降低，但好吃好喝~ 恢复" << heal << "点生命。\n";
                }
            }
        },
        {
            "室友请求",
            "室友让你帮忙带饭！",
            {
                "1. 帮忙带 (花8金币，50%获得20金币)",
                "2. 委婉拒绝 (无事发生)"
            },
            {
                [](EventSystem* es) {
                    if (es->coins_ < 8) {
                        cout << "\n你连自己的饭钱都不够...\n";
                        return;
                    }
                    es->coins_ -= 8;
                    cout << "\n你帮室友带了饭...\n";
                    if (RandomGenerator::getInstance()->getRandomNumber(1, 100) <= 50) {
                        es->coins_ += 20;
                        cout << "室友非常感谢，给了你20金币！净赚12金币！\n";
                    } else {
                        cout << "室友说下次请你，但你知道不会有下次...\n";
                    }
                },
                [](EventSystem* es) {
                    cout << "\n你委婉拒绝了，继续做自己的事。\n";
                }
            }
        },
        {
            "SRM招新",
            "学长学姐热情地向你招手：\"来打robomaster吧!(凌晨五点正是RMer奋斗的时候.jpg\"",
            {
                "1. 去打比赛 (花30金币，获得一张稀有卡)",
                "2. 礼貌拒绝 (获得15金币*)"
            },
            {
                [](EventSystem* es) {
                    if (es->coins_ < 30) {
                        cout << "\n什么，你的意思是板子要先自己垫吗...\n";
                        return;
                    }
                    es->coins_ -= 30;
                    auto card = CardRegistry::createCard("CoffeePower");
                    if (card) {
                        CombatSystem::getInstance()->addToDeck(card);
                        cout << "\n你成为了正式队员，获得卡牌【咖啡续命】！\n";
                    }
                },
                [](EventSystem* es) {
                    cout << "\n想啥呢，赞助都要自己拉怎么可能给你钱呢🤡\n";
                }
            }
        },
        {
            "食堂阿姨",
            "今天食堂阿姨心情特别好！",
            {
                "1. 点最贵的菜 (花20金币，恢复当前生命值的50%)",
                "2. 要一份免费汤 (恢复5点生命)"
            },
            {
                [](EventSystem* es) {
                    if (es->coins_ < 20) {
                        cout << "\n钱不够点好菜...\n";
                        return;
                    }
                    es->coins_ -= 20;
                    int heal = min(es->health_/2 , es->fullHealth_ - es->health_);
                    es->health_ += heal;
                    cout << "\n阿姨大手一挥，给你打了满满一盘！恢复一半生命！\n";
                },
                [](EventSystem* es) {
                    int heal = min(5, es->fullHealth_ - es->health_);
                    es->health_ += heal;
                    cout << "\n阿姨给你盛了一大碗汤，暖暖的。恢复" << heal << "点生命。\n";
                }
            }
        },
        {
            "快递到了",
            "西门快递到了，但你在东区遥遥相望！",
            {
                "1. 立即去取 (失去4点生命，获得25金币)",
                "2. 让它再躺一天 (无事发生)"
            },
            {
                [](EventSystem* es) {
                    es->health_ -= 4;
                    if (es->health_ < 0) es->health_ = 0;
                    es->coins_ += 25;
                    cout << "\n横跨校园东西南，失去4点生命。\n";
                    cout << "但快递里是家里寄来的生活用品！获得25金币！\n";
                },
                [](EventSystem* es) {
                    cout << "\n算了，明天再说吧...\n";
                }
            }
        },
        {
            "传单推销",
            "路上遇到发传单的：\"办卡吗同学？超值优惠！\"",
            {
                "1. 办一张看看 (花15金币，60%获得35金币返利)",
                "2. 快步离开 (无事发生)"
            },
            {
                [](EventSystem* es) {
                    if (es->coins_ < 15) {
                        cout << "\n你掏了掏口袋，尴尬地走开了...\n";
                        return;
                    }
                    es->coins_ -= 15;
                    cout << "\n你办了一张卡...\n";
                    if (RandomGenerator::getInstance()->getRandomNumber(1, 100) <= 60) {
                        es->coins_ += 35;
                        cout << "居然真的有优惠！返利35金币，净赚20！\n";
                    } else {
                        cout << "返利活动已结束...亏了15金币。\n";
                    }
                },
                [](EventSystem* es) {
                    cout << "\n你假装打电话快步离开。\n";
                }
            }
        },
        {
            "南12不眠之夜",
            "半夜室友大卷特卷，心理委员我不得劲！",
            {
                "1. 加入他们 (失去6点生命，获得一张卡牌)",
                "2. 戴耳塞忍耐 (失去3点生命)"
            },
            {
                [](EventSystem* es) {
                    if (es->health_ < 6) {
                        cout << "\n你没力气加入，只能看着...\n";
                        return;
                    }
                    es->health_ -= 6;
                    if (es->health_ < 0) es->health_ = 0;
                    auto card = CardRegistry::createCard("AllNighter");
                    if (card) {
                        CombatSystem::getInstance()->addToDeck(card);
                        cout << "\n你加入了战局，通宵达旦！获得卡牌【通宵】！\n";
                    }
                },
                [](EventSystem* es) {
                    es->health_ -= 3;
                    if (es->health_ < 0) es->health_ = 0;
                    cout << "\n辗转反侧，一夜没睡好。失去3点生命。\n";
                }
            }
        },
        {
            "学霸请教",
            "学霸正在图书馆学习，要不要去请教问题？",
            {
                "1. 厚着脸皮请教 (获得随机卡牌升级)",
                "2. 图书馆禁止交谈 (恢复10点生命)"
            },
            {
                [](EventSystem* es) {
                    auto hand = CombatSystem::getInstance()->deck;
                    if (!hand.empty()) {
                        int idx = RandomGenerator::getInstance()->getRandomNumber(0, hand.size() - 1);
                        hand[idx]->upgrade();
                        cout << "\n学霸耐心地给你讲解，【" << hand[idx]->getName() << "】得到了升级！\n";
                    } else {
                        cout << "\n你发现自己没什么好问的...\n";
                    }
                },
                [](EventSystem* es) {
                    int heal = min(10, es->fullHealth_ - es->health_);
                    es->health_ += heal;
                    cout << "\n好吧，其实是你的水平还不足以支持你提出问题。沉淀后恢复" << heal << "点生命。\n";
                }
            }
        },
        {
            "奖学金公示",
            "奖学金名单公示了！",
            {
                "1. 去看看 (50%概率获得100金币*，50%概率失去5生命*)",
                "2. 不看了，眼不见为净 (无事发生*)"
            },
            {
                [](EventSystem* es) {
                    cout << "\n你紧张地走到公告栏前...\n";
                    if (RandomGenerator::getInstance()->getRandomNumber(1, 100) <= 50) {
                        es->coins_ += 1;
                        cout << "有你的名字！！获得100金币！\n";
                        cout << "Hold on，作者表示自己都没得过奖学金，一不小心多写了两个零嘻嘻~\n";
                        
                    } else {
                        es->health_ -= 1;
                        cout << "没有你...心态崩了。失去5点生命。\n";
                        cout << "骗你的，作者表示自己都没得过奖学金，安心玩游戏吧~\n";
                        cout << "(但还是扣一滴血意思一下😈) \n";
                    }
                },
                [](EventSystem* es) {
                    cout << "\n(其实是连申请都没写。\n";
                    cout << "作者表示你我皆是同道中人，奖励你99金币~\n什么？你问为什么不是100？自己去看源码啦~\n";
                    es->coins_ += 99;
                }
            }
        }
    };
    
    int eventIndex = RandomGenerator::getInstance()->getRandomNumber(0, events.size() - 1);
    Event& event = events[eventIndex];
    
    cout << "\n╔════════════════════════════════════════╗\n";
    cout << "║            【" << event.title << "】\n";
    cout << "╚════════════════════════════════════════╝\n\n";
    cout << event.description << "\n\n";
    
    for (const auto& option : event.options) {
        cout << option << "\n";
    }
    cout << "\n请选择: ";
    
    int choice;
    cin >> choice;
    if (cin.fail()) {
        cin.clear();
        cin.ignore(10000, '\n');
        choice = 1;
    }
    
    if (choice >= 1 && choice <= static_cast<int>(event.outcomes.size())) {
        event.outcomes[choice - 1](this); // this指向es指针，调用对应结果函数
    } else {
        cout << "\n无效选择，默认选择1\n";
        event.outcomes[0](this);
    }
    
    // 确保生命值不会低于1（除非真的死了）
    if (health_ <= 0) {
        health_ = 0;
    }
    
    cout << "\n----------------------------------------\n";
    cout << "当前状态 - 生命: " << health_ << "/" << fullHealth_ 
         << " | 金币: " << coins_ << "\n";
    cout << "----------------------------------------\n";
}