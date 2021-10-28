#include "pch.h"
#include "matrix.h"


// comaring matrices with each other in order to find them equal
template<typename T>
::testing::AssertionResult MatrixMatch(const Matrix<T>& expected,
	const Matrix<T>& actual) {
	for (size_t i = 0; i < expected.rows(); i++)
	{
		for (size_t j = 0; j < expected.columns(); j++)
		{
			if (expected(i, j) != actual(i, j))
			{
				return ::testing::AssertionFailure() << "actual (" << i
					<< ", " << j << actual(i, j) << ") != expected(" << i
					<< ", " << j << expected(i, j) << ")";
			}
		}
	}

	return ::testing::AssertionSuccess();
}

template <typename T>
class MatrixTest : public testing::Test {
};

using testing::Types;

typedef Types<int, float, double> Implementations;
TYPED_TEST_CASE(MatrixTest, Implementations);

TYPED_TEST(MatrixTest, MatrixConstructor) {
	ASSERT_NO_THROW(Matrix<TypeParam>(2, 3));
	ASSERT_NO_THROW(Matrix<TypeParam>(2, 4));
	ASSERT_NO_THROW(Matrix<TypeParam>(3, 2));
	ASSERT_NO_THROW(Matrix<TypeParam>(3, 3));
	ASSERT_NO_THROW(Matrix<TypeParam>(2, 3, { 1, 2, 3, 4, 5, 6 }));
}

TYPED_TEST(MatrixTest, MatrixAddition) {

	Matrix<TypeParam> m1(2, 3, { 1, 2, 3, 4, 5, 6 });
	Matrix<TypeParam> m2(2, 3, { 6, 5, 4, 3, 2, 1 });
	Matrix<TypeParam> matrix_addition(2, 3, { 7, 7, 7, 7, 7, 7 });
	
	ASSERT_NO_THROW(m1 + m2);

	EXPECT_TRUE(MatrixMatch(matrix_addition, m1+m2));
}

TYPED_TEST(MatrixTest, MatrixProduct) {

	Matrix<TypeParam> m1(2, 3, { 1, 2, 3, 4, 5, 6 });
	Matrix<TypeParam> m2(3, 2, { 6, 5, 4, 3, 2, 1 });
	Matrix<TypeParam> matrix_product(2, 2, { 20, 14, 56, 41 });

	ASSERT_NO_THROW(m1 * m2);

	EXPECT_TRUE(MatrixMatch(matrix_product, m1 * m2));
}

TYPED_TEST(MatrixTest, MatrixInverse) {

	Matrix<TypeParam> m1(3, 3, { 1, 0, 5, 2, 1, 6, 3, 4, 0 });
	Matrix<TypeParam> matrix_inverse(3, 3, { -24, 20, -5, 18, -15, 4, 5, -4, 1 });

	ASSERT_NO_THROW(m1.inverse());

	EXPECT_TRUE(MatrixMatch(matrix_inverse, m1.inverse()));
}

TYPED_TEST(MatrixTest, MatrixTranspose) {

	Matrix<TypeParam> m1(2, 3, { 1, 2, 3, 4, 5, 6 });
	Matrix<TypeParam> matrix_transpose(3, 2, { 1, 4, 2, 5, 3, 6 });

	ASSERT_NO_THROW(m1.transpose());

	EXPECT_TRUE(MatrixMatch(matrix_transpose, m1.transpose()));
}

TYPED_TEST(MatrixTest, MatrixAddRowEmpty) {

	Matrix<TypeParam> m(2, 3, { 1, 2, 3, 4, 5, 6 });
	Matrix<TypeParam> matrix_addRow(3, 3, { 1, 2, 3, 0, 0, 0, 4, 5, 6 });

	ASSERT_NO_THROW(m.addRow(0));

	EXPECT_TRUE(MatrixMatch(matrix_addRow, m));
}

TYPED_TEST(MatrixTest, MatrixAddRowValues) {

	Matrix<TypeParam> m(2, 3, { 1, 2, 3, 4, 5, 6 });
	Matrix<TypeParam> matrix_addRow(3, 3, { 1, 2, 3, 7, 8, 9, 4, 5, 6 });

	ASSERT_NO_THROW(m.addRow(0, { 7, 8, 9 }));

	EXPECT_TRUE(MatrixMatch(matrix_addRow, m));
}

TYPED_TEST(MatrixTest, MatrixAddColumnEmpty) {

	Matrix<TypeParam> m(2, 3, { 1, 2, 3, 4, 5, 6 });
	Matrix<TypeParam> matrix_addColumn(2, 4, { 1, 0, 2, 3, 4, 0, 5, 6 });

	ASSERT_NO_THROW(m.addColumn(0));

	EXPECT_TRUE(MatrixMatch(matrix_addColumn, m));

}
TYPED_TEST(MatrixTest, MatrixAddColumnValues) {

	Matrix<TypeParam> m(2, 3, { 1, 2, 3, 4, 5, 6 });
	Matrix<TypeParam> matrix_addColumn(2, 4, { 1, 7, 2, 3, 4, 8, 5, 6 });

	ASSERT_NO_THROW(m.addColumn(0, { 7,8 }));

	EXPECT_TRUE(MatrixMatch(matrix_addColumn, m));

}

int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
