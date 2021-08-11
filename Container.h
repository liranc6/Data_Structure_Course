
#ifndef DATA_STRUCTURE_CONTAINER_H
#define DATA_STRUCTURE_CONTAINER_H
#include "RankAVLTree.h"
#include "RankAVLTreeNode.h"
#include "CarType.h"
#include "CarModel.h"
#include "ModelSells.h"





typedef enum
{
    CON_SUCCESS = 0,
    CON_FAILURE = -1,
    CON_ALLOCATION_ERROR = -2,
    CON_INVALID_INPUT = -3
} Con_StatusType;

class Container
{

public:
    RankAVLTree<CarType, Compare_Car_To_ID> new_models_tree_;
    RankAVLTree<CarType, Compare_Car_To_ID> car_type_tree_;
    RankAVLTree<CarModel, Compare_Car_To_ID> grade_tree_;
    RankAVLTree<ModelSells, Compare_Car_To_ID> best_seller_tree_;

    RankAVLTreeNode<CarModel, Compare_Car_To_ID> *smallest_in_new_models_tree_;
   
    RankAVLTreeNode<CarModel, Compare_Car_To_ID> *smallest_in_grade_tree_;

    int sum_models_counter_;

    Container() : new_models_tree_(RankAVLTree<CarType, Compare_Car_To_ID>()),
                  car_type_tree_(RankAVLTree<CarType, Compare_Car_To_ID>()),
                  grade_tree_(RankAVLTree<CarModel, Compare_Car_To_ID>()),
                  best_seller_tree_(RankAVLTree<ModelSells, Compare_Car_To_ID>()),
                  smallest_in_new_models_tree_(nullptr), smallest_in_grade_tree_(nullptr),
                  sum_models_counter_(0) {}

    Con_StatusType AddCarType(int typeID, int numOfModels)
    {
     

        if (numOfModels <= 0 || typeID <= 0)
        {

            return CON_INVALID_INPUT;
        }
    // will be used for serach
        CarType new_type(typeID, numOfModels);

        if (car_type_tree_.find_by_data(new_type) != nullptr)
        {
            return CON_FAILURE;
        }

// inserting new vertex to cartype tree and new models tree
        RankAVLTreeNode<CarType, Compare_Car_To_ID> *new_type_ptr = car_type_tree_.insert(new_type);
        if(new_type_ptr==nullptr)
        {
            return CON_ALLOCATION_ERROR;
        }

        RankAVLTreeNode<CarType, Compare_Car_To_ID> *to_check= new_type_ptr->data.address_in_zeros_tree_ = new_models_tree_.insert(new_type);
           if(to_check==nullptr)
        {
            return CON_ALLOCATION_ERROR;
        }
        new_type_ptr->data.address_in_zeros_tree_->data.Iniate_Sub_Tree();

        new_type_ptr->data.Iniate_array();

        new_type_ptr->data.num_of_sales_popular_model_in_same_type_ = 0;
        new_type_ptr->data.popular_model_in_same_type_ = 0;
        sum_models_counter_ += numOfModels;

        return CON_SUCCESS;
    }

