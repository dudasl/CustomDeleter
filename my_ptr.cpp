// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <memory>
#include <functional>

#define BIND_BASED_CODE		1
#define DELETER_BASED_CODE	2
#define LAMBDA_BASED_CODE	3
#define MY_PTR_BASED_CODE	LAMBDA_BASED_CODE

#if MY_PTR_BASED_CODE == BIND_BASED_CODE
void my_free(int x, int* p)
{
	std::cout << "my_free - *p: " << *p << "\nx: " << x << "\n";

	delete p;
}

class my_ptr : public std::unique_ptr<int, std::_Bind<true, void, void(*const)(int, int *), const int&, std::_Ph<1> &>>
{
public:
	my_ptr(int* p, const int x) : std::unique_ptr<int, std::_Bind<true, void, void(*const)(int, int *), const int&, std::_Ph<1> &>>(p, std::bind(my_free, x, std::placeholders::_1)) {}

	my_ptr(my_ptr&& other) : std::unique_ptr<int, std::_Bind<true, void, void(*const)(int, int *), const int&, std::_Ph<1> &>>(std::move(other)) {}
};
#elif MY_PTR_BASED_CODE == DELETER_BASED_CODE
class my_ptr_deleter {
private:
	int _x;

public:
	explicit my_ptr_deleter(int x) : _x(x) {}

	void operator() (int* p) { std::cout << "my_ptr_deleter - *p: " << *p << "\nx: " << this->_x << '\n'; delete p; }
};

class my_ptr : public std::unique_ptr<int, my_ptr_deleter>
{
public:
	my_ptr(int* p, const int x) : std::unique_ptr<int, my_ptr_deleter>(p, my_ptr_deleter(x)) {}

	my_ptr(my_ptr&& other) : std::unique_ptr<int, my_ptr_deleter>(std::move(other)) {}
};
#elif MY_PTR_BASED_CODE == LAMBDA_BASED_CODE
void my_free(int x, int* p)
{
	std::cout << "my_free - *p: " << *p << "\nx: " << x << "\n";

	delete p;
}
std::function<void(int*)> deleterType;

class my_ptr : public std::unique_ptr<int, decltype(deleterType)>
{
public:
	my_ptr(int* p, const int x) : std::unique_ptr<int, decltype(deleterType)>(p, [x](int* p) { my_free(x, p); }) {}

	my_ptr(my_ptr&& other) = default; //: std::unique_ptr<int, void(*)(int*)>(std::move(other)) {}
};
#endif

my_ptr create_my_ptr(int initVal, int x)
{
	my_ptr ptr(new int(initVal), x);

	return ptr;
}

int main(int argc, char * argv[])
{
	//int * raw_ptr = new int(10);
	//auto my_deleter = std::bind(my_free, std::placeholders::_1, 2);
	//auto my_lambda = [](int* t) { my_free(3, t); };
	
	//std::unique_ptr<int, std::_Bind<true, void, void(*const)(int *, int), std::_Ph<1> &, int>> ptr(raw_ptr, std::bind(my_free, std::placeholders::_1, 2));
	//std::unique_ptr<int, void(*)(int *)> ptr2(new int(10), [](int* t) { my_free(3, t); });
	my_ptr ptr1(create_my_ptr(10, 20));
	my_ptr ptr2(new int(30), 60);

	return 0;
}
