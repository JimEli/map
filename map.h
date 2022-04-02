#ifndef _MAP_H_
#define _MAP_H_

#include <new>
#include <utility>
#include "rbtree.h"

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

template<typename NodeType>
class MapIterator
{
    using Node_t = NodeType;
    using Value_t = typename Node_t::Value_t;

public:
    MapIterator() : _node(nullptr) { }
    MapIterator(const MapIterator& other) : _node(other._node) { }
    MapIterator(Node_t* node) : _node(node) { }
    ~MapIterator() { }

    MapIterator& operator=(const MapIterator& other)
    {
        if (&other != this)
            _node = other._node;

        return *this;
    }

    Node_t* Node() { return _node; }

    Value_t& operator*() { return _node->_value; }
    Value_t* operator->() { return &_node->_value; }

    MapIterator& operator++()
    {
        _node = _node->forward();
        return *this;
    }
    MapIterator& operator++(int)
    {
        auto node = _node->forward();
        return MapIterator(node);
    }

    bool operator==(const MapIterator& other) const { return _node == other._node; }
    bool operator!=(const MapIterator& other) const { return _node != other._node; }

private:
    Node_t* _node;
};

template<typename NodeType>
class MapConstIterator
{
    using Node_t = NodeType;
    using Value_t = typename Node_t::Value_t;

public:
    MapConstIterator() : _node(nullptr) { }
    MapConstIterator(const MapConstIterator& other) : _node(other._node) { }
    MapConstIterator(const Node_t* node) : _node(node) { }
    ~MapConstIterator() { }

    MapConstIterator& operator=(const MapConstIterator& other)
    {
        if (&other != this)
            _node = other._node;

        return *this;
    }

    const Node_t* Node() const { return _node; }

    const Value_t& operator*() const { return _node->_value; }
    const Value_t* operator->() const { return &_node->_value; }

    MapConstIterator& operator++()
    {
        _node = _node->Forward();
        return *this;
    }

    MapConstIterator& operator++(int)
    {
        const auto node = _node->Forward();
        return MapConstIterator(node);
    }

    bool operator==(const MapConstIterator& other) const { return _node == other._node; }
    bool operator!=(const MapConstIterator& other) const { return _node != other._node; }

private:
    const Node_t* _node;
};

template
<typename PairType, typename CompareType>
struct PairCompare
{
    CompareType _compare;
    int operator ()(const PairType& left, const PairType& right) const { return _compare(left.first, right.first); }
};

template
<typename KeyType, typename ValueType, typename CompareType=GenericCompare<KeyType>>
class Map
{
    using Key_t = KeyType;
    using Value_t = ValueType;
    using Compare_t = CompareType;
    using Data_t = std::pair<Key_t, Value_t>;
    using DataCompare_t = PairCompare<Data_t, Compare_t>;
    using Node_t = RBTreeNode<Data_t>;
    using Tree_t = RBTree<Node_t, DataCompare_t>;

public:
    using Iterator_t = MapIterator<Node_t> ;
    using ConstIterator_t = MapConstIterator<Node_t>;

    Map() { }
    Map(const Map&) = delete;
    Map& operator =(const Map&) = delete;
    ~Map() 
    {
        if (!empty())
            clear();
    }

    void clear() 
    {
        while (!empty()) 
        {
            Iterator_t iter = begin();
            erase(iter);
        }
    }

    bool empty() const { return _tree.empty(); }
    size_t size() const { return _tree.size(); }

    ConstIterator_t begin() const {return ConstIterator_t((const auto)_tree.First()); }
    Iterator_t begin() { return Iterator_t(_tree.first()); }
    ConstIterator_t end() const { return ConstIterator_t(); }
    Iterator_t end() { return Iterator_t(); }

    Iterator_t insert(const Key_t& key, const Value_t& value) 
    {
        auto node = createNode();
        assert(!isNull(node));
        node->_value.first = key;
        node->_value.second = value;
        const auto ret = _tree.insertUnique(node);
        if (isNull(ret)) 
            destoryNode(node);

        return Iterator_t(ret);
    }

    void erase(Iterator_t iter) 
    {
        assert(iter != end());
        _tree.remove(iter.Node());
        destoryNode(iter.Node());
    }

    void erase(const Key_t& key) 
    {
        auto iter = find(key);
        if (iter != end()) 
            erase(iter);
    }

    Iterator_t find(const Key_t& key) 
    {
        Data_t data;
        data.first = key;
        Node_t* node = _tree.search(data);
    
        return Iterator_t(node);
    }

private:
    Node_t* createNode() 
    {
        auto node = new Node_t();

        return node;
    }

    void destoryNode(Node_t* node) 
    {
        node->~Node_t();
        delete node;
    }

    Tree_t _tree;
};

#endif
