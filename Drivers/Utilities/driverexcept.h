#pragma once
#include <exception>

class DriverError : public std::exception
{
  private:
    char const * message;
  public:
    DriverError(char const* const _message) throw()
    : message(_message)
    {
        
    }
    virtual char const* what() const throw()
    {
        return message;
    }
};