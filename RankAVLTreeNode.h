
#ifndef DATA_STRUCTURE_RANKAVLTREENODE_H
#define DATA_STRUCTURE_RANKAVLTREENODE_H

#include <iostream>

bool nod_debug = false;

template <class T, class Cmp>
class RankAVLTreeNode
{
public:
    T data;
    Cmp cmp; ///cmp class
    RankAVLTreeNode<T, Cmp> *parent;
    RankAVLTreeNode<T, Cmp> *left;
    RankAVLTreeNode<T, Cmp> *right;
    int height;
    int rank;

  
    RankAVLTreeNode(T &data, RankAVLTreeNode *parent = nullptr,
                    RankAVLTreeNode *left = nullptr, RankAVLTreeNode *right = nullptr) : data(data), parent(parent), left(left), right(right), height(0), rank(1)
    {
    }

    RankAVLTreeNode(const T &data, Cmp cmp_, RankAVLTreeNode *parent = nullptr,
                    RankAVLTreeNode *left = nullptr, RankAVLTreeNode *right = nullptr) : data(data), cmp(cmp), parent(parent), left(left), right(right), height(0), rank(1)
    {
    }

    virtual ~RankAVLTreeNode(){

    };

    template <typename ValueType>
    //returning new root
    RankAVLTreeNode *remove_node_by_value(const ValueType &value)
    {
        RankAVLTreeNode *to_remove = find_in_tree_by_value(value);

        //RankAVLTreeNode* to_remove_parent = to_remove->parent;
        if (to_remove)
        {
            RankAVLTreeNode *new_root = to_remove->remove_specific_node();
            while (new_root != nullptr && new_root->rotate())
            {
                new_root = new_root->parent;
            }
            return new_root->get_root();
        }

        return nullptr;
    }

    /**
     *
     * @return new root
     */

    RankAVLTreeNode *remove_specific_node() //returning new root
    {

        RankAVLTreeNode *temp_parent = parent;
        //leaf
        if (!left && !right)
        {
            if (temp_parent != nullptr)
            {
                move_to_parent(this, nullptr);
            }

            delete this;

            while (temp_parent)
            {

               
               temp_parent->rotate() ;
               temp_parent->fix_height_and_rank();

             

                if (temp_parent->parent == nullptr)
                {
                    break;
                }

                temp_parent = temp_parent->parent;
            }

            return temp_parent;
        }

        //has 1 child, left child
        if (left && !right)
        {
            if (temp_parent == nullptr)
            {
                RankAVLTreeNode *new_root = left;
                left->parent = nullptr;
                delete this;
                return new_root;
            }
            if (temp_parent != nullptr)
            {
                move_to_parent(this, left);
            }

            delete this;
            while (temp_parent)
            {

                temp_parent->rotate();
                temp_parent->fix_height_and_rank();

                if (temp_parent->parent == nullptr)
                {
                    break;
                }
                temp_parent = temp_parent->parent;
            }
            return temp_parent;
        }

        //has 2 children or right children


        RankAVLTreeNode *next_in_order = FindFirstInOrder(right);

        swap_vertex(this, next_in_order);
 
        if (this->right != nullptr)
        {

            RankAVLTreeNode *temp = this;
            RankAVLTreeNode *temp2 = this->right;
            swap_vertex(temp, temp2);
           
        }

        RankAVLTreeNode *to_remove_parent = parent;
        bool is_left_son = true;
        if (to_remove_parent->right == this)
        {
            is_left_son = false;
        }

        delete this;

        if (is_left_son)
        {
            to_remove_parent->left = nullptr;
        }
        else
        {

            to_remove_parent->right = nullptr;
        }
to_remove_parent->fix_height_and_rank();
        while (1)
        {
            if (to_remove_parent)
            {
               
         
                to_remove_parent->rotate();
             
                to_remove_parent->fix_height_and_rank();
            }
            if (to_remove_parent->parent == nullptr)
            {
                break;
            }
            to_remove_parent = to_remove_parent->parent;
        }

        return to_remove_parent;
    }
    friend void swap_vertex(RankAVLTreeNode *a, RankAVLTreeNode *b)
    {
        RankAVLTreeNode *t_parent = b->parent;
        RankAVLTreeNode *t_right = b->right;
        RankAVLTreeNode *t_left = b->left;

        if (b->parent && b->parent->left == b)
        {
            b->parent->left = a;
        }
        if (b->parent && b->parent->right == b)
        {
            b->parent->right = a;
        }
        if (b->left)
        {
            b->left->parent = a;
        }
        if (b->right)
        {
            b->right->parent = a;
        }

        if (a->parent == b)
        {
            b->parent = a;
        }
        else
        {
            b->parent = a->parent;
        }
        if (a->left == b)
        {
            b->left = a;
        }
        else
        {
            b->left = a->left;
        }
        if (a->right == b)
        {
            b->right = a;
        }
        else
        {
            b->right = a->right;
        }

        if (a->parent && b->parent->left == a)
        {
            a->parent->left = b;
        }
        if (a->parent && b->parent->right == a)
        {
            a->parent->right = b;
        }

        if (a->right)
        {
            a->right->parent = b;
        }
        if (a->left)
        {
            a->left->parent = b;
        }

        if (t_parent == a)
        {
            a->parent = b;
        }
        else
        {
            a->parent = t_parent;
        }
        if (t_left == a)
        {
            a->left = b;
        }
        else
        {
            a->left = t_left;
        }

        if (t_right == a)
        {
            a->right = b;
        }
        else
        {
            a->right = t_right;
        }
    }

