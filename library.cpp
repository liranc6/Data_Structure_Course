/****************************************************************************/
/*                                                                          */
/* This file contains the interface functions                               */
/* you should use for the wet ex 1                                          */
/*                                                                          */
/****************************************************************************/



#include "library.h"
#include "Container.h"






void *Init() 
{

    auto* DS = new Container();

    return (void*)DS;
}

StatusType AddCarType(void *DS, int typeID, int numOfModels)
{
       if(DS==nullptr)
    {
        return INVALID_INPUT;
    }
    
    return static_cast<StatusType>(((Container *) DS)->AddCarType(typeID, numOfModels));

 
}

StatusType RemoveCarType(void *DS, int typeID) 
{
    if (DS== nullptr || typeID<=0)
    {
        return INVALID_INPUT;
    }
    return static_cast<StatusType>(((Container *) DS)->RemoveCarType(typeID));
}


StatusType SellCar(void *DS, int typeID, int modelID)
{
      if(DS==nullptr)
    {
        return INVALID_INPUT;
    }
   return static_cast<StatusType>(((Container *) DS)->SellCar(typeID, modelID));
}

StatusType MakeComplaint(void *DS, int typeID, int modelID, int t) 
{
    
   
    if (t <= 0 || modelID < 0 || !DS)
    {
        return INVALID_INPUT;
    }
    return static_cast<StatusType>(((Container *) DS)->MakeComplaint(typeID, modelID, t));
}


StatusType GetBestSellerModelByType(void *DS, int typeID, int * modelID)
{
   
    if (typeID<0||DS== nullptr)
    {
        return INVALID_INPUT;
    }
    return static_cast<StatusType>(((Container *) DS)->GetBestSellerModelByType(typeID, modelID));
}

StatusType GetWorstModels(void *DS, int numOfModels, int *types, int *models) 
{
      if(DS==nullptr)
    {
        return INVALID_INPUT;
    }
    return static_cast<StatusType>(((Container *) DS)->GetWorstModels(numOfModels, types, models));
}

void Quit(void** DS) 
{
   

     ((Container *)  (*DS) )-> Quit();
    
*DS=nullptr;
   
  return;

}


