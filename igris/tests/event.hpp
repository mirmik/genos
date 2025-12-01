#include <igris/event/delegate.h>
#include <igris/event/multiple_delegate.h>

struct A_cls 
{
	int i;
};

int sum(int a, int b) 
{
	return a + b;
} 

int extadd(void * priv, int a, int b) 
{
	return ((A_cls*)priv)->i + a + b;
}

int sub(int a, int b) 
{
	return a - b;
} 

class B
{
public:
	int b = 3;
	int inc(int a, int c) { return a + b; }
};

LT_BEGIN_TEST(igris_test_suite, delegate_test)
{
	int a = 5;
	int b = 3;

	igris::delegate<int,int,int> func;

	func = sum;
	CHECK_EQ(func(a,b), a+b);	

	func = sub;
	CHECK_EQ(func(a,b), a-b);	

	B a_obj;
	a_obj.b = 4;
	func = igris::make_delegate(&B::inc, &a_obj);
	CHECK_EQ(func(7, 100500), 11);	

	A_cls A = {3};
	func = igris::make_delegate(extadd, &A);
	CHECK_EQ(func(1, 2), 6);	

}
LT_END_TEST(delegate_test)


int a = 0;
int b = 0;

void set_a(int i) { a=i; }
void set_b(int i) { b=i; }

LT_BEGIN_TEST(igris_test_suite, multiple_delegate_test)
{
	igris::multiple_delegate<int> func;

	func += set_a;
	func += set_b;

	func(3);

	CHECK_EQ(a, 3);
	CHECK_EQ(b, 3);

	func.clear();

	func(4);
	CHECK_EQ(a, 3);
	CHECK_EQ(b, 3);


	func += set_a;
	func += set_b;

	func -= set_a;
	func(4);
	CHECK_EQ(a, 3);
	CHECK_EQ(b, 4);
}
LT_END_TEST(delegate_test)
