#include "EventSystem.h"
#include "RandomGenerator.h"
#include "Player.h"
#include "CombatSystem.h"
#include "Register.h"
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

struct Event {
    string title;
    string description;
    vector<string> options;
    vector<function<void(EventSystem*)>> outcomes;
};

void EventSystem::triggerRandomEvent() {
    vector<Event> events = {
        {
            "修路施工",
            "学校又开始修路了！到处都是围栏，你要迟到了！",
            {
                "1. 随大流迟到 (失去3点生命)",
                "2. 翘课吃顿好的 (花10金币，30%概率恢复15点生命)"
            },
            {
                [](EventSystem* es) { 
                    es->health_ -= 3;
                    if (es->health_ < 0) es->health_ = 0;
                    cout << "\n你迟到了，被老师点名批评，心情不好。失去3点生命。\n";
                },
                [](EventSystem* es) {
                    if (es->coins_ < 10) {
                        cout << "\n金币不足！只能硬着头皮去上课...\n";
                        es->health_ -= 3;
                        return;
                    }
                    es->coins_ -= 10;
                    cout << "\n你决定翘课，去食堂美餐一顿...\n";
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
            "期末周图书馆一座难求！",
            {
                "1. 早起抢座 (失去5点生命，获得一张【临时抱佛脚】)",
                "2. 去网吧学习 (花15金币，恢复8点生命)"
            },
            {
                [](EventSystem* es) {
                    es->health_ -= 5;
                    if (es->health_ < 0) es->health_ = 0;
                    cout << "\n你早上5点起床抢座，虽然成功了但累得半死。失去5点生命。\n";
                    auto card = CardRegistry::createCard("LastMinuteReview");
                    if (card) {
                        CombatSystem::getInstance()->addToDeck(card);
                        cout << "获得卡牌【临时抱佛脚】！\n";
                    }
                },
                [](EventSystem* es) {
                    if (es->coins_ < 15) {
                        cout << "\n金币不足！只能在寝室躺平了...\n";
                        return;
                    }
                    es->coins_ -= 15;
                    int heal = min(8, es->fullHealth_ - es->health_);
                    es->health_ += heal;
                    cout << "\n网吧空调真凉快，学习效率意外地高！恢复" << heal << "点生命。\n";
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
            "社团招新",
            "学长学姐热情地向你招手：\"来加入我们社团吧！\"",
            {
                "1. 加入社团 (花30金币，获得一张稀有卡)",
                "2. 礼貌拒绝 (获得15金币)"
            },
            {
                [](EventSystem* es) {
                    if (es->coins_ < 30) {
                        cout << "\n交不起社费啊...\n";
                        return;
                    }
                    es->coins_ -= 30;
                    auto card = CardRegistry::createCard("CoffeePower");
                    if (card) {
                        CombatSystem::getInstance()->addToDeck(card);
                        cout << "\n你加入了咖啡社，获得卡牌【咖啡续命】！\n";
                    }
                },
                [](EventSystem* es) {
                    es->coins_ += 15;
                    cout << "\n你婉拒了邀请，学长无奈地给了你15金币的传单费。\n";
                }
            }
        },
        {
            "食堂阿姨",
            "今天食堂阿姨心情特别好！",
            {
                "1. 点最贵的菜 (花5金币，恢复20点生命)",
                "2. 要一份免费汤 (恢复5点生命)"
            },
            {
                [](EventSystem* es) {
                    if (es->coins_ < 5) {
                        cout << "\n钱不够点好菜...\n";
                        return;
                    }
                    es->coins_ -= 5;
                    int heal = min(20, es->fullHealth_ - es->health_);
                    es->health_ += heal;
                    cout << "\n阿姨大手一挥，给你打了满满一盘！恢复" << heal << "点生命！\n";
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
            "你有个快递要取，但在六楼没电梯！",
            {
                "1. 立即去取 (失去4点生命，获得25金币)",
                "2. 让它再躺一天 (无事发生)"
            },
            {
                [](EventSystem* es) {
                    es->health_ -= 4;
                    if (es->health_ < 0) es->health_ = 0;
                    es->coins_ += 25;
                    cout << "\n你气喘吁吁地爬完六楼，失去4点生命。\n";
                    cout << "但快递里是家里寄来的生活费！获得25金币！\n";
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
            "寝室夜战",
            "半夜室友开黑打游戏，吵得你睡不着！",
            {
                "1. 加入他们 (花10金币，获得一张卡牌)",
                "2. 戴耳塞忍耐 (失去6点生命)"
            },
            {
                [](EventSystem* es) {
                    if (es->coins_ < 10) {
                        cout << "\n你没钱开会员，只能看着...\n";
                        es->health_ -= 6;
                        return;
                    }
                    es->coins_ -= 10;
                    auto card = CardRegistry::createCard("AllNighter");
                    if (card) {
                        CombatSystem::getInstance()->addToDeck(card);
                        cout << "\n你加入了战局，通宵达旦！获得卡牌【通宵】！\n";
                    }
                },
                [](EventSystem* es) {
                    es->health_ -= 6;
                    if (es->health_ < 0) es->health_ = 0;
                    cout << "\n你戴着耳塞辗转反侧，一夜没睡好。失去6点生命。\n";
                }
            }
        },
        {
            "学霸请教",
            "学霸正在图书馆学习，要不要去请教问题？",
            {
                "1. 厚着脸皮请教 (获得卡牌升级)",
                "2. 自己琢磨 (恢复10点生命)"
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
                    cout << "\n你独自思考，感觉大脑得到了休息。恢复" << heal << "点生命。\n";
                }
            }
        },
        {
            "奖学金公示",
            "奖学金名单公示了！",
            {
                "1. 去看看 (50%概率获得50金币，50%概率失去5生命)",
                "2. 不看了，眼不见为净 (无事发生)"
            },
            {
                [](EventSystem* es) {
                    cout << "\n你紧张地走到公告栏前...\n";
                    if (RandomGenerator::getInstance()->getRandomNumber(1, 100) <= 50) {
                        es->coins_ += 50;
                        cout << "有你的名字！！获得50金币！\n";
                    } else {
                        es->health_ -= 5;
                        if (es->health_ < 0) es->health_ = 0;
                        cout << "没有你...心态崩了。失去5点生命。\n";
                    }
                },
                [](EventSystem* es) {
                    cout << "\n你决定保持内心的平静。\n";
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
        event.outcomes[choice - 1](this);
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