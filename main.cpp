#include <iostream>
#include "GameManager.h" // 你需要自己封装一个简易管理器

int main() {
    std::cout << "=== Text Slay the Spire ===" << std::endl;
    
    // 初始化玩家
    Player player;
    player.init(80, 80); // 80 HP
    
    // 简单的地图循环
    while (player.isAlive()) {
        std::cout << "\nChoose next room: [1] Monster  [2] Rest  [3] Quit" << std::endl;
        int choice;
        std::cin >> choice;
        
        if (choice == 1) {
            CombatSystem combat;
            combat.startEncounter(player, new Cultist()); //  Cultist 是具体怪物
        } else if (choice == 2) {
            std::cout << "You rested. HP +30." << std::endl;
            player.heal(30);
        } else {
            break;
        }
    }
    
    return 0;
}