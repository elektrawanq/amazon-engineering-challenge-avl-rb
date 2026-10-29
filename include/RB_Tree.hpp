#ifndef RB_TREE_HPP
#define RB_TREE_HPP

#include <iostream>
#include <algorithm>

using value_type = int;
enum Color
{
    RED,
    BLACK
};

struct Node_RB
{
    value_type data{0};
    Color color{BLACK};
    Node_RB *left_child{nullptr};
    Node_RB *right_child{nullptr};
    Node_RB *parent;

    Node_RB(Node_RB *p) : parent(p) {}
};

class RB_Tree
{
private:
    Node_RB *root;
    Node_RB *TNULL;

    void destroy_tree(Node_RB *node)
    {
        if (node != TNULL && node != nullptr)
        {
            destroy_tree(node->left_child);
            destroy_tree(node->right_child);
            delete node;
        }
    }

public:
    int rotation_count{0};

private:
    // ROTAÇÕES O(1)
    void Left_Rotation(Node_RB *unbal)
    {
        Node_RB *new_root = unbal->right_child;
        unbal->right_child = new_root->left_child;
        if (new_root->left_child != TNULL)
        {
            new_root->left_child->parent = unbal;
        }
        new_root->parent = unbal->parent;
        if (unbal->parent == nullptr)
        {
            this->root = new_root;
        }
        else if (unbal == unbal->parent->left_child)
        {
            unbal->parent->left_child = new_root;
        }
        else
        {
            unbal->parent->right_child = new_root;
        }
        new_root->left_child = unbal;
        unbal->parent = new_root;
        rotation_count++;
    }

    void Right_Rotation(Node_RB *unbal)
    {
        Node_RB *new_root = unbal->left_child;
        unbal->left_child = new_root->right_child;
        if (new_root->right_child != TNULL)
        {
            new_root->right_child->parent = unbal;
        }
        new_root->parent = unbal->parent;
        if (unbal->parent == nullptr)
        {
            this->root = new_root;
        }
        else if (unbal == unbal->parent->right_child)
        {
            unbal->parent->right_child = new_root;
        }
        else
        {
            unbal->parent->left_child = new_root;
        }
        new_root->right_child = unbal;
        unbal->parent = new_root;
        rotation_count++;
    }

    // INSERÇÃO E BALANCEAMENTO
    void insert_fixup(Node_RB *node)
    {
        Node_RB *uncle;
        while (node->parent != nullptr and node->parent->color == RED)
        {
            if (node->parent == node->parent->parent->right_child)
            {
                uncle = node->parent->parent->left_child;
                if (uncle->color == RED)
                {
                    uncle->color = BLACK;
                    node->parent->color = BLACK;
                    node->parent->parent->color = RED;
                    node = node->parent->parent;
                }
                else
                {
                    if (node == node->parent->left_child)
                    {
                        node = node->parent;
                        Right_Rotation(node);
                    }
                    node->parent->color = BLACK;
                    node->parent->parent->color = RED;
                    Left_Rotation(node->parent->parent);
                }
            }
            else
            {
                uncle = node->parent->parent->right_child;
                if (uncle->color == RED)
                {
                    uncle->color = BLACK;
                    node->parent->color = BLACK;
                    node->parent->parent->color = RED;
                    node = node->parent->parent;
                }
                else
                {
                    if (node == node->parent->right_child)
                    {
                        node = node->parent;
                        Left_Rotation(node);
                    }
                    node->parent->color = BLACK;
                    node->parent->parent->color = RED;
                    Right_Rotation(node->parent->parent);
                }
            }
            if (node == root)
            {
                break;
            }
        }
        root->color = BLACK;
    }

    // REMOÇÃO E BALANCEAMENTO
    void transplant(Node_RB *u, Node_RB *v)
    {
        if (u->parent == nullptr)
        {
            root = v;
        }
        else if (u == u->parent->left_child)
        {
            u->parent->left_child = v;
        }
        else
        {
            u->parent->right_child = v;
        }
        v->parent = u->parent;
    }

