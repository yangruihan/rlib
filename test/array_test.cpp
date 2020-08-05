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
    }
} // namespace RLibTest
