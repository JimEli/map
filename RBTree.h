#ifndef _RBTREE_H_
#define _RBTREE_H_

#include <cassert>

template<typename T>
inline bool isNull(const T* const ptr) { return nullptr == ptr; }
template<typename T>
inline bool isNull(const T& val) { return nullptr == val; }

template<typename ValueType>
struct RBTreeNode 
{
    using Value_t = ValueType;

    enum Color { RED = 0, BLACK, };

    RBTreeNode* _parent = nullptr;
    RBTreeNode* _right = nullptr;
    RBTreeNode* _left = nullptr;
    Color _color = RED;
    Value_t _value;

    bool isRed() const { return _color == RED; }
    void setRed() { _color = RED; }
    bool isBlack() const { return _color == BLACK; }
    void setBlack() { _color = BLACK; }

    RBTreeNode* backward() 
    {
        auto node = this;
        if (node->_parent == node) 
            return nullptr;

        if (!isNull(node->_left)) 
        {
            node = node->_left;
            while (!isNull(node->_right)) 
                node = node->_right;

            return node;
        }

        auto parent = node->_parent;
        while (!isNull(parent) && node == parent->_left) 
        {
            node = parent;
            parent = node->_parent;
        }

        return parent;
    }

    RBTreeNode* forward() 
    {
        auto node = this;
        if (node->_parent == node) 
            return nullptr;

        if (!isNull(node->_right)) 
        {
            node = node->_right;
            while (!isNull(node->_left)) 
                node = node->_left;

            return node;
        }

        auto parent = node->_parent;
        while (!isNull(parent) && node == parent->_right) 
        {
            node = parent;
            parent = node->_parent;
        }

        return parent;
    }
};

template
<typename NodeType, typename CompareType>
class RBTree 
{
    using Node_t = NodeType;
    using Compare_t = CompareType;
    using Value_t = typename Node_t::Value_t;

public:
    RBTree() : _root(nullptr), _size(0) { }
    RBTree(const RBTree&) = delete;
    RBTree& operator =(const RBTree&) = delete;
    ~RBTree() { }

public:
    void clear() 
    {
        _root = nullptr;
        _size = 0;
    }

    bool empty() const { return nullptr == _root; }
    size_t size() const { return _size; }

    Node_t* first() 
    {
        auto next = _root;
        if (isNull(next)) 
            return nullptr;

        while (!isNull(next->_left)) 
            next = next->_left;

        return next;
    }

    Node_t* last() 
    {
        auto next = _root;
        if (isNull(next)) 
            return nullptr;

        while (!isNull(next->_right)) 
            next = next->_right;

        return next;
    }

    Node_t* insertUnique(Node_t* node) 
    {
        assert(!isNull(node));
        Node_t* parent = nullptr;
        auto temp = &_root;

        while (!isNull(*temp)) 
        {
            parent = *temp;

            const auto ret = _compare(node->_value, parent->_value);
            if (ret < 0) 
                temp = &(*temp)->_left;
            else if (ret > 0) 
                temp = &(*temp)->_right;
            else  
                return nullptr;
        }

        link(node, parent, *temp);
        insertColor(node);
        ++_size;
        return node;
    }

    Node_t* insertRepeat(Node_t* node) 
    {
        assert(!isNull(node));
        Node_t* parent = nullptr;
        auto temp = &_root;

        while (!isNull(*temp)) 
        {
            parent = *temp;

            const auto ret = _compare(node->_value, parent->_value);
            if (ret < 0) 
                temp = &(*temp)->_left;
            else 
                temp = &(*temp)->_right;
        }

        link(node, parent, *temp);
        insertColor(node);
        ++_size;
        return node;
    }

    void remove(Node_t* node) 
    {
        assert(!isNull(node));

        Node_t* child = nullptr;
        Node_t* parent = nullptr;
        auto color = Node_t::RED;

        do {
            if (isNull(node->_left)) 
                child = node->_right;
            else if (isNull(node->_right)) 
                child = node->_left;
            else 
            {
                auto old = node;
                node = node->_right;

                auto left = node->_left;
                while (!isNull(left)) 
                {
                    node = left;
                    left = node->_left;
                }

                auto oldParent = old->_parent;
                if (!isNull(oldParent)) 
                {
                    if (oldParent->_left == old) 
                        oldParent->_left = node;
                    else 
                        oldParent->_right = node;
                }
                else 
                {
                    _root = node;
                }

                child = node->_right;
                parent = node->_parent;
                color = node->_color;

                if (parent == old) 
                    parent = node;
                else 
                {
                    if (!isNull(child)) 
                        child->_parent = parent;

                    parent->_left = child;
                    node->_right = old->_right;
                    old->_right->_parent = node;
                }

                node->_color = old->_color;
                node->_parent = old->_parent;
                node->_left = old->_left;
                old->_left->_parent = node;

                break;
            }

            parent = node->_parent;
            color = node->_color;

            if (!isNull(child)) 
                child->_parent = parent;

            if (!isNull(parent)) 
            {
                if (parent->_left == node) 
                    parent->_left = child;
                else 
                    parent->_right = child;
            }
            else 
                _root = child;
        } while (false);

        if (color == Node_t::BLACK) 
            removeColor(child, parent);

        --_size;
    }