    /**
     * find_closest_leaf_in_tree_by_value in data
     * search in tree by value in the data itself
     * @tparam ValueType
     * @param value
     * @return pointer to closest_in_leaf_tree or leaf itself with the value
     */
    template <typename ValueType>
    RankAVLTreeNode *find_closest_leaf_in_tree_by_value(const ValueType &value)
    {
        if (cmp(data, value) < 0)
        {
            if (right)
            {
                return right->find_closest_leaf_in_tree_by_value(value);
            }
        }
        else if (cmp(data, value) > 0)
        {
            if (left)
            {
                return left->find_closest_leaf_in_tree_by_value(value);
            }
        }

        return this;
    }

    /**
     * find in tree by value in the data itself
     * @tparam ValueType
     * @param value
     * @return  pointer to node if found or nullptr if not found
     */
    template <typename ValueType>
    RankAVLTreeNode *find_in_tree_by_value(const ValueType &value)
    {
        RankAVLTreeNode *to_return = find_closest_leaf_in_tree_by_value(value);
        if (cmp(to_return->data, value) == 0)
        {
            return to_return;
        }

        return nullptr;
    }

    /**
     * find_in_tree_by_data
     * find in tree by data in the Tree node
     * @param data
     * @return pointer to node or nullptr if not found
     */
    RankAVLTreeNode *find_in_tree_by_data(const T &data_to_cmp)
    {
        if (this == nullptr)
        {
            return nullptr;
        }
        if (data == data_to_cmp)
        {
            return this;
        }
        if (data < data_to_cmp)
        {
            if (right)
            {
                return right->find_in_tree_by_data(data_to_cmp);
            }
        }
        else if (data > data_to_cmp)
        {
            if (left)
            {
                return left->find_in_tree_by_data(data_to_cmp);
            }
        }

        return nullptr;
    }
    RankAVLTreeNode *find_parent(const T &data_to_cmp)
    { // if data_to_cmp elready exit will return nullptr
        if (data == data_to_cmp && parent)
        {
            return parent;
        }
        else if (data == data_to_cmp)
        {
            return nullptr;
        }
        if (data < data_to_cmp)
        {
            if (right)
            {
                return right->find_parent(data_to_cmp);
            }
        }
        else if (data > data_to_cmp)
        {
            if (left)
            {
                return left->find_parent(data_to_cmp);
            }
        }

        return this;
    }

