#pragma once

class MySqrt {
    double state;
public:
    MySqrt(double number);
    operator double () const;
};
