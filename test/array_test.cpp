#include "array_test.h"

#include "rlib.h"

#define TEST_COUNT 10000

namespace RLibTest
{
    namespace RLibArrayTest
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

            array_free(ap);
        }

        TEST_F(ArrayTest, TestFree)
        {
            Array a;
            Array* ap = &a;
            ARR_INIT(ap, int);
            array_free(ap);
            ASSERT_TRUE(ap->data == nullptr);
            ASSERT_EQ(ap->capacity, 0);
            ASSERT_EQ(ap->count, 0);
            ASSERT_EQ(ap->elemSize, 0);
        }

        TEST_F(ArrayTest, TestBaseType)
        {
            Array a;
            Array* ap = &a;
            ARR_INIT(ap, int);

            ARR_PUSH(ap, int, 5);
            ARR_PUSH(ap, int, 10);
            ARR_PUSH(ap, int, 15);
            ASSERT_EQ(ap->count, 3);

            int ret;
            ASSERT_TRUE(array_pop(ap, &ret));
            ASSERT_EQ(ret, 15);
            ASSERT_EQ(ap->count, 2);

            ARR_SET(ap, int, 0, 25);
            ASSERT_EQ(ap->count, 2);
            ASSERT_TRUE(array_get(ap, 0, &ret));
            ASSERT_EQ(ret, 25);

            ASSERT_TRUE(array_remove(ap, 0));
            ASSERT_EQ(ap->count, 1);
            ASSERT_TRUE(array_get(ap, 0, &ret));
            ASSERT_EQ(ret, 10);

            array_free(ap);
        }

        TEST_F(ArrayTest, TestPointerType)
        {
            Array a;
            Array* ap = &a;
            ARR_INIT(ap, B);

            B b1;
            b1.a.f = 100.0;
            b1.i = 100;
            b1.b = new B();
            b1.b->i = 1100;

            B b2;
            b2.a.f = 200.0;
            b2.i = 200;
            b2.b = new B();
            b2.b->i = 2100;

            B* b3 = new B();
            b3->a.f = 300.0;
            b3->i = 300;
            b3->b = new B();
            b3->b->i = 3100;

            array_push(ap, &b1);
            array_push(ap, &b2);
            array_push(ap, b3);
            ASSERT_EQ(ap->count, 3);

            B b4;
            ASSERT_TRUE(array_pop(ap, &b4));
            ASSERT_EQ(b4.a.f, 300.0);
            ASSERT_EQ(b4.i, 300);
            ASSERT_EQ(b4.b->i, 3100);

            ASSERT_TRUE(array_set(ap, 0, &b4));
            B b5;
            ASSERT_TRUE(array_get(ap, 0, &b5));
            ASSERT_EQ(b5.a.f, 300.0);
            ASSERT_EQ(b5.i, 300);
            ASSERT_EQ(b5.b->i, 3100);

            ASSERT_TRUE(array_remove(ap, 1));
            ASSERT_EQ(ap->count, 1);

            array_free(ap);
        }

        TEST_F(ArrayTest, PushTest)
        {
            Array a;
            Array* ap = &a;
            ARR_INIT(ap, int);

            for (size_t i = 0; i < TEST_COUNT; i++)
            {
                ARR_PUSH(ap, int, 5);
            }

            ASSERT_EQ(ap->count, TEST_COUNT);

            int ret;
            ASSERT_TRUE(array_get(ap, 50, &ret));
            ASSERT_EQ(ret, 5);

            array_free(ap);
        }

        TEST_F(ArrayTest, PopTest)
        {
            Array a;
            Array* ap = &a;
            ARR_INIT(ap, int);

            for (size_t i = 0; i < TEST_COUNT; i++)
            {
                ARR_PUSH(ap, int, 5);
            }

            ASSERT_EQ(ap->count, TEST_COUNT);

            for (size_t i = 0; i < TEST_COUNT; i++)
            {
                ASSERT_TRUE(array_pop(ap, nullptr));
            }

            ASSERT_EQ(ap->count, 0);
            ASSERT_FALSE(array_pop(ap, nullptr));

            array_free(ap);
        }

        TEST_F(ArrayTest, TestSetGet)
        {
            Array a;
            Array* ap = &a;
            ARR_INIT(ap, int);

            for (size_t i = 0; i < TEST_COUNT; i++)
            {
                ARR_PUSH(ap, int, 1);
            }

            ASSERT_EQ(ap->count, TEST_COUNT);

            for (size_t i = 0; i < TEST_COUNT; i++)
            {
                ASSERT_TRUE(array_set(ap, i, &i));
            }

            for (size_t i = 0; i < TEST_COUNT; i++)
            {
                int ret;
                ASSERT_TRUE(array_get(ap, i, &ret));
                ASSERT_EQ(ret, i);
            }

            int temp = 0;
            ASSERT_FALSE(array_set(ap, TEST_COUNT + 1, &temp));
            ASSERT_FALSE(array_set(ap, -1, &temp));
            ASSERT_FALSE(array_get(ap, TEST_COUNT + 1, &temp));
            ASSERT_FALSE(array_get(ap, -1, &temp));

            array_free(ap);
        }

        TEST_F(ArrayTest, TestRemove)
        {
            Array a;
            Array* ap = &a;
            ARR_INIT(ap, int);

            for (size_t i = 0; i < TEST_COUNT; i++)
            {
                ARR_PUSH(ap, int, i);
            }

            ASSERT_EQ(ap->count, TEST_COUNT);

            for (size_t i = 0; i < TEST_COUNT - 1; i++)
            {
                ASSERT_TRUE(array_remove(ap, 0));
                int ret;
                ASSERT_TRUE(array_get(ap, 0, &ret));
                ASSERT_EQ(ret, i + 1);
            }

            ASSERT_EQ(ap->count, 1);
            ASSERT_TRUE(array_remove(ap, 0));
            ASSERT_EQ(ap->count, 0);

            array_free(ap);
        }

        TEST_F(ArrayTest, TestRemoveRange)
        {
            Array a;
            Array* ap = &a;
            ARR_INIT(ap, int);

            int ret;

            for (size_t i = 0; i < TEST_COUNT; i++)
            {
                ARR_PUSH(ap, int, i);
            }

            ASSERT_EQ(ap->count, TEST_COUNT);

            ASSERT_TRUE(array_removeRange(ap, TEST_COUNT / 2, TEST_COUNT / 2));

            ASSERT_EQ(ap->count, TEST_COUNT / 2);
            ASSERT_FALSE(array_get(ap, TEST_COUNT / 2, &ret));
            ASSERT_TRUE(array_get(ap, TEST_COUNT / 2 - 1, &ret));
            ASSERT_EQ(ret, TEST_COUNT / 2 - 1);

            ASSERT_FALSE(array_removeRange(ap, TEST_COUNT / 2, 10));

            ASSERT_TRUE(array_removeRange(ap, 0, 10));
            ASSERT_EQ(ap->count, TEST_COUNT / 2 - 10);

            ASSERT_TRUE(array_get(ap, 0, &ret));
            ASSERT_EQ(10, ret);

            ASSERT_TRUE(array_removeRange(ap, 100, 20));
            ASSERT_EQ(ap->count, TEST_COUNT / 2 - 30);

            ASSERT_TRUE(array_get(ap, 100, &ret));
            ASSERT_EQ(130, ret);

            array_free(ap);
        }

        TEST_F(ArrayTest, TestClear)
        {
            Array a;
            Array* ap = &a;
            ARR_INIT(ap, int);

            for (size_t i = 0; i < TEST_COUNT; i++)
            {
                ARR_PUSH(ap, int, i);
            }

            ASSERT_EQ(ap->count, TEST_COUNT);

            array_clear(ap);
            ASSERT_EQ(ap->count, 0);

            array_free(ap);
        }

        TEST_F(ArrayTest, TestEq)
        {
            Array a;
            Array* ap = &a;
            ARR_INIT(ap, int);

            Array b;
            Array* bp = &b;
            ARR_INIT(bp, int);

            Array c;
            Array* cp = &c;
            ARR_INIT(cp, double);

            ASSERT_FALSE(array_eq(NULL, NULL));
            ASSERT_FALSE(array_eq(ap, NULL));
            ASSERT_TRUE(array_eq(ap, ap));
            ASSERT_TRUE(array_eq(ap, bp));
            ASSERT_FALSE(array_eq(ap, cp));

            ARR_PUSH(ap, int, 5);
            ASSERT_FALSE(array_eq(ap, bp));
            ARR_PUSH(bp, int, 5);
            ASSERT_TRUE(array_eq(ap, bp));

            for (size_t i = 0; i < TEST_COUNT; i++)
            {
                ARR_PUSH(ap, int, 5);
            }

            for (size_t i = 0; i < TEST_COUNT; i++)
            {
                ARR_PUSH(bp, int, 5);
            }
            ASSERT_TRUE(array_eq(ap, bp));

            array_free(ap);
        }

        TEST_F(ArrayTest, TestIndexOf)
        {
            Array a;
            Array* ap = &a;
            ARR_INIT(ap, int);

            int ret;

            for (size_t i = 0; i < TEST_COUNT; i++)
            {
                ARR_PUSH(ap, int, i % 100);
            }

            ASSERT_EQ(ap->count, TEST_COUNT);

            int toFindNumber = 1;
            ASSERT_EQ(1, array_indexOf(ap, &toFindNumber, sizeof(int)));

            toFindNumber = 99;
            ASSERT_EQ(99, array_indexOf(ap, &toFindNumber, sizeof(int)));

            toFindNumber = 100;
            ASSERT_EQ(-1, array_indexOf(ap, &toFindNumber, sizeof(int)));

            array_free(ap);
        }

        TEST_F(ArrayTest, TestLastIndexOf)
        {
            Array a;
            Array* ap = &a;
            ARR_INIT(ap, int);

            int ret;

            for (size_t i = 0; i < TEST_COUNT; i++)
            {
                ARR_PUSH(ap, int, i % 100);
            }

            ASSERT_EQ(ap->count, TEST_COUNT);

            int toFindNumber = 1;
            ASSERT_EQ(TEST_COUNT - 99, array_lastIndexOf(ap, &toFindNumber, sizeof(int)));

            toFindNumber = 99;
            ASSERT_EQ(TEST_COUNT - 1, array_lastIndexOf(ap, &toFindNumber, sizeof(int)));

            toFindNumber = 100;
            ASSERT_EQ(-1, array_lastIndexOf(ap, &toFindNumber, sizeof(int)));

            array_free(ap);
        }

        TEST_F(ArrayTest, TestContains)
        {
            Array a;
            Array* ap = &a;
            ARR_INIT(ap, int);

            int ret;

            for (size_t i = 0; i < TEST_COUNT; i++)
            {
                ARR_PUSH(ap, int, i % 100);
            }

            ASSERT_EQ(ap->count, TEST_COUNT);

            int toFindNumber = 1;
            ASSERT_TRUE(array_contains(ap, &toFindNumber, sizeof(int)));

            toFindNumber = 99;
            ASSERT_TRUE(array_contains(ap, &toFindNumber, sizeof(int)));

            toFindNumber = 100;
            ASSERT_FALSE(array_contains(ap, &toFindNumber, sizeof(int)));

            array_free(ap);
        }

        TEST_F(ArrayTest, TestInsert)
        {
            Array a;
            Array* ap = &a;
            ARR_INIT(ap, int);

            int ret;

            for (size_t i = 0; i < TEST_COUNT; i++)
            {
                ARR_PUSH(ap, int, i % 100);
            }

            ASSERT_EQ(ap->count, TEST_COUNT);

            int value = 9920;
            ASSERT_TRUE(array_insert(ap, 0, &value));
            ASSERT_EQ(ap->count, TEST_COUNT + 1);
            ASSERT_TRUE(array_get(ap, 0, &ret));
            ASSERT_EQ(9920, ret);

            value = 123;
            ASSERT_TRUE(array_insert(ap, TEST_COUNT + 1, &value));
            ASSERT_EQ(ap->count, TEST_COUNT + 2);
            ASSERT_TRUE(array_get(ap, TEST_COUNT + 1, &ret));
            ASSERT_EQ(123, ret);

            value = 2340;
            ASSERT_TRUE(array_insert(ap, TEST_COUNT  / 2 + 20, &value));
            ASSERT_EQ(ap->count, TEST_COUNT + 3);
            ASSERT_TRUE(array_get(ap, TEST_COUNT  / 2 + 20, &ret));
            ASSERT_EQ(2340, ret);

            ASSERT_FALSE(array_insert(ap, -1, &value));
            ASSERT_EQ(ap->count, TEST_COUNT + 3);
            ASSERT_FALSE(array_insert(ap, TEST_COUNT + 4, &value));
            ASSERT_EQ(ap->count, TEST_COUNT + 3);
        }

        TEST_F(ArrayTest, TestSetCap)
        {
            Array a;
            Array* ap = &a;
            ARR_INIT(ap, int);
            ASSERT_TRUE(ap->data != nullptr);
            ASSERT_EQ(ap->capacity, 8);
            ASSERT_EQ(ap->count, 0);
            ASSERT_EQ(ap->elemSize, sizeof(int));

            ASSERT_FALSE(array_setCapacity(ap, 7));
            ASSERT_EQ(ap->capacity, 8);

            ASSERT_TRUE(array_setCapacity(ap, 100));
            ASSERT_EQ(ap->capacity, 100);

            array_free(ap);
        }

        TEST_F(ArrayTest, TestClone)
        {
            Array a;
            Array* ap = &a;
            ARR_INIT(ap, int);

            int ret;

            for (size_t i = 0; i < TEST_COUNT; i++)
            {
                ARR_PUSH(ap, int, i);
                ASSERT_TRUE(array_get(ap, i, &ret));
                ASSERT_EQ(ret, i);
            }

            ASSERT_EQ(ap->count, TEST_COUNT);

            Array b;
            Array* bp = &b;
            ARR_INIT_CAP(bp, int, 0);
            array_clone(ap, bp);

            ASSERT_EQ(bp->count, TEST_COUNT);
            for (size_t i = 0; i < TEST_COUNT; i++)
            {
                ASSERT_TRUE(array_get(bp, i, &ret));
                ASSERT_EQ(ret, i);
            }
        }
    }
} // namespace RLibTest