    /**
     * SellCar
     *
     * @param typeID
     * @param typeID
     * @param modelID
     * @return
     */
    Con_StatusType SellCar(int typeID, int modelID)
    {
     
        if (modelID < 0 || typeID <= 0)
        {
            return CON_INVALID_INPUT;
        }

        //search if type exist`
        CarType new_type(typeID, 0);
        RankAVLTreeNode<CarType, Compare_Car_To_ID> *car_type_node = car_type_tree_.find_by_data(new_type);
        if (car_type_node == nullptr)
        {
            return CON_FAILURE;
        } //typeID not exist

        //check if modelID is valid
        if (car_type_node->data.num_of_models_in_type_ <= modelID)
        {
            return CON_FAILURE;
        }

        // check if model model has ever been bought
        if (car_type_node->data.models_array[modelID] == nullptr) //model has never been bought
        {
       

            CarModel to_search(typeID, modelID);
            //copy from new_models tree
            RankAVLTreeNode<CarModel, Compare_Car_To_ID> *address_in_sub_models = car_type_node->data.address_in_zeros_tree_->data.sub_tree_of_models.find_by_data(to_search);
  

            CarModel &to_remove_from_sub_tree = address_in_sub_models->data;
            CarModel to_insert_to_grade_tree = to_remove_from_sub_tree;

            //update copy grade
            to_insert_to_grade_tree.grade_ = 10;       //first sale
            to_insert_to_grade_tree.num_of_seles_ = 1; //first sale
            //to_insert_to_grade_tree.have_been_bought_= true;

            // add the copy to the grade tree and save location on car_type
            RankAVLTreeNode<CarModel, Compare_Car_To_ID> *to_array = grade_tree_.insert(to_insert_to_grade_tree);
            if (to_array == nullptr) //insert failed
            {
                return CON_ALLOCATION_ERROR;
            }
            car_type_node->data.models_array[modelID] = to_array;

            //add new node to best_seller_tree_
            ModelSells new_sells(1, typeID, modelID);
            if (best_seller_tree_.insert(new_sells) == nullptr)
            {
                grade_tree_.remove_by_data(to_insert_to_grade_tree);
                return CON_ALLOCATION_ERROR;
            }

            //delete from new_models tree
            car_type_node->data.address_in_zeros_tree_->data.sub_tree_of_models.remove_by_data(to_remove_from_sub_tree);
            if (car_type_node->data.address_in_zeros_tree_->data.sub_tree_of_models.root_ == nullptr) //now all of the models in type has been sold
            {
                new_models_tree_.remove_by_value(typeID);
               car_type_node->data.address_in_zeros_tree_=nullptr;
            }

            update_best_seller_in_same_type(&to_insert_to_grade_tree, &(car_type_node->data));
    

            return CON_SUCCESS;
        }
        else //model has been sold already once -> change info in grade tree and best seller tree
        {
            //change info in grade tree
         

            CarModel &to_remove_from_grade_tree = car_type_node->data.models_array[modelID]->data; ///what if not reference?
            CarModel to_insert_to_grade_tree = to_remove_from_grade_tree;
            to_insert_to_grade_tree.grade_ += 10;
            to_insert_to_grade_tree.num_of_seles_ += 1;
            RankAVLTreeNode<CarModel, Compare_Car_To_ID> *new_node = grade_tree_.insert(to_insert_to_grade_tree);
            if (new_node == nullptr) //insert failed
            {
                return CON_ALLOCATION_ERROR;
            }
            car_type_node->data.models_array[modelID] = new_node;

            //change info in sells tree
            ModelSells sells_copy(to_remove_from_grade_tree.num_of_seles_, typeID, modelID);
          

            ModelSells to_remove_from_sells_tree = best_seller_tree_.find_by_data(sells_copy)->data;
            ModelSells to_insert_to_sells_tree = to_remove_from_sells_tree;
            to_insert_to_sells_tree.sells_ += 1;
            RankAVLTreeNode<ModelSells, Compare_Car_To_ID> *new_sells_node = best_seller_tree_.insert(to_insert_to_sells_tree);
            if (new_sells_node == nullptr) //insert failed
            {
                grade_tree_.remove_by_data(to_insert_to_grade_tree);
                return CON_ALLOCATION_ERROR;
            }
            else //both inserts had SUCCESS
            {
                grade_tree_.remove_by_data(to_remove_from_grade_tree);
                best_seller_tree_.remove_by_data(to_remove_from_sells_tree);
            }

            update_best_seller_in_same_type(&to_insert_to_grade_tree, &(car_type_node->data));

       
            return CON_SUCCESS;
        }

    }

    void update_best_seller_in_same_type(CarModel *model_sold, CarType *type_to_update)
    {
        if (model_sold->num_of_seles_ > type_to_update->num_of_sales_popular_model_in_same_type_) /*first sale in type*/
        {
            type_to_update->num_of_sales_popular_model_in_same_type_ = model_sold->num_of_seles_;
            type_to_update->popular_model_in_same_type_ = model_sold->model_ID_;
        }
        else if (model_sold->num_of_seles_ == type_to_update->num_of_sales_popular_model_in_same_type_)
        {
            if (model_sold->model_ID_ < type_to_update->popular_model_in_same_type_)
            {
                type_to_update->popular_model_in_same_type_ = model_sold->model_ID_;
            }
        }
    }

