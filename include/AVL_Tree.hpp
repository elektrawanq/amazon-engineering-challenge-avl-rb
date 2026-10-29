// ÁRVORE AVL
#ifndef AVL_TREE
#define AVL_TREE

#include <algorithm>
#include <iostream>
using value_type = int;

struct Node
{
    value_type data;
    value_type height{0};
    Node *left_child{nullptr};
    Node *right_child{nullptr};

    Node(value_type v) : data(v) {}
};

class AVL_Tree
{
private:
    Node *root{nullptr};

    void destroy_tree(Node *node)
    {
        if (node != nullptr)
        {
            destroy_tree(node->left_child);
            destroy_tree(node->right_child);
            delete node;
        }
    }

    value_type get_height_internal(Node *node)
    {
        if (node == nullptr)
            return -1;
        return node->height;
    }

    void update_height(Node *node)
    {
        if (node != nullptr)
        {
            node->height = std::max(get_height_internal(node->left_child), get_height_internal(node->right_child)) + 1;
        }
    }

    value_type balance_factor(Node *node)
    {
        if (node == nullptr)
            return 0;
        return get_height_internal(node->left_child) - get_height_internal(node->right_child);
    }

    // Rotação LL ou simples à direita
    Node *Right_Rotation(Node *unbal)
    {
        Node *new_root = unbal->left_child;
        Node *previous_right = new_root->right_child;

        new_root->right_child = unbal;
        unbal->left_child = previous_right;

        update_height(unbal);
        update_height(new_root);

        rotation_count++; // Métrica para os testes
        return new_root;
    }

    // Rotação RR ou simples à esquerda
    Node *Left_Rotation(Node *unbal)
    {
        Node *new_root = unbal->right_child;
        Node *previous_left = new_root->left_child;

        new_root->left_child = unbal;
        unbal->right_child = previous_left;

        update_height(unbal);
        update_height(new_root);

        rotation_count++; // Métrica
        return new_root;
    }

    // Rotação LR ou dupla à direita
    Node *Double_Right_LR(Node *unbal)
    {
        unbal->left_child = Left_Rotation(unbal->left_child);
        Node *new_root = Right_Rotation(unbal);
        return new_root;
    }

    // Rotação RL ou dupla à esquerda
    Node *Double_Left_RL(Node *unbal)
    {
        unbal->right_child = Right_Rotation(unbal->right_child);
        Node *new_root = Left_Rotation(unbal);
        return new_root;
    }

    Node *insert_recursive(Node *node, value_type data)
    {
        if (node == nullptr)
        {
            return new Node(data);
        }

        if (data < node->data)
        {
            node->left_child = insert_recursive(node->left_child, data);
        }
        else if (data > node->data)
        {
            node->right_child = insert_recursive(node->right_child, data);
        }
        else
        {
            return node;
        }

        update_height(node);
        value_type bf = balance_factor(node);

        if (bf > 1 and data < node->left_child->data)
        {
            return Right_Rotation(node);
        }
        if (bf < -1 and data > node->right_child->data)
        {
            return Left_Rotation(node);
        }
        if (bf > 1 and data > node->left_child->data)
        {
            return Double_Right_LR(node);
        }
        if (bf < -1 and data < node->right_child->data)
        {
            return Double_Left_RL(node);
        }

        return node;
    }

    Node *get_min_value_node(Node *node)
    {
        Node *current = node;
        while (current != nullptr and current->left_child != nullptr)
        {
            current = current->left_child;
        }
        return current;
    }

    Node *remove_recursive(Node *node, value_type data)
    {
        if (node == nullptr)
            return node;

        if (data < node->data)
        {
            node->left_child = remove_recursive(node->left_child, data);
        }
        else if (data > node->data)
        {
            node->right_child = remove_recursive(node->right_child, data);
        }
        else
        {
            if (node->left_child == nullptr or node->right_child == nullptr)
            {
                Node *temp = node->left_child ? node->left_child : node->right_child;
                if (temp == nullptr)
                {
                    temp = node;
                    node = nullptr;
                }
                else
                {
                    *node = *temp;
                }
                delete temp;
            }
            else
            {
                Node *temp = get_min_value_node(node->right_child);
                node->data = temp->data;
                node->right_child = remove_recursive(node->right_child, temp->data);
            }
        }

        if (node == nullptr)
            return node;

        update_height(node);
        value_type bf = balance_factor(node);

        if (bf > 1 and balance_factor(node->left_child) >= 0)
        {
            return Right_Rotation(node);
        }
        if (bf > 1 and balance_factor(node->left_child) < 0)
        {
            return Double_Right_LR(node);
        }
        if (bf < -1 and balance_factor(node->right_child) <= 0)
        {
            return Left_Rotation(node);
        }
        if (bf < -1 and balance_factor(node->right_child) > 0)
        {
            return Double_Left_RL(node);
        }

        return node;
    }

public:
    value_type rotation_count{0};

    AVL_Tree() = default;

    ~AVL_Tree()
    {
        destroy_tree(root);
    }

    void insert(value_type data)
    {
        root = insert_recursive(root, data);
    }

    void remove(value_type data)
    {
        root = remove_recursive(root, data);
    }

    value_type get_height()
    {
        if (root == nullptr)
            return 0;
        return root->height;
    }

    value_type get_rotations()
    {
        return rotation_count;
    }

    void reset_metrics()
    {
        rotation_count = 0;
    }
};
#endif