    Node_t* search(const Value_t& value) 
    {
        auto node = _root;
        while (!isNull(node)) 
        {
            const auto ret = _compare(value, node->_value);
            if (ret < 0) 
                node = node->_left;
            else if (ret > 0) 
                node = node->_right;
            else 
                return node;
        }

        return nullptr;
    }

    void replace(Node_t* victim, Node_t* replacement) 
    {
        assert(!isNull(victim));
        assert(!isNull(replacement));

        auto parent = victim->_parent;
        if (!isNull(parent)) 
        {
            if (victim == parent->_left) 
                parent->_left = replacement;
            else 
                parent->_right = replacement;
        }
        else 
            _root = replacement;

        if (!isNull(victim->_left)) 
            victim->_left->_parent = replacement;

        if (!isNull(victim->_right)) 
            victim->_right->_parent = replacement;

        *replacement = *victim;
    }

    Node_t* backward(Node_t* node) 
    {
        assert(!isNull(node));
        return node->backward();
    }

    Node_t* forward(Node_t* node) 
    {
        assert(!isNull(node));
        return node->forward();
    }

private:
    void link(Node_t* node, Node_t* parent, Node_t*& link) 
    {
        node->_parent = parent;
        node->_left = node->_right = nullptr;
        link = node;
    }

    void insertColor(Node_t* node) 
    {
        auto parent = node->_parent;
        while (!isNull(parent) && parent->isRed()) 
        {
            auto gparent = parent->_parent;
            if (parent == gparent->_left) 
            {
                auto uncle = gparent->_right;
                if (uncle && uncle->isRed()) 
                {
                    uncle->setBlack();
                    parent->setBlack();
                    gparent->setRed();
                    node = gparent;
                    continue;
                }

                if (parent->_right == node) 
                {
                    rotateLeft(parent);
                    auto temp = parent;
                    parent = node;
                    node = temp;
                }

                parent->setBlack();
                gparent->setRed();
                rotateRight(gparent);
            }
            else 
            {
                auto uncle = gparent->_left;
                if (uncle && uncle->isRed()) 
                {
                    uncle->setBlack();
                    parent->setBlack();
                    gparent->setRed();
                    node = gparent;
                    continue;
                }

                if (parent->_left == node) 
                {
                    rotateRight(parent);
                    auto temp = parent;
                    parent = node;
                    node = temp;
                }

                parent->setBlack();
                gparent->setRed();
                rotateLeft(gparent);
            }

            parent = node->_parent;
        }

        _root->setBlack();
    }

    void removeColor(Node_t* node, Node_t* parent) 
    {
        while ((isNull(node) || node->isBlack()) && node != _root) 
        {
            if (parent->_left == node) 
            {
                auto other = parent->_right;
                if (other->isRed()) 
                {
                    other->setBlack();
                    parent->setRed();
                    rotateLeft(parent);
                    other = parent->_right;
                }

                if ((isNull(other->_left) || other->_left->isBlack()) && (isNull(other->_right) || other->_right->isBlack())) 
                {
                    other->setRed();
                    node = parent;
                    parent = node->_parent;
                }
                else 
                {
                    if (isNull(other->_right) || other->_right->isBlack()) 
                    {
                        other->_left->setBlack();
                        other->setRed();
                        rotateRight(other);
                        other = parent->_right;
                    }

                    other->_color = parent->_color;
                    parent->setBlack();
                    other->_right->setBlack();
                    rotateLeft(parent);
                    node = _root;
                    break;
                }
            }
            else 
            {
                auto other = parent->_left;
                if (other->isRed()) 
                {
                    other->setBlack();
                    parent->setRed();
                    rotateRight(parent);
                    other = parent->_left;
                }

                if ((isNull(other->_left) || other->_left->isBlack()) && (isNull(other->_right) || other->_right->isBlack())) 
                {
                    other->setRed();
                    node = parent;
                    parent = node->_parent;
                }
                else 
                {
                    if (isNull(other->_left) || other->_left->isBlack()) 
                    {
                        other->_right->setBlack();
                        other->setRed();
                        rotateLeft(other);
                        other = parent->_left;
                    }

                    other->_color = parent->_color;
                    parent->setBlack();
                    other->_left->setBlack();
                    rotateRight(parent);
                    node = _root;
                    break;
                }
            }
        }

        if (!isNull(node)) 
            node->setBlack();
    }

    void rotateLeft(Node_t* node) 
    {
        auto right = node->_right;
        auto parent = node->_parent;
        node->_right = right->_left;

        if (!isNull(right->_left)) 
            right->_left->_parent = node;

        right->_left = node;
        right->_parent = parent;

        if (!isNull(parent)) 
        {
            if (node == parent->_left) 
                parent->_left = right;
            else 
                parent->_right = right;
        }
        else 
            _root = right;

        node->_parent = right;
    }

    void rotateRight(Node_t* node) 
    {
        auto left = node->_left;
        auto parent = node->_parent;
        node->_left = left->_right;

        if (!isNull(left->_right)) 
            left->_right->_parent = node;

        left->_right = node;
        left->_parent = parent;

        if (!isNull(parent)) 
        {
            if (node == parent->_right) 
                parent->_right = left;
            else 
                parent->_left = left;
        }
        else 
            _root = left;

        node->_parent = left;
    }

private:
    Node_t* _root;
    size_t _size;
    Compare_t _compare;
};

#endif
