#include "mysqrt.h"
#include <stdio.h>
#include <stdlib.h>

//#include "Table.h"

MySqrt::MySqrt(double number){
    this->state = number * number;
    printf("MySqrt::MySqrt()\n");
}

MySqrt::operator double() const {
    return this->state;
}


//MySqrt sqrt(10);
