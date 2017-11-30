#ifndef PROJECT_SETTINGS_H
#define PROJECT_SETTINGS_H

#include <cstddef>
#include <initializer_list>


struct Settings
{
    std::initializer_list<size_t> dimensions = {10, 10, 10};
    unsigned bank_number = 16u;
    unsigned bank_size  = 256u;
};


#endif //PROJECT_SETTINGS_H