    void delete_fixup(Node_RB *node)
    {
        Node_RB *sibling;
        while (node != root and node->color == BLACK)
        {
            if (node == node->parent->left_child)
            {
                sibling = node->parent->right_child;
                if (sibling->color == RED)
                {
                    sibling->color = BLACK;
                    node->parent->color = RED;
                    Left_Rotation(node->parent);
                    sibling = node->parent->right_child;
                }
                if (sibling->left_child->color == BLACK and sibling->right_child->color == BLACK)
                {
                    sibling->color = RED;
                    node = node->parent;
                }
                else
                {
                    if (sibling->right_child->color == BLACK)
                    {
                        sibling->left_child->color = BLACK;
                        sibling->color = RED;
                        Right_Rotation(sibling);
                        sibling = node->parent->right_child;
                    }
                    sibling->color = node->parent->color;
                    node->parent->color = BLACK;
                    sibling->right_child->color = BLACK;
                    Left_Rotation(node->parent);
                    node = root;
                }
            }
            else
            {
                sibling = node->parent->left_child;
                if (sibling->color == RED)
                {
                    sibling->color = BLACK;
                    node->parent->color = RED;
                    Right_Rotation(node->parent);
                    sibling = node->parent->left_child;
                }
                if (sibling->right_child->color == BLACK and sibling->left_child->color == BLACK)
                {
                    sibling->color = RED;
                    node = node->parent;
                }
                else
                {
                    if (sibling->left_child->color == BLACK)
                    {
                        sibling->right_child->color = BLACK;
                        sibling->color = RED;
                        Left_Rotation(sibling);
                        sibling = node->parent->left_child;
                    }
                    sibling->color = node->parent->color;
                    node->parent->color = BLACK;
                    sibling->left_child->color = BLACK;
                    Right_Rotation(node->parent);
                    node = root;
                }
            }
        }
        node->color = BLACK;
    }

    Node_RB *get_min_value_node(Node_RB *node)
    {
        while (node->left_child != TNULL)
        {
            node = node->left_child;
        }
        return node;
    }

    void delete_node_helper(Node_RB *node, value_type key)
    {
        Node_RB *target = TNULL;
        Node_RB *x, *y;
        while (node != TNULL)
        {
            if (node->data == key)
            {
                target = node;
            }
            if (node->data <= key)
            {
                node = node->right_child;
            }
            else
            {
                node = node->left_child;
            }
        }

        if (target == TNULL)
            return;

        y = target;
        Color y_original_color = y->color;
        if (target->left_child == TNULL)
        {
            x = target->right_child;
            transplant(target, target->right_child);
        }
        else if (target->right_child == TNULL)
        {
            x = target->left_child;
            transplant(target, target->left_child);
        }
        else
        {
            y = get_min_value_node(target->right_child);
            y_original_color = y->color;
            x = y->right_child;
            if (y->parent == target)
            {
                x->parent = y;
            }
            else
            {
                transplant(y, y->right_child);
                y->right_child = target->right_child;
                y->right_child->parent = y;
            }
            transplant(target, y);
            y->left_child = target->left_child;
            y->left_child->parent = y;
            y->color = target->color;
        }
        delete target;
        if (y_original_color == BLACK)
        {
            delete_fixup(x);
        }
    }

    int get_height_recursive(Node_RB *node)
    {
        if (node == TNULL)
            return 0;
        return std::max(get_height_recursive(node->left_child), get_height_recursive(node->right_child)) + 1;
    }

public:
    RB_Tree()
    {
        TNULL = new Node_RB(nullptr);
        TNULL->color = BLACK;
        TNULL->left_child = TNULL;
        TNULL->right_child = TNULL;
        root = TNULL;
    }

    ~RB_Tree()
    {
        destroy_tree(root);
        delete TNULL;
    }

    void insert(value_type data)
    {
        Node_RB *node = new Node_RB(nullptr);
        node->data = data;
        node->left_child = TNULL;
        node->right_child = TNULL;
        node->color = RED;

        Node_RB *temp = nullptr;
        Node_RB *current = this->root;

        while (current != TNULL)
        {
            temp = current;
            if (node->data < current->data)
            {
                current = current->left_child;
            }
            else if (node->data > current->data)
            {
                current = current->right_child;
            }
            else
            {
                delete node;
                return;
            }
        }

        node->parent = temp;
        if (temp == nullptr)
        {
            root = node;
        }
        else if (node->data < temp->data)
        {
            temp->left_child = node;
        }
        else
        {
            temp->right_child = node;
        }

        if (node->parent == nullptr)
        {
            node->color = BLACK;
            return;
        }

        if (node->parent->parent == nullptr)
        {
            return;
        }

        insert_fixup(node);
    }

    void remove(value_type data)
    {
        delete_node_helper(this->root, data);
    }

    int get_height()
    {
        return get_height_recursive(this->root);
    }

    int get_rotations()
    {
        return rotation_count;
    }

    void reset_metrics()
    {
        rotation_count = 0;
    }
};

#endif