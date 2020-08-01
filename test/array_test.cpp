#include "array_test.h"

#include "rlib.h"

namespace RLibTest
{
    void ArrayTest::SetUp()
    {
    }
    
    void ArrayTest::TearDown()
    {
    }

    TEST_F(ArrayTest, TestInit)
    {
        Array a;
        Array* ap = &a;
        ARR_INIT(ap, int);
        ASSERT_TRUE(ap->data != nullptr);
        ASSERT_EQ(ap->capacity, 8);
        ASSERT_EQ(ap->count, 0);
        ASSERT_EQ(ap->elemSize, sizeof(int));
    }
} // namespace RLibTest
