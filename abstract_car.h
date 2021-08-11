//
// Created by liran on 07/05/2021.
//
#ifndef DATA_STRUCTURE_AbsractCar_H
#define DATA_STRUCTURE_AbsractCar_H
#include <iostream>


class AbsractCar {
public: 
    int type_id_; 


     explicit AbsractCar(const int id): type_id_(id) {}
AbsractCar():type_id_(-1)
{}

AbsractCar(const AbsractCar &b):type_id_(b.type_id_)
{}
    int getTypeId() const {return type_id_;}

   
};


#endif //DATA_STRUCTURE_AbsractCar_H
