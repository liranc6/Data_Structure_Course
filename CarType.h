//
// Created by liran on 07/05/2021.
//
#ifndef DATA_STRUCTURE_CARTYPE_H
#define DATA_STRUCTURE_CARTYPE_H
#include "abstract_car.h"
#include "RankAVLTree.h"
#include "CarModel.h"
#include "RankAVLTreeNode.h"
//helper functions

#include "RankAVLTreeNode.h" 



class CarType: public AbsractCar{
public:

    int num_of_models_in_type_;
    int popular_model_in_same_type_;
    int num_of_sales_popular_model_in_same_type_;
    int most_popular_model_in_sub_AVL_;
    int type_num_of_popular_model_in_sub_AVL;
    int num_of_sales_most_popular_model_in_sub_AVL_;
    RankAVLTree<CarModel, Compare_Car_To_ID> sub_tree_of_models;


    
    RankAVLTreeNode<CarType, Compare_Car_To_ID>* address_in_zeros_tree_; 
    RankAVLTreeNode<CarModel, Compare_Car_To_ID>** models_array;
    
    CarType(int id, int numOfModels): AbsractCar(id), num_of_models_in_type_(numOfModels),sub_tree_of_models(),models_array(nullptr){
        sub_tree_of_models.root_=nullptr;}
     
  static   RankAVLTreeNode<CarModel,Compare_Car_To_ID>* Build_Sub_Tree(RankAVLTreeNode<CarModel,Compare_Car_To_ID>* parent_ ,int id,int begin,int end,int height)
   {
       
      if(begin>end)
        {
            return nullptr;
       }
   
       int mid= (begin+end)/2;
    CarModel new_model= CarModel(id,mid);

    
    RankAVLTreeNode<CarModel,Compare_Car_To_ID> *node= new RankAVLTreeNode<CarModel,Compare_Car_To_ID> (new_model);
      node->height=height;
      node->parent=parent_; 
       node->left=Build_Sub_Tree(node,id,begin,mid-1,height-1);
       node->right=Build_Sub_Tree(node,id,mid+1,end,height-1);

return node;
     }
 
 // used for computing trees height
 int log2(int arr)
 {
     int to_return=0;
     int base =1;
     while(base<=arr)
     {
         base*=2;
         to_return++;
     }
     return to_return-1 ;
 }

   ~CarType()
   {
       
       if(models_array!=nullptr)
       
       {delete[] models_array;}
       if(sub_tree_of_models.root_!=nullptr)
       {
           sub_tree_of_models.root_->delete_tree();
       }
   }

CarType(const CarType &b): AbsractCar(b.type_id_),sub_tree_of_models(),models_array(nullptr)
    {
 sub_tree_of_models.root_=nullptr;
 address_in_zeros_tree_=b.address_in_zeros_tree_;
 num_of_models_in_type_=b.num_of_models_in_type_;

    }
    
    // iniatte models array
 void Iniate_array()
    {
     
        models_array = new RankAVLTreeNode<CarModel, Compare_Car_To_ID>* [num_of_models_in_type_];

        for (int i=0; i<num_of_models_in_type_ ; i++) 
        {
           models_array[i]=nullptr;  
        }
    }
    
    friend bool operator<(const CarType & a, const CarType & b)
    {
        return b.type_id_>a.type_id_;
    }


    bool operator>(const CarType & b) {
        return b.type_id_<type_id_;
    }
 friend  bool operator==(const CarType & a,const CarType& b)
{
    return (!(a<b))&&(!(b<a))    ;
}

    int Get_Key()
    {
        return type_id_;
    }
   
    void Iniate_Sub_Tree()
    {
        
        RankAVLTreeNode<CarModel,Compare_Car_To_ID>* root=Build_Sub_Tree(nullptr,type_id_,0,num_of_models_in_type_-1,log2(num_of_models_in_type_) );
     
        sub_tree_of_models.root_=root;
         sub_tree_of_models.update_smallest();
    }

    /**
     * Changes best seller of sub AVL
     * @param b
     */
    void ChangeBestSeller(CarType* b)
    {
        num_of_sales_most_popular_model_in_sub_AVL_=b->num_of_sales_most_popular_model_in_sub_AVL_;
        most_popular_model_in_sub_AVL_=b->most_popular_model_in_sub_AVL_;
        type_num_of_popular_model_in_sub_AVL=b->type_num_of_popular_model_in_sub_AVL;
    }
   
  

};

  


#endif //DATA_STRUCTURE_CARTYPE_H
