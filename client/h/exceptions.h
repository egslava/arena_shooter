#pragma once


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


struct MySDLException : public MyException {
    MySDLException(const char *description):MyException(description){}
};


struct MyGlException: public MyException {
    MyGlException(const char *description):MyException(description){}
};


struct MyShaderException: public MyGlException {
    MyShaderException(const char *description):MyGlException(description){}
};
