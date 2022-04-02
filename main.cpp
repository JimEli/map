#include <iostream>
#include "rbtree.h"
#include "map.h"

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
    TestMap();

    return 0;
}
