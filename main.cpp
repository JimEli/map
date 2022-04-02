#include <iostream>
#include "RBTree.h"
#include "Map.h"

/*
template<typename T>
struct GenericCompare 
{
    int operator ()(const T& left, const T& right) const 
    {
        if (left < right) 
            return -1;
        else if (left > right)
            return 1;
        else 
            return 0;
    }
};
*/
/*
using DataNode_t = RBTreeNode<int>;
using RBTree_t = RBTree<DataNode_t, Compare_t>;

void PrintRBTree(RBTree_t& rbTree) 
{
    std::cout << "PrintRBTree\n";
    std::cout << "Size: " << rbTree.size() << "\n";
    
    DataNode_t* node = rbTree.first();
    while (!isNull(node)) 
    {
        std::cout << "Value: " << node->_value << "\n";
        node = rbTree.forward(node);
    }
}

void TestRBTree() 
{
    std::cout << "PrintRBTree\n";

    DataNode_t a, b, c;
    a._value = 1;
    b._value = 3;
    c._value = 2;

    RBTree<DataNode_t, Compare_t> rbTree;
    rbTree.insertUnique(&a);
    rbTree.insertUnique(&b);
    rbTree.insertUnique(&c);

    PrintRBTree(rbTree);

    auto first = rbTree.first();
    auto last = rbTree.last();
    auto target = rbTree.search(2);
    auto backward = rbTree.backward(target);
    auto forward = rbTree.forward(target);

    rbTree.remove(&c);
    first = rbTree.first();
    last = rbTree.last();

    PrintRBTree(rbTree);
}
*/

//void PrintMap(Map<int, int, GenericCompare<int>>& map)
void PrintMap(Map<int, int>& map)
{
    std::cout << "PrintMap\n";
    std::cout << "Size: " << map.size() << "\n";

    for (auto iter = map.begin(); iter != map.end(); ++iter) 
        std::cout << "Key: " << iter->first << " Value: " << iter->second << "\n";
}

void TestMap() 
{
    std::cout << "TestMap\n";

//    Map<int, int, GenericCompare<int>> map;
    Map<int, int> map;
    map.insert(1, 101);
    map.insert(1, 1001);
    map.insert(-2, -2);
    map.insert(252, 252);
    map.insert(33, 33);
    map.insert(3342, 3342);
    map.insert(-9, -9);

    assert(map.find(999) == nullptr);

    PrintMap(map);

    auto iter = map.find(-2);
    map.erase(iter);

    map.erase(3342);

    map.insert(44, 44);
    map.insert(-65, -65);

    PrintMap(map);
}

int main() 
{
//    TestRBTree();
    TestMap();

    return 0;
}