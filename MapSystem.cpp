#include "MapSystem.h"
#include "RandomGenerator.h"
#include <iostream>
#include <algorithm>

using namespace std;

shared_ptr<MapSystem> MapSystem::instance_ = nullptr;

shared_ptr<MapSystem> MapSystem::getInstance() {
    if (!instance_) {
        instance_ = shared_ptr<MapSystem>(new MapSystem());
    }
    return instance_;
}

MapSystem::MapSystem() : currentLayer_(0), completed_(false), selectedNode_(nullptr) {
}

void MapSystem::generateMap() {
    layers_.clear();
    currentLayer_ = 0;
    completed_ = false;
    selectedNode_ = nullptr;
    
    // 生成每一层
    for (int i = 0; i < TOTAL_LAYERS; i++) {
        generateLayerNodes(i);
    }
    
    cout << "\n========================================\n";
    cout << "       地图已生成！开始你的学期吧！\n";
    cout << "========================================\n";
}

void MapSystem::generateLayerNodes(int layer) {
    vector<MapNode> nodes;
    auto rng = RandomGenerator::getInstance();
    
    if (layer == TOTAL_LAYERS - 1) {
        // 最后一层是Boss
        nodes.push_back(MapNode(NODE_BOSS, "期末考试周"));
    } else if (layer == 0) {
        // 第一层
        nodes.push_back(MapNode(NODE_MONSTER, "早八课程"));
        nodes.push_back(MapNode(NODE_MONSTER, "高数作业"));
        nodes.push_back(MapNode(NODE_EVENT, "校园偶遇"));
    } else if (layer == TOTAL_LAYERS - 2) {
        // Boss前一层
        nodes.push_back(MapNode(NODE_REST, "寝室休息"));
        nodes.push_back(MapNode(NODE_SHOP, "校园超市"));
        nodes.push_back(MapNode(NODE_ELITE, "期中考试"));
    } else {
        // 中间层随机生成2-3个节点
        int nodeCount = rng->getRandomNumber(2, 3);
        
        // 节点类型权重
        vector<pair<NodeType, int>> weights = {
            {NODE_MONSTER, 40},
            {NODE_EVENT, 30},
            {NODE_SHOP, 15},
            {NODE_REST, 10},
            {NODE_ELITE, 5}
        };
        
        // 节点名称池
        vector<string> monsterNames = {"早八课程", "高数作业", "英语听力", "体测800米", "实验报告"};
        vector<string> eventNames = {"食堂偶遇", "快递驿站", "社团活动", "图书馆", "校园小路"};
        vector<string> eliteNames = {"小测验", "课堂展示", "实验答辩"};
        
        for (int i = 0; i < nodeCount; i++) {
            int roll = rng->getRandomNumber(1, 100);
            int cumulative = 0;
            NodeType selectedType = NODE_MONSTER;
            
            for (const auto& w : weights) {
                cumulative += w.second;
                if (roll <= cumulative) {
                    selectedType = w.first;
                    break;
                }
            }
            
            string nodeName;
            switch (selectedType) {
                case NODE_MONSTER:
                    nodeName = monsterNames[rng->getRandomNumber(0, monsterNames.size() - 1)];
                    break;
                case NODE_EVENT:
                    nodeName = eventNames[rng->getRandomNumber(0, eventNames.size() - 1)];
                    break;
                case NODE_ELITE:
                    nodeName = eliteNames[rng->getRandomNumber(0, eliteNames.size() - 1)];
                    break;
                case NODE_SHOP:
                    nodeName = "校园超市";
                    break;
                case NODE_REST:
                    nodeName = "寝室休息";
                    break;
                default:
                    nodeName = "未知";
            }
            
            nodes.push_back(MapNode(selectedType, nodeName));
        }
    }
    
    layers_.push_back(nodes);
}

vector<MapNode>& MapSystem::getCurrentLayerNodes() {
    return layers_[currentLayer_];
}

bool MapSystem::selectNode(int index) {
    auto& nodes = getCurrentLayerNodes();
    if (index < 0 || index >= static_cast<int>(nodes.size())) {
        return false;
    }
    
    selectedNode_ = &nodes[index];
    nodes[index].visited = true;
    return true;
}

void MapSystem::displayMap() {
    cout << "\n========================================\n";
    cout << "              校园地图\n";
    cout << "========================================\n";
    cout << "当前位置: 第 " << (currentLayer_ + 1) << " 周 / 共 " << TOTAL_LAYERS << " 周\n";
    cout << "----------------------------------------\n\n";
    
    // 显示地图概览
    for (int i = 0; i < TOTAL_LAYERS; i++) {
        if (i == currentLayer_) {
            cout << ">>> 第" << (i + 1) << "周: ";
        } else if (i < currentLayer_) {
            cout << "[✓] 第" << (i + 1) << "周: ";
        } else {
            cout << "[ ] 第" << (i + 1) << "周: ";
        }
        
        for (size_t j = 0; j < layers_[i].size(); j++) {
            cout << "[" << getNodeTypeName(layers_[i][j].type) << "]";
            if (j < layers_[i].size() - 1) cout << " ";
        }
        cout << "\n";
    }
    
    cout << "\n----------------------------------------\n";
    cout << "可选路径:\n";
    
    auto& currentNodes = getCurrentLayerNodes();
    for (size_t i = 0; i < currentNodes.size(); i++) {
        cout << (i + 1) << ". " << currentNodes[i].name 
             << " [" << getNodeTypeName(currentNodes[i].type) << "]\n";
    }
}

string MapSystem::getNodeTypeName(NodeType type) {
    switch (type) {
        case NODE_MONSTER: return "战斗";
        case NODE_ELITE: return "精英";
        case NODE_EVENT: return "事件";
        case NODE_SHOP: return "商店";
        case NODE_REST: return "休息";
        case NODE_BOSS: return "BOSS";
        default: return "???";
    }
}