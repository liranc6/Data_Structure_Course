//1
// Created by liran on 07/05/2021.
//

#ifndef DATA_STRUCTURE_RANKAVLTREE_H
#define DATA_STRUCTURE_RANKAVLTREE_H
#include "RankAVLTreeNode.h"

class TreeNodeNotFound : public std::exception
{
};

template <class T, typename Cmp>
class RankAVLTree
{
public:
    RankAVLTreeNode<T, Cmp> *root_;
    Cmp cmp_;
    RankAVLTreeNode<T, Cmp> *smallest_leaf_;

    explicit RankAVLTree() : root_(nullptr), smallest_leaf_(nullptr){};

    ///what for?
    //RankAVLTree(RankAVLTree& tree1, RankAVLTree& tree2, Cmp cmp = Cmp()){};

    ~RankAVLTree()
    {
    }
    RankAVLTreeNode<T, Cmp> *insert(const T &data)
    {
        if (root_ == nullptr)
        {
            root_ = new RankAVLTreeNode<T, Cmp>(data, cmp_, nullptr, nullptr, nullptr);

            update_smallest();
            return root_;
        }
        RankAVLTreeNode<T, Cmp> *new_vertex = root_->insert(data);

        root_ = root_->get_root(); // updating new root if needed
        update_smallest();
        return new_vertex;
    }

    void update_smallest()
    {
        if (root_ == nullptr)
        {
            smallest_leaf_ = nullptr;
            return;
        }
        smallest_leaf_ = root_;
        while (smallest_leaf_->left != nullptr)
        {
            smallest_leaf_ = smallest_leaf_->left;
        }
    }
    template <typename ValueType>
    void remove_by_value(const ValueType &value)
    {
        if (root_)
        {

            root_ = root_->remove_node_by_value(value); ///compiling?
        }
        update_smallest();
    }

    /**
     * removes node and update tree
     * @param node - pointer to node to remove
     */
    void remove_by_data(T &data)
    {

        RankAVLTreeNode<T, Cmp> *to_remove = root_->find_in_tree_by_data(data);

        if (to_remove)
        {

            root_ = to_remove->remove_specific_node();

            root_ = root_->get_root();
            if (root_ == nullptr)
            {
                smallest_leaf_ = nullptr;
                return ;
            }
            update_smallest();
        }
    }

    /**
     *  find in tree by data
     * @tparam ValueType
     * @param value
     * @return pointer to node or nullptr if not found
     */
    template <typename ValueType>
    RankAVLTreeNode<T, Cmp> *find_by_data(const ValueType &value)
    {
        if (root_)
        {
            return root_->find_in_tree_by_data(value);
        }

        return nullptr;
    }

    /**
     *search_in_tree by value in the data itself
     * @tparam ValueType
     * @param value
     * @return  pointer to node or nullptr if not found
     */

    template <typename ValueType>
    RankAVLTreeNode<T, Cmp> *find_by_value(const ValueType &value)
    {
        if (root_)
        {

            return root_->find_in_tree_by_value(value);
        }
        return nullptr;
    }

    template <typename Func>
    void inorder(Func func)
    {
        if (root_)
        {
            root_->inorder(func);
        }
    }

    T &getRootData()
    {
        return root_->data;
    }
    void delete_tree()
    {

        root_->delete_tree();
        root_ = nullptr;
    }
};

#endif //DATA_STRUCTURE_RANKAVLTREE_H
