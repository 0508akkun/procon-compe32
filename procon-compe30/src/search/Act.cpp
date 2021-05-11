# include "Act.h"

std::string ActToString(const Act a)
{
    if(a == Act::Move)
    {
        return "move";
    }
    else if(a == Act::Remove)
    {
        return "remove";
    }
    else
    {
        return "stay";
    }
    
}