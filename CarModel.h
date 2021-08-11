//
// Created by liran on 07/05/2021.
//

#ifndef DATA_STRUCTURE_CARMODEL_H
#define DATA_STRUCTURE_CARMODEL_H
#include "RankAVLTreeNode.h"
#include "abstract_car.h"

class Compare_Car_To_ID{
public:
    int operator()(const AbsractCar a, const int Id_value)
    {
        if(a.getTypeId()<Id_value) {
            return -1;
        }
        else if (a.getTypeId()>Id_value)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
};
class CarModel: public AbsractCar{
   
   public:
    int grade_;
    int model_ID_;
    bool have_been_bought_; //to know if it is on zeros tree or grade tree ///im not sure its necessary
    int num_of_seles_;
    RankAVLTreeNode<CarModel,Compare_Car_To_ID>* address_in_tree_;


    CarModel(const int type_id, const int model_ID) :
            AbsractCar(type_id) ,grade_(0), model_ID_(model_ID) ,
            have_been_bought_(false),
            num_of_seles_(0),
            address_in_tree_(nullptr){}

    CarModel(const CarModel& c)
    {
        type_id_=c.type_id_;
        grade_=c.grade_;
        num_of_seles_=c.num_of_seles_;
        address_in_tree_=c.address_in_tree_;
        model_ID_=c.model_ID_;

    } 
  
    void setGrade(int grade){
        grade_=grade;
    };

    int getGrade() const {
        return grade_;
    }
    int getModelID() const {return model_ID_;}
   // RankAVLTreeNode<Car_model,Compare* get_address_in_grade_tree(); ///need to make

friend bool operator<(const CarModel& a ,const CarModel& b)  {
    if (a.grade_<b.grade_) {
        return true;
    } else if (a.grade_>b.grade_){
        return false;
    } else if ( (a.type_id_) < (b.type_id_) ){
        return true;
    } else if (a.type_id_>b.type_id_){
        return false;
    } else if (a.model_ID_<b.model_ID_){
        return true;
    }
    else return false; //a->id_>b->id_ or a->id_==b->id_
}
   
   friend bool operator>(const CarModel& a,const CarModel& b) {
    return (b<a) ;
}

friend bool operator==(const CarModel& a,const CarModel& b)  {
    return (!(a<b)&&!(a>b));
}
   
    bool operator==(const CarModel* b) ;
    bool operator>(const CarModel* b) ;
  int Get_Key()
   {
       return model_ID_;
   }
   
};

#endif //DATA_STRUCTURE_CARMODEL_H
