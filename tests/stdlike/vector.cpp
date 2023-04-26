#include <algorithm>
#include <iostream>
#include <iterator>

#include <stdlike/vector.hpp>

class TestClass {
public:
    TestClass(int val) {
        m_val = val;
    }

    int val() {
        return m_val;
    }

private:
    int m_val = 0;
};

int main() {
    stdlike::Vector<stdlike::Vector<int>> test1(2, stdlike::Vector<int>(3, 1));
    std::cout << "Test #1: (create a vector of vectros of ints)" << std::endl;
    std::cout << test1 << std::endl << std::endl;
    
    test1.Insert(test1.Begin() + 1, stdlike::Vector<int>(2, 2));
    std::cout << "Test #2: (insert into a vector of vectros of ints)" << std::endl;
    std::cout << test1 << std::endl << std::endl;

    test1.Erase(test1.Begin() + 1);
    std::cout << "Test #3: (erase from a vector of vectros of ints)" << std::endl;
    std::cout << test1 << std::endl << std::endl;

    stdlike::Vector<stdlike::Vector<int>> test2 = test1;
    std::cout << "Test #4: (copy a vector of vectros of ints)" << std::endl;
    std::cout << test2 << std::endl << std::endl;

    test1.PushBack(stdlike::Vector<int>(2, 2));
    std::cout << "Test #5: (push_back into a vector of vectros of ints)" << std::endl;
    std::cout << test1 << std::endl << std::endl;

    test2.Swap(test1);
    std::cout << "Test #6: (swap two vectors of vectros of ints)" << std::endl;
    std::cout << test1 << std::endl << test2 << std::endl << std::endl;
    test1.Swap(test2);

    test1.PopBack();
    std::cout << "Test #7: (pop_back from a vector of vectros of ints)" << std::endl;
    std::cout << test1 << std::endl << std::endl;

    test1.Clear();
    std::cout << "Test #8: (Clear a vector of vectros of ints)" << std::endl;
    std::cout << test1.Size() << " " << test1.Capacity() << std::endl << std::endl;

    test1.ShrinkToFit();
    std::cout << "Test #9: (shrink_to_fit a vector of vectros of ints)" << std::endl;
    std::cout << test1.Size() << " " << test1.Capacity() << std::endl << std::endl;

    stdlike::Vector<TestClass> test3(1, TestClass(2));
    std::cout << "Test #10: (create a vector of TestClass objects)" << std::endl;
    std::cout << test3.At(0).val() << std::endl;

    stdlike::Vector<bool> bvector_test(10, true);
    bvector_test.PushBack(false);
    auto found = std::find(bvector_test.begin(), bvector_test.end(), false);

    if (found != bvector_test.end()) {
        std::cout << "Found" << std::endl;
    } else {
        std::cout << "Not found" << std::endl;
    }

    stdlike::Vector<int> vector_test(10, 9);
    vector_test.PushBack(10);
    auto found2 = std::find(vector_test.begin(), vector_test.end(), 10);

    if (found2 != vector_test.end()) {
        std::cout << "Found" << std::endl;
    } else {
        std::cout << "Not found" << std::endl;
    }

    return 0;
}
