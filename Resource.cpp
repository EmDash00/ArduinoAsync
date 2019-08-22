//
// Created by emdash00 on 8/22/19.
//

#include "Resource.hpp"

T Resource::Get()
{
    return data;
}

void Resource::Set(T val)
{
    data = val;
}