    /**
      * the function removes all models of specific type from all trees.
      * first find type in Typetree, if type is best seller - we take the best seller from type lest (smaller then type)
      * and fixing all the nodes above the type in tree who still have the info that the deleted node is best seller.
      * then find type in Typetree then enter array of models nodes pointers. in each pointer delete its from tree.
      * (every model is only in one tree)
      * then deleting type node
      * @param typeID
      * @return  StatusType:     SUCCESS or INVALID_INPUT
      */

    Con_StatusType RemoveCarType(int typeID) ///how to delete the CarType itself and his models?
    {
      

        CarType to_search(typeID, 0);
        RankAVLTreeNode<CarType, Compare_Car_To_ID> *node_to_remove = car_type_tree_.find_by_data(to_search);

        if (typeID <= 0)
        {
            return CON_INVALID_INPUT;
        }

        if (node_to_remove==nullptr)
        {
            return CON_FAILURE;
        }

        //fixing sum_models_counter_
        sum_models_counter_ -= node_to_remove->data.num_of_models_in_type_;

        //removing all models were sold at least once from grade tree and best seller tree
        for (int i = 0; i < node_to_remove->data.num_of_models_in_type_ ; i++)
        {

            if (node_to_remove->data.models_array[i] == nullptr)
            {
                continue;
            }

            CarModel model_to_remove = node_to_remove->data.models_array[i]->getDataReference();
            ModelSells sells_to_remove(node_to_remove->data.models_array[i]->data.num_of_seles_, typeID, i);
            grade_tree_.remove_by_data(model_to_remove);       //removing all models from grade tree
            best_seller_tree_.remove_by_data(sells_to_remove); //removing all models from best seller tree
        }

        //removing type from new models tree
        new_models_tree_.remove_by_data(node_to_remove->data);

        //removing type from car type tree
        car_type_tree_.remove_by_data(node_to_remove->data);


        return CON_SUCCESS;
    }

    /**
     * filing a complaint for model in type
     * the func finds the model in grade tree and changes the grade of the model.
     * to save the tree keep the tree sorted  we delete the model from it and then inserting it again.
     * if the model not in grade tree so its never sold or not exist.
     *
     * @param DS
     * @param typeID
     * @param modelID
     * @param t
     * @return INVALID_INPUT if typeId or modelID not inserted or t<0
     */
    Con_StatusType MakeComplaint(int typeID, int modelID, int t)
    {
      
       
   
         if (typeID<=0||modelID<0)
         {
             return CON_INVALID_INPUT;
         }
        RankAVLTreeNode<CarType, Compare_Car_To_ID> *type_of_model_node = car_type_tree_.find_by_value(typeID);
        if (type_of_model_node == nullptr)
        {
            return CON_FAILURE;
        }
        CarType &type_of_model = type_of_model_node->data;

        if (type_of_model.num_of_models_in_type_ <= modelID)
        {
            return CON_FAILURE;
        }

       
        if(t>100) // no changes needed
        {
            return CON_SUCCESS; 
        }
        CarModel& car_to_remove = type_of_model.models_array[modelID]->getDataReference();
        CarModel car_to_insert = car_to_remove;

        int grade = car_to_insert.getGrade();
        int reduce = 100 / t; ///rounded down to 0
        car_to_insert.setGrade(grade - reduce);

        if(grade_tree_.insert(car_to_insert)== nullptr)
        {
            return CON_ALLOCATION_ERROR;
        }
        
        else
        {
            grade_tree_.remove_by_data(car_to_remove);
        }
         
        //we removed and reinserted the model, need to fix the array in carType
        type_of_model.models_array[modelID] = grade_tree_.find_by_data(car_to_insert); //assumption: not nullptr
      
        //assumption: maybe the new grade is now the smallest in grade tree ->  the tree fixed its smallest_leaf
        return CON_SUCCESS;
       
    }

