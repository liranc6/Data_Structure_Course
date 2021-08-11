#ifndef DATA_STRUCTURE_MODELSELLS_H
#define DATA_STRUCTURE_MODELSELLS_H

class ModelSells
{
public:
    int sells_;
    int type_id_;
    int model_ID_;

    ModelSells(int sells, int type_id, int model_ID) : sells_(sells), type_id_(type_id), model_ID_(model_ID)
    {
    }
    int getModelID()
    {
        return model_ID_;
    }
    int Get_Key()
    {
        return sells_;
    }
    friend bool operator<(const ModelSells &a, const ModelSells &b)
    {
        if (a.sells_ > b.sells_)
        {
            return true;
        }
        else if (a.sells_ < b.sells_)
        {
            return false;
        }
        else if (a.type_id_ < b.type_id_)
        {
            return true;
        }
        else if (a.type_id_ > b.type_id_)
        {
            return false;
        }
        else if (a.model_ID_ < b.model_ID_)
        {
            return true;
        }
        else
        {
            return false;
        }

    }

    friend bool operator>(const ModelSells &a, const ModelSells &b)
    {
        return (b < a);
    }

    friend bool operator==(const ModelSells &a, const ModelSells &b)
    {
        return (!(b < a)) && (!(a < b));
    }

    
};

#endif