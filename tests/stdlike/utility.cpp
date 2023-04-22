#include <stdlike/utility.hpp>
#include <stdlike/function.hpp>
#include <stdlike/shared_ptr.hpp>
#include <stdlike/unique_ptr.hpp>
#include <stdlike/weak_ptr.hpp>

int Func(const int* x, int y) {
	return (*x) + y;
}

struct Foo {
	int a = 0;

	int Func(int x) {
		return x + 1;
	}
};

int main() {
	stdlike::Function<int (const int*, int)> f2(Func);
	int x = 10;
	f2(&x, 8);

	stdlike::Function<int (Foo, int)> f1(&Foo::Func);
	Foo foo;
	f1(foo, 5);

	stdlike::SharedPtr<Foo> ptr = stdlike::MakeShared<Foo>(6);

	stdlike::SharedPtr<int> sptr(new int(5));
	stdlike::WeakPtr<int> wptr(sptr);

	return 0;
}