#pragma once
#include <cstdint>

class TOUCH {
public:
    TOUCH();
    ~TOUCH();
    
    bool read(uint16_t *x, uint16_t *y);
};