    RankAVLTreeNode *FindFirstInOrder(RankAVLTreeNode *root)
    {
        if (root->left)
        {
            return FindFirstInOrder(root->left);
        }
        return root;
    }

   RankAVLTreeNode *FindNextInOrder(RankAVLTreeNode *root)
    {
        if(root->right)
        {
            return root->FindFirstInOrder(root->right);
        }
        else
        {
            while(root->parent)
            {
                if(root->parent->getDataReference() > root->getDataReference())
                {
                    return root->parent;
                }
                else
                {
                    root = root->parent;
                }
            }
        }

        return nullptr; //the root sanded was the biggest in tree
    }
    /**
     * find_in_tree_by_data
     * @param data_to_cmp
     * @return pointer to node
     */
    RankAVLTreeNode *search_in_tree_by_data(const T &data_to_cmp) //converted to find_in_tree_by_data
    {
    }

    RankAVLTreeNode *insert(const T &data)
    {
        RankAVLTreeNode *parent_node = find_parent(data);

        RankAVLTreeNode *new_child = new RankAVLTreeNode(data, cmp, parent_node, nullptr, nullptr);

        if (parent_node->data < data)
        {
            parent_node->right = new_child;
        }
        else
        {
            parent_node->left = new_child;
        }
        new_child->parent = parent_node;

        while (parent_node)
        {
            parent_node->rotate();

            parent_node->fix_height_and_rank();
            parent_node = parent_node->parent;
        }

        return new_child;
    }
    template <typename Func>
    void inorder(Func func)
    {
        if (left)
            left->inorder(func);
        func(this);
        if (right)
            right->inorder(func);
    }

    T *getDataPointer()
    {
        return &data;
    }
    T &getDataReference()
    {
        return data;
    }

    RankAVLTreeNode *get_root()
    {
        if (this == nullptr)
        {
            return nullptr;
        }
        RankAVLTreeNode *temp = this;
        while (temp != nullptr && temp->parent != nullptr)
        {
            temp = temp->parent;
        }
        return temp;
    }

  

    RankAVLTreeNode<T, Cmp> *rotate_right();
    RankAVLTreeNode<T, Cmp> *rotate_left();

    bool rotate();

    void fix_height_and_rank();
    void SwitchLocationsWithLeaf(RankAVLTreeNode<T, Cmp> *a_child);
    void move_to_parent(RankAVLTreeNode<T, Cmp> *old_child, RankAVLTreeNode<T, Cmp> *new_child);
    int get_height();
    int get_rank();
    int CalcBalanceFactor() const;
    void delete_tree()
    {
        if (this == nullptr)
        {
            return;
        }
        if (left != nullptr)
        {
            left->delete_tree();
        }

        if (right != nullptr)
        {
            right->delete_tree();
        }

        delete this;
    }
};
template <class T, class Cmp>
RankAVLTreeNode<T, Cmp> *RankAVLTreeNode<T, Cmp>::rotate_right()
{

    RankAVLTreeNode *temp = left;

    RankAVLTreeNode *temp2 = this;

    RankAVLTreeNode *temp3 = this->parent;
    //std::cout<<temp->data<<temp2->data<<temp3->data<<std::endl;
    bool is_left_son = true;
    if (temp3 != nullptr && temp3->right == this)
    {
        is_left_son = false;
    }

    move_to_parent(temp, left->right);

    temp->right = temp2;
    temp2->parent = temp;
    temp->parent = temp3;
    if (temp3 != nullptr && is_left_son)
    {
        temp3->left = temp;
    }
    if (temp3 != nullptr && !is_left_son)
    {
        temp3->right = temp;
    }

    fix_height_and_rank();

    parent->fix_height_and_rank();

    return this;
}
template <class T, class Cmp>
RankAVLTreeNode<T, Cmp> *RankAVLTreeNode<T, Cmp>::rotate_left()
{
    RankAVLTreeNode<T, Cmp> *temp_right = right;
    RankAVLTreeNode<T, Cmp> *temp_this = this;
    RankAVLTreeNode<T, Cmp> *temp_parent = this->parent;
    bool is_left_son = true;
    if (temp_parent != nullptr && temp_parent->right == this)
    {
        is_left_son = false;
    }

    move_to_parent(temp_right, right->left);

    temp_right->left = temp_this;
    temp_this->parent = temp_right;
    temp_right->parent = temp_parent;
    if (temp_parent != nullptr && is_left_son)
    {
        temp_parent->left = temp_right;
    }
    if (temp_parent != nullptr && !is_left_son)
    {
        temp_parent->right = temp_right;
    }
    fix_height_and_rank();
    parent->fix_height_and_rank();

    return this;
}
template <class T, class Cmp>
void RankAVLTreeNode<T, Cmp>::move_to_parent(RankAVLTreeNode<T, Cmp> *old_child, RankAVLTreeNode<T, Cmp> *new_child)
{
    if (!old_child)
    {
        return;
    }
    if (!(old_child->parent))
    {
        return;
    }

    RankAVLTreeNode *parent = old_child->parent;

    if (parent->left == old_child)
    {
        parent->left = new_child;
        if (new_child != nullptr)
        {
            new_child->parent = parent;
        }
    }
    else if (parent->right == old_child)
    {
        parent->right = new_child;
        if (new_child != nullptr)
        {
            new_child->parent = parent;
        }
    }
   
}

