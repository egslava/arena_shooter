#pragma once

#include <exception>
#include <string>

#ifndef NDEBUG
#define EXCEPT_D
#else
#define EXCEPT_D noexcept
#endif



struct MyException : public std::exception{
    std::string _description;
//    MyException(const char *description):description(description){}
    MyException(std::string description):_description(description) {}

//    virtual operator const char *() const {
//        return description;
//    }
    virtual const char* what() const noexcept{
        return _description.c_str();
    }

//    ~MyException(){}
};


struct MyIllegalStateException : public MyException {
    MyIllegalStateException (const char *description): MyException (description){}
};


struct MyIndexOutOfBoundException : public MyIllegalStateException {
    MyIndexOutOfBoundException (const char *description): MyIllegalStateException (description){}
};


struct MySDLException : public MyException {
    MySDLException(const char *description):MyException(description){}
};


struct MyGlException: public MyException {
    MyGlException(const char *description):MyException(description){}
};


struct MyShaderException: public MyGlException {
    MyShaderException(const char *description):MyGlException(description){}
};

struct MyFileException: public MyException {
    MyFileException(const char *description):MyException(description){}
};

struct MyUnsupportedFileException : public MyFileException {
    MyUnsupportedFileException(const char *description):MyFileException(description){}
};
