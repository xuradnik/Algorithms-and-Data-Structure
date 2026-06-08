#include "array.test.hpp"

#include <libds/adt/array.hpp>


namespace ds::tests {


ArrayTests::ArrayTests() : stest::CompositeTest("Array") {

this->add_test("clear", [](stest::LeafTest &s){
    adt::Array<int> a(4);
    s.assert_throws([&](){ a.clear(); }, "Clear throws.");
});

this->add_test("isEmpty", [](stest::LeafTest &s){
    adt::Array<int> a(4);
    s.assert_false(a.isEmpty(), "Array is not empty.");
});

this->add_test("construct_size", [](stest::LeafTest &s){
    adt::Array<int> a(4);
    s.assert_equals(size_t{4}, a.size(), "Size is 4.");
});

this->add_test("construct_dimension", [](stest::LeafTest &s){
    adt::Array<int> a(adt::Dimension{-5, 4});
    s.assert_equals(size_t{4}, a.size(), "Size is 4.");
    s.assert_equals(-5LL, a.getBase(), "Base is -5.");
});

this->add_test("access_zeroBaseValidLeft", [](stest::LeafTest &s){
    adt::Array<int> a(4);
    int &elem = a.access(0);
    elem = 1;
    s.assert_equals(1, a.access(0), "Element is set correctly.");
});

this->add_test("access_zeroBaseValidMiddle", [](stest::LeafTest &s){
    adt::Array<int> a(4);
    int &elem = a.access(1);
    elem = 1;
    s.assert_equals(1, a.access(1), "Element is set correctly.");
});

this->add_test("access_zeroBaseValidRight", [](stest::LeafTest &s){
    adt::Array<int> a(4);
    int &elem = a.access(3);
    elem = 1;
    s.assert_equals(1, a.access(3), "Element is set correctly.");
});

this->add_test("access_zeroBaseInvalidRight", [](stest::LeafTest &s){
    adt::Array<int> a(4);
    s.assert_throws([&](){ a.access(4); }, "Throws on invalid index.");
});

this->add_test("access_zeroBaseInvalidLeft", [](stest::LeafTest &s){
    adt::Array<int> a(4);
    s.assert_throws([&](){ a.access(-1); }, "Throws on invalid index.");
});

this->add_test("access_positiveBaseValidLeft", [](stest::LeafTest &s){
    adt::Array<int> a(adt::Dimension{5, 4});
    int &elem = a.access(5);
    elem = 1;
    s.assert_equals(1, a.access(5), "Element is set correctly.");
});

this->add_test("access_positiveBaseValidMiddle", [](stest::LeafTest &s){
    adt::Array<int> a(adt::Dimension{5, 4});
    int &elem = a.access(7);
    elem = 1;
    s.assert_equals(1, a.access(7), "Element is set correctly.");
});

this->add_test("access_positiveBaseValidRight", [](stest::LeafTest &s){
    adt::Array<int> a(adt::Dimension{5, 4});
    int &elem = a.access(8);
    elem = 1;
    s.assert_equals(1, a.access(8), "Element is set correctly.");
});

this->add_test("access_positiveBaseInvalidRight", [](stest::LeafTest &s){
    adt::Array<int> a(adt::Dimension{5, 4});
    s.assert_throws([&](){ a.access(9); }, "Throws on invalid index.");
});

this->add_test("access_positiveBaseInvalidLeft", [](stest::LeafTest &s){
    adt::Array<int> a(adt::Dimension{5, 4});
    s.assert_throws([&](){ a.access(4); }, "Throws on invalid index.");
});

this->add_test("access_negativeBaseValidLeft", [](stest::LeafTest &s){
    adt::Array<int> a(adt::Dimension{-5, 4});
    int &elem = a.access(-5);
    elem = 1;
    s.assert_equals(1, a.access(-5), "Element is set correctly.");
});

this->add_test("access_negativeBaseValidMiddle", [](stest::LeafTest &s){
    adt::Array<int> a(adt::Dimension{-5, 4});
    int &elem = a.access(-3);
    elem = 1;
    s.assert_equals(1, a.access(-3), "Element is set correctly.");
});

this->add_test("access_negativeBaseValidRight", [](stest::LeafTest &s){
    adt::Array<int> a(adt::Dimension{-5, 4});
    int &elem = a.access(-2);
    elem = 1;
    s.assert_equals(1, a.access(-2), "Element is set correctly.");
});

this->add_test("access_negativeBaseInvalidRight", [](stest::LeafTest &s){
    adt::Array<int> a(adt::Dimension{-5, 4});
    s.assert_throws([&](){ a.access(0); }, "Throws on invalid index.");
});

this->add_test("access_negativeBaseInvalidLeft", [](stest::LeafTest &s){
    adt::Array<int> a(adt::Dimension{-5, 4});
    s.assert_throws([&](){ a.access(-6); }, "Throws on invalid index.");
});

this->add_test("assign_differentDimensions", [](stest::LeafTest &s){
    adt::Array<int> a1(adt::Dimension{5, 4});
    adt::Array<int> a2(adt::Dimension{0, 4});
    s.assert_throws([&](){ a1.assign(a2); });
});

this->add_test("assign_sameDimensions", [](stest::LeafTest &s){
    adt::Array<int> a1(adt::Dimension{0, 4});
    adt::Array<int> a2(adt::Dimension{0, 4});
    a1.access(0) = 0;
    a1.access(1) = 1;
    a1.access(2) = 2;
    a1.access(3) = 3;
    a2.assign(a1);
    s.assert_equals(a1.access(0), a2.access(0), "First elements are equal.");
    s.assert_equals(a1.access(1), a2.access(1), "Second elements are equal.");
    s.assert_equals(a1.access(2), a2.access(2), "Third elements are equal.");
    s.assert_equals(a1.access(3), a2.access(3), "Fourth elements are equal.");
    a1.access(0) = 100;
    s.assert_equals(a2.access(0), 0, "Copy is independent.");
});

this->add_test("equals_differentDimensionsEqual", [](stest::LeafTest &s){
    adt::Array<int> a1(adt::Dimension{1, 4});
    adt::Array<int> a2(adt::Dimension{0, 4});
    a1.access(1) = 0;
    a1.access(2) = 1;
    a1.access(3) = 2;
    a1.access(4) = 3;
    a2.access(0) = 0;
    a2.access(1) = 1;
    a2.access(2) = 2;
    a2.access(3) = 3;
    s.assert_false(a1.equals(a2), "Arrays differ in dimensions.");
});

this->add_test("equals_sameDimensionsEqual", [](stest::LeafTest &s){
    adt::Array<int> a1(adt::Dimension{0, 4});
    adt::Array<int> a2(adt::Dimension{0, 4});
    a1.access(0) = 0;
    a1.access(1) = 1;
    a1.access(2) = 2;
    a1.access(3) = 3;
    a2.access(0) = 0;
    a2.access(1) = 1;
    a2.access(2) = 2;
    a2.access(3) = 3;
    s.assert_true(a1.equals(a2), "Arrays are equal.");
});

this->add_test("equals_sameDimensionsNotEqual", [](stest::LeafTest &s){
    adt::Array<int> a1(adt::Dimension{0, 4});
    adt::Array<int> a2(adt::Dimension{0, 4});
    a1.access(0) = 0;
    a1.access(1) = 10;
    a1.access(2) = 20;
    a1.access(3) = 30;
    a2.access(0) = 0;
    a2.access(1) = 1;
    a2.access(2) = 2;
    a2.access(3) = 3;
    s.assert_false(a1.equals(a2), "Arrays are equal.");
});

this->add_test("iterators", [](stest::LeafTest &s){
    adt::Array<int> a(adt::Dimension{-5, 3});
    a.access(-5) = 1;
    a.access(-4) = 2;
    a.access(-3) = 3;
    auto begin = a.begin();
    auto end = a.end();
    const int first = *begin++;
    const int second = *begin++;
    const int third = *begin++;
    s.assert_true(begin == end, "All elements were accessed.");
    s.assert_equals(1, first, "First element is 1.");
    s.assert_equals(2, second, "First element is 2.");
    s.assert_equals(3, third, "First element is 3.");
});

} // ArrayTests::ArrayTests


CompactMatrixTests::CompactMatrixTests() :
    stest::CompositeTest("CompactMatrix")
{

this->add_test("clear", [](stest::LeafTest &s){
    adt::CompactMatrix<int> m(3, 4);
    s.assert_throws([&](){ m.clear(); }, "Clear throws.");
});

this->add_test("isEmpty", [](stest::LeafTest &s){
    adt::CompactMatrix<int> m(3, 4);
    s.assert_false(m.isEmpty(), "Matrix is not empty.");
});

this->add_test("construct_size", [](stest::LeafTest &s){
    adt::CompactMatrix<int> m(3, 4);
    s.assert_equals(size_t{12}, m.size(), "Size is 3*4=12.");
});

this->add_test("construct_dimensions", [](stest::LeafTest &s){
    adt::CompactMatrix<int> m(adt::Dimension{-5, 3}, adt::Dimension{-3, 4});
    s.assert_equals(size_t{12}, m.size(), "Size is 3*4=12.");
    s.assert_equals(-5LL, m.getDimension1().base, "Row base is -5.");
    s.assert_equals(size_t{3}, m.getDimension1().size, "Row count is 3.");
    s.assert_equals(-3LL, m.getDimension2().base, "Col base is -3.");
    s.assert_equals(size_t{4}, m.getDimension2().size, "Col count is 4.");
});

this->add_test("access_zeroBasesValidTopLeft", [](stest::LeafTest &s){
    adt::CompactMatrix<int> m(3, 4);
    m.access(0, 0) = 1;
    s.assert_equals(1, m.access(0, 0), "Element is set correctly.");
});

this->add_test("access_zeroBasesValidMiddle", [](stest::LeafTest &s){
    adt::CompactMatrix<int> m(3, 4);
    m.access(1, 2) = 1;
    s.assert_equals(1, m.access(1, 2), "Element is set correctly.");
});

this->add_test("access_zeroBasesValidBottomRight", [](stest::LeafTest &s){
    adt::CompactMatrix<int> m(3, 4);
    m.access(2, 3) = 1;
    s.assert_equals(1, m.access(2, 3), "Element is set correctly.");
});

this->add_test("access_zeroBasesInvalidRowHigh", [](stest::LeafTest &s){
    adt::CompactMatrix<int> m(3, 4);
    s.assert_throws([&](){ m.access(3, 0); }, "Throws on invalid row index.");
});

this->add_test("access_zeroBasesInvalidRowLow", [](stest::LeafTest &s){
    adt::CompactMatrix<int> m(3, 4);
    s.assert_throws([&](){ m.access(-1, 0); }, "Throws on invalid row index.");
});

this->add_test("access_zeroBasesInvalidColHigh", [](stest::LeafTest &s){
    adt::CompactMatrix<int> m(3, 4);
    s.assert_throws([&](){ m.access(0, 4); }, "Throws on invalid col index.");
});

this->add_test("access_zeroBasesInvalidColLow", [](stest::LeafTest &s){
    adt::CompactMatrix<int> m(3, 4);
    s.assert_throws([&](){ m.access(0, -1); }, "Throws on invalid col index.");
});

this->add_test("access_positiveBasesValidTopLeft", [](stest::LeafTest &s){
    adt::CompactMatrix<int> m(adt::Dimension{5, 3}, adt::Dimension{10, 4});
    m.access(5, 10) = 1;
    s.assert_equals(1, m.access(5, 10), "Element is set correctly.");
});

this->add_test("access_positiveBasesValidMiddle", [](stest::LeafTest &s){
    adt::CompactMatrix<int> m(adt::Dimension{5, 3}, adt::Dimension{10, 4});
    m.access(6, 12) = 1;
    s.assert_equals(1, m.access(6, 12), "Element is set correctly.");
});

this->add_test("access_positiveBasesValidBottomRight", [](stest::LeafTest &s){
    adt::CompactMatrix<int> m(adt::Dimension{5, 3}, adt::Dimension{10, 4});
    m.access(7, 13) = 1;
    s.assert_equals(1, m.access(7, 13), "Element is set correctly.");
});

this->add_test("access_positiveBasesInvalidRowHigh", [](stest::LeafTest &s){
    adt::CompactMatrix<int> m(adt::Dimension{5, 3}, adt::Dimension{10, 4});
    s.assert_throws([&](){ m.access(8, 10); }, "Throws on invalid row index.");
});

this->add_test("access_positiveBasesInvalidRowLow", [](stest::LeafTest &s){
    adt::CompactMatrix<int> m(adt::Dimension{5, 3}, adt::Dimension{10, 4});
    s.assert_throws([&](){ m.access(4, 10); }, "Throws on invalid row index.");
});

this->add_test("access_positiveBasesInvalidColHigh", [](stest::LeafTest &s){
    adt::CompactMatrix<int> m(adt::Dimension{5, 3}, adt::Dimension{10, 4});
    s.assert_throws([&](){ m.access(5, 14); }, "Throws on invalid col index.");
});

this->add_test("access_positiveBasesInvalidColLow", [](stest::LeafTest &s){
    adt::CompactMatrix<int> m(adt::Dimension{5, 3}, adt::Dimension{10, 4});
    s.assert_throws([&](){ m.access(5, 9); }, "Throws on invalid col index.");
});

this->add_test("access_negativeBasesValidTopLeft", [](stest::LeafTest &s){
    adt::CompactMatrix<int> m(adt::Dimension{-5, 3}, adt::Dimension{-3, 4});
    m.access(-5, -3) = 1;
    s.assert_equals(1, m.access(-5, -3), "Element is set correctly.");
});

this->add_test("access_negativeBasesValidMiddle", [](stest::LeafTest &s){
    adt::CompactMatrix<int> m(adt::Dimension{-5, 3}, adt::Dimension{-3, 4});
    m.access(-4, -1) = 1;
    s.assert_equals(1, m.access(-4, -1), "Element is set correctly.");
});

this->add_test("access_negativeBasesValidBottomRight", [](stest::LeafTest &s){
    adt::CompactMatrix<int> m(adt::Dimension{-5, 3}, adt::Dimension{-3, 4});
    m.access(-3, 0) = 1;
    s.assert_equals(1, m.access(-3, 0), "Element is set correctly.");
});

this->add_test("access_negativeBasesInvalidRowHigh", [](stest::LeafTest &s){
    adt::CompactMatrix<int> m(adt::Dimension{-5, 3}, adt::Dimension{-3, 4});
    s.assert_throws([&](){ m.access(-2, -3); }, "Throws on invalid row index.");
});

this->add_test("access_negativeBasesInvalidRowLow", [](stest::LeafTest &s){
    adt::CompactMatrix<int> m(adt::Dimension{-5, 3}, adt::Dimension{-3, 4});
    s.assert_throws([&](){ m.access(-6, -3); }, "Throws on invalid row index.");
});

this->add_test("access_negativeBasesInvalidColHigh", [](stest::LeafTest &s){
    adt::CompactMatrix<int> m(adt::Dimension{-5, 3}, adt::Dimension{-3, 4});
    s.assert_throws([&](){ m.access(-5, 1); }, "Throws on invalid col index.");
});

this->add_test("access_negativeBasesInvalidColLow", [](stest::LeafTest &s){
    adt::CompactMatrix<int> m(adt::Dimension{-5, 3}, adt::Dimension{-3, 4});
    s.assert_throws([&](){ m.access(-5, -4); }, "Throws on invalid col index.");
});

this->add_test("assign_differentDimensions", [](stest::LeafTest &s){
    adt::CompactMatrix<int> m1(adt::Dimension{5, 3}, adt::Dimension{0, 4});
    adt::CompactMatrix<int> m2(adt::Dimension{0, 3}, adt::Dimension{0, 4});
    s.assert_throws([&](){ m1.assign(m2); }, "Throws on dimension mismatch.");
});

this->add_test("assign_sameDimensions", [](stest::LeafTest &s){
    adt::CompactMatrix<int> m1(adt::Dimension{0, 2}, adt::Dimension{0, 2});
    adt::CompactMatrix<int> m2(adt::Dimension{0, 2}, adt::Dimension{0, 2});
    m1.access(0, 0) = 0;
    m1.access(0, 1) = 1;
    m1.access(1, 0) = 2;
    m1.access(1, 1) = 3;
    m2.assign(m1);
    s.assert_equals(m1.access(0, 0), m2.access(0, 0), "Element (0,0) is equal.");
    s.assert_equals(m1.access(0, 1), m2.access(0, 1), "Element (0,1) is equal.");
    s.assert_equals(m1.access(1, 0), m2.access(1, 0), "Element (1,0) is equal.");
    s.assert_equals(m1.access(1, 1), m2.access(1, 1), "Element (1,1) is equal.");
    m1.access(0, 0) = 100;
    s.assert_equals(m2.access(0, 0), 0, "Copy is independent.");
});

this->add_test("equals_differentDim1Equal", [](stest::LeafTest &s){
    adt::CompactMatrix<int> m1(adt::Dimension{1, 2}, adt::Dimension{0, 2});
    adt::CompactMatrix<int> m2(adt::Dimension{0, 2}, adt::Dimension{0, 2});
    m1.access(1, 0) = 0; m1.access(1, 1) = 1;
    m1.access(2, 0) = 2; m1.access(2, 1) = 3;
    m2.access(0, 0) = 0; m2.access(0, 1) = 1;
    m2.access(1, 0) = 2; m2.access(1, 1) = 3;
    s.assert_false(m1.equals(m2), "Matrices differ in row dimension.");
});

this->add_test("equals_differentDim2Equal", [](stest::LeafTest &s){
    adt::CompactMatrix<int> m1(adt::Dimension{0, 2}, adt::Dimension{1, 2});
    adt::CompactMatrix<int> m2(adt::Dimension{0, 2}, adt::Dimension{0, 2});
    m1.access(0, 1) = 0; m1.access(0, 2) = 1;
    m1.access(1, 1) = 2; m1.access(1, 2) = 3;
    m2.access(0, 0) = 0; m2.access(0, 1) = 1;
    m2.access(1, 0) = 2; m2.access(1, 1) = 3;
    s.assert_false(m1.equals(m2), "Matrices differ in col dimension.");
});

this->add_test("equals_sameDimensionsEqual", [](stest::LeafTest &s){
    adt::CompactMatrix<int> m1(adt::Dimension{0, 2}, adt::Dimension{0, 2});
    adt::CompactMatrix<int> m2(adt::Dimension{0, 2}, adt::Dimension{0, 2});
    m1.access(0, 0) = 0; m1.access(0, 1) = 1;
    m1.access(1, 0) = 2; m1.access(1, 1) = 3;
    m2.access(0, 0) = 0; m2.access(0, 1) = 1;
    m2.access(1, 0) = 2; m2.access(1, 1) = 3;
    s.assert_true(m1.equals(m2), "Matrices are equal.");
});

this->add_test("equals_sameDimensionsNotEqual", [](stest::LeafTest &s){
    adt::CompactMatrix<int> m1(adt::Dimension{0, 2}, adt::Dimension{0, 2});
    adt::CompactMatrix<int> m2(adt::Dimension{0, 2}, adt::Dimension{0, 2});
    m1.access(0, 0) = 0; m1.access(0, 1) = 10;
    m1.access(1, 0) = 20; m1.access(1, 1) = 30;
    m2.access(0, 0) = 0; m2.access(0, 1) = 1;
    m2.access(1, 0) = 2; m2.access(1, 1) = 3;
    s.assert_false(m1.equals(m2), "Matrices are not equal.");
});

} // CompactMatrixTests::CompactMatrixTests


} // namespace ds::tests