    /**

     * @param DS
     * @param typeID
     * @param modelID
     * @return
     */
    Con_StatusType GetBestSellerModelByType(int typeID, int *modelID)
    {
   
        if (typeID == 0)
        {
         

            
            if (car_type_tree_.root_ == nullptr)
            {
                return CON_FAILURE;
            }
            if (best_seller_tree_.root_ == nullptr)
            {
                *modelID = 0;
            }
            else
            {
                *modelID = best_seller_tree_.smallest_leaf_->data.model_ID_;
            }
            return CON_SUCCESS;
        }

        RankAVLTreeNode<CarType, Compare_Car_To_ID> *by_type = car_type_tree_.find_by_value(typeID);
        if (by_type == nullptr) //the type not exist
        {
            return CON_FAILURE;
        }

        CarType &type = by_type->data;

        *modelID = type.popular_model_in_same_type_;

        return CON_SUCCESS;
    }

    Con_StatusType GetWorstModels(int numOfModels, int *types, int *models)
    {
   
  
   
        if (!types || !models || numOfModels <= 0 )
        {
            return CON_INVALID_INPUT;
        }
        if(numOfModels > sum_models_counter_)
        {
            return CON_FAILURE;
        }

        int counter = 0;

        RankAVLTreeNode<CarModel, Compare_Car_To_ID> *to_print = grade_tree_.smallest_leaf_;
        RankAVLTreeNode<CarModel, Compare_Car_To_ID> *to_print_new_model = nullptr;
        RankAVLTreeNode<CarType, Compare_Car_To_ID> *type_in_new_models = new_models_tree_.smallest_leaf_;
        if (type_in_new_models != nullptr)
        {
            to_print_new_model = type_in_new_models->data.sub_tree_of_models.smallest_leaf_;
        }

        for (; counter < numOfModels; counter++)
        {

       
            
            if (to_print == nullptr && to_print_new_model != nullptr)
            {
                RankAVLTreeNode<CarModel, Compare_Car_To_ID> *temp = printFunc_aux(to_print_new_model, types, models, counter);

                if (temp == nullptr)
                {
                 
                    type_in_new_models = type_in_new_models->FindNextInOrder(type_in_new_models);
                
                    if (type_in_new_models != nullptr)
                    {
                        to_print_new_model = type_in_new_models->data.sub_tree_of_models.smallest_leaf_;
                    }
                    else
                    {
                        to_print_new_model = nullptr;
                    }
                }
                else
                {to_print_new_model=temp;}
            }
            else if (to_print != nullptr && to_print_new_model == nullptr)
            {
                to_print = printFunc_aux(to_print, types, models, counter);
            }
            else if (to_print->getDataPointer()->grade_ >= 0 && to_print_new_model != nullptr) //compare with new_models tree
            {
                if (to_print_new_model->getDataReference() < to_print->getDataReference())
                {
                    RankAVLTreeNode<CarModel, Compare_Car_To_ID> *temp = printFunc_aux(to_print_new_model, types, models, counter);
                    if (temp == nullptr)
                    {
                        type_in_new_models = type_in_new_models->FindNextInOrder(type_in_new_models);
                 
                        if (type_in_new_models != nullptr)
                        {
                            to_print_new_model = type_in_new_models->data.sub_tree_of_models.smallest_leaf_;
                                       
                        }
                        else
                        {
                            to_print_new_model = nullptr;
                        }
                    }
                    else
                    {to_print_new_model=temp;}
                }
                else if (to_print_new_model->getDataReference() > to_print->getDataReference())
                {
                    to_print = printFunc_aux(to_print, types, models, counter);
                }
         
            }
            else
            {
                to_print = printFunc_aux(to_print, types, models, counter);
            }
        }

    
        return CON_SUCCESS;
    };

    RankAVLTreeNode<CarModel, Compare_Car_To_ID> *printFunc_aux(RankAVLTreeNode<CarModel, Compare_Car_To_ID> *to_print, int *types, int *models, int i)
    {
        types[i] = to_print->getDataPointer()->type_id_;
        models[i] = to_print->getDataPointer()->getModelID();
        return to_print->FindNextInOrder(to_print);
    }

    void Quit()
    {
        grade_tree_.delete_tree();
        new_models_tree_.delete_tree();
        car_type_tree_.delete_tree();
        best_seller_tree_.delete_tree();

    
        delete this;
    }

  
};

#endif //DATA_STRUCTURE_CONTAINER_H