template <class T, class Cmp>
bool RankAVLTreeNode<T, Cmp>::rotate()
{

    int balance_factor = CalcBalanceFactor();

    if (balance_factor > -2 && balance_factor < 2)
    {
        fix_height_and_rank(); //meyotar ledaaty

        return false;
    }
    if (balance_factor == -2)
    {
        if (right)
        {
            if (right->CalcBalanceFactor() > 0)
            {
                right->rotate_right(); //RL
            }
        }

        rotate_left();

        return true;
    }

    //balance_factor==2
    if (left)
    {
        if (left->CalcBalanceFactor() < 0)
        {
            left->rotate_left(); //LR
          
          
        }
        rotate_right();
    }
    return false;
}
template <class T, class Cmp>
void RankAVLTreeNode<T, Cmp>::fix_height_and_rank()
{
    /*
    if (this == nullptr) //can it be?
    {
        return;
    }
     */

    height = get_height();

    rank = get_rank();
}
template <class T, class Cmp>
void RankAVLTreeNode<T, Cmp>::SwitchLocationsWithLeaf(RankAVLTreeNode<T, Cmp> *a_child)
{

    if (this == a_child)
    {
        return;
    }

    if (!a_child)
    {
        return;
    }

    T t_temp = data;
    data = a_child->data;
    a_child->data = t_temp;
}

template <class T, class Cmp>
int RankAVLTreeNode<T, Cmp>::get_height()
{
    int left_height = 0;

    int right_height = 0;
    if (!left && !right)
    {
        return 0;
    }
    if (left)
    {
        left_height = left->height;
    }
    if (right)
    {
        right_height = right->height;
    }
    if (left_height > right_height)
    {
        height = 1 + left_height;
    }
    else
    {
        height = 1 + right_height;
    }

    return height;
}
template <class T, class Cmp>
int RankAVLTreeNode<T, Cmp>::get_rank()
{
    int left_rank = 0;
    int right_rank = 0;
    if (left)
    {
        left_rank = left->rank;
    }
    if (right)
    {
        right_rank = right->rank;
    }
    rank = 1 + left_rank + right_rank;
    return rank;
}
template <class T, class Cmp>
int RankAVLTreeNode<T, Cmp>::CalcBalanceFactor() const
{
    int left_height = -1;
    int right_height = -1;



    if (right)
    {
        right_height = right->height;
    }
    if (left)
    {
        left_height = left->height;
    }

    return left_height - right_height;
}

#endif //DATA_STRUCTURE_RANKAVLTREENODE_H
