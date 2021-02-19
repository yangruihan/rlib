#include "stack_test.h"

#include "rlib.h"

namespace RLibTest
{
    namespace RLibStackTest
    {
        void StackTest::SetUp()
        {
        }

        void StackTest::TearDown()
        {
        }

        TEST_F(StackTest, TestInit)
        {
            Stack s;
            Stack* sp = &s;
            STACK_INIT(sp, int);
            ASSERT_TRUE(sp->stack.data != nullptr);

            stack_free(sp);
        }

        TEST_F(StackTest, TestFree)
        {
            Stack s;
            Stack* sp = &s;
            STACK_INIT(sp, int);
            ASSERT_TRUE(sp->stack.data != nullptr);

            stack_free(sp);

            ASSERT_TRUE(sp->stack.data == nullptr);
            ASSERT_EQ(sp->stack.capacity, 0);
            ASSERT_EQ(sp->stack.count, 0);
            ASSERT_EQ(sp->stack.elemSize, 0);
        }

        TEST_F(StackTest, PushTest)
        {
            Stack s;
            Stack* sp = &s;
            STACK_INIT(sp, int);

            ASSERT_EQ(stack_size(sp), 0);

            for (int i = 0; i < 10; i++)
            {
                STACK_PUSH(sp, int, 0);
                ASSERT_EQ(sp->stack.count, i + 1);
            }

            stack_free(sp);
        }

        TEST_F(StackTest, PopTest)
        {
            Stack s;
            Stack* sp = &s;
            STACK_INIT(sp, int);

            ASSERT_EQ(stack_size(sp), 0);

            for (int i = 0; i < 10; i++)
            {
                STACK_PUSH(sp, int, i);
                ASSERT_EQ(sp->stack.count, i + 1);
            }

            for (int i = 0; i < 10; i++)
            {
                int v;
                stack_pop(sp, &v);
                ASSERT_EQ(sp->stack.count, 10 - i - 1);
                ASSERT_EQ(v, 10 - i - 1);
            }

            ASSERT_EQ(sp->stack.count, 0);

            stack_free(sp);
        }

        TEST_F(StackTest, PeekTest)
        {
            Stack s;
            Stack* sp = &s;
            STACK_INIT(sp, int);

            ASSERT_EQ(stack_size(sp), 0);

            for (int i = 0; i < 10; i++)
            {
                STACK_PUSH(sp, int, i);
                int v;
                stack_peek(sp, &v);
                ASSERT_EQ(v, i);
            }

            int v;
            stack_peek(sp, &v);
            ASSERT_EQ(v, 9);

            stack_free(sp);
        }


        TEST_F(StackTest, IsEmptyTest)
        {
            Stack s;
            Stack* sp = &s;
            STACK_INIT(sp, int);

            ASSERT_EQ(stack_size(sp), 0);
            ASSERT_TRUE(stack_isEmpty(sp));

            for (int i = 0; i < 10; i++)
            {
                STACK_PUSH(sp, int, 0);
                ASSERT_EQ(stack_size(sp), i + 1);
            }

            ASSERT_FALSE(stack_isEmpty(sp));

            stack_free(sp);
        }

        TEST_F(StackTest, SizeTest)
        {
            Stack s;
            Stack* sp = &s;
            STACK_INIT(sp, int);

            ASSERT_EQ(stack_size(sp), 0);

            for (int i = 0; i < 10; i++)
            {
                STACK_PUSH(sp, int, 0);
                ASSERT_EQ(stack_size(sp), i + 1);
            }

            stack_free(sp);
        }
    }
}