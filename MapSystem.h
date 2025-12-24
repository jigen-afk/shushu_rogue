#pragma once
#include <vector>
#include <string>
#include <memory>

using namespace std;

// 地图节点类型
enum NodeType {
    NODE_MONSTER = 0,    // 普通战斗
    NODE_ELITE = 1,      // 精英战斗
    NODE_EVENT = 2,      // 随机事件
    NODE_SHOP = 3,       // 商店
    NODE_REST = 4,       // 休息点
    NODE_BOSS = 5        // Boss战
};

// 地图节点
struct MapNode {
    NodeType type;
    string name;
    bool visited;
    
    MapNode(NodeType t, const string& n) : type(t), name(n), visited(false) {}
};

class MapSystem {
public:
    static shared_ptr<MapSystem> getInstance();
    
    // 生成地图
    void generateMap();
    
    // 获取当前层的可选节点
    vector<MapNode>& getCurrentLayerNodes();
    
    // 选择节点并移动到下一层
    bool selectNode(int index);
    
    // 推进到下一层
    void advanceLayer() { 
        if (currentLayer_ < TOTAL_LAYERS - 1) {
            currentLayer_++; 
        }
    }
    
    // 获取当前层数
    int getCurrentLayer() const { return currentLayer_; }
    
    // 获取总层数
    int getTotalLayers() const { return TOTAL_LAYERS; }
    
    // 是否到达Boss层
    bool isBossLayer() const { return currentLayer_ == TOTAL_LAYERS - 1; }
    
    // 是否通关
    bool isCompleted() const { return completed_; }
    
    // 显示当前地图状态
    void displayMap();
    
    // 获取当前选中的节点
    MapNode* getSelectedNode() { return selectedNode_; }
    
private:
    MapSystem();
    static shared_ptr<MapSystem> instance_;
    
    static const int TOTAL_LAYERS = 6;  // 5个普通层 + 1个Boss层
    vector<vector<MapNode>> layers_;     // 每层的节点
    int currentLayer_;
    bool completed_;
    MapNode* selectedNode_;
    
    // 根据层数生成节点
    void generateLayerNodes(int layer);
    
    // 获取节点类型的中文名
    string getNodeTypeName(NodeType type);
};