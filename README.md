# CustomDeleter
This project shows how to use custom deleter with additional param

## Problem
- derive own class from `std::unique_ptr` (in this example `my_ptr` that holds pointer to `int`)
- ctor of class have two parameters:
    1. `int* p` - pointer to holded value
    2. `int x` - additional param to deleter
- deleter of that needs one additional param (in this example `int x`)
- implement class that hide deleter from outside code and class also should not be template, e.g. `my_ptr ptr(new int(10), 20);` should be compilable
- code should be c++11/14 compliant

## Solutions
Solutions should be based on macro `MY_PTR_BASED_CODE` which can be defined as:
- `BIND_BASED_CODE` (1) solution based on binding argument to deleter function
- `DELETER_BASED_CODE` (2) solution based on deleter defined as struct
- `LAMBDA_BASED_CODE` (3) solution based on lambda function as deleter