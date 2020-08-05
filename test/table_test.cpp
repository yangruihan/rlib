#include "table_test.h"

#include "rlib.h"

#define TEST_COUNT 10000

namespace RLibTest
{
    namespace RLibTableTest
    {
        void TableTest::SetUp()
        {
        }
        
        void TableTest::TearDown()
        {
        }

        TEST_F(TableTest, TestInit)
        {
            Table _;
            Table* t = &_;
            TABLE_INIT(t, int);
            ASSERT_EQ(t->count, 0);
            ASSERT_EQ(t->capacity, 0);
            ASSERT_EQ(t->elemSize, sizeof(int));
            ASSERT_TRUE(t->entries == NULL);

            ASSERT_EQ(t->_entrySize, sizeof(int) + sizeof(uint32_t));
            ASSERT_FALSE(t->_zeroSetFlag);
            ASSERT_TRUE(t->_zeroValue != NULL);
            ASSERT_TRUE(t->_emptyMem != NULL);
            ASSERT_EQ(*t->_zeroValue, 0);
            ASSERT_EQ(*t->_emptyMem, 0);
        }

        TEST_F(TableTest, TestEmptyFree)
        {
            Table _;
            Table* t = &_;
            TABLE_INIT(t, int);
            table_free(t);
            ASSERT_EQ(t->count, 0);
            ASSERT_EQ(t->capacity, 0);
            ASSERT_EQ(t->elemSize, 0);
            ASSERT_TRUE(t->entries == NULL);

            ASSERT_EQ(t->_entrySize, 0);
            ASSERT_FALSE(t->_zeroSetFlag);
            ASSERT_TRUE(t->_zeroValue == NULL);
            ASSERT_TRUE(t->_emptyMem == NULL);
        }

        TEST_F(TableTest, TestFree)
        {
            Table _;
            Table* t = &_;
            TABLE_INIT(t, int);
            TABLE_SET(t, int, 1, 5);
            TABLE_SET(t, int, 2, 5);
            TABLE_SET(t, int, 3, 5);
            table_free(t);
            ASSERT_EQ(t->count, 0);
            ASSERT_EQ(t->capacity, 0);
            ASSERT_EQ(t->elemSize, 0);
            ASSERT_TRUE(t->entries == NULL);
            
            ASSERT_FALSE(t->_zeroSetFlag);
            ASSERT_TRUE(t->_zeroValue == NULL);
            ASSERT_TRUE(t->_emptyMem == NULL);
        }

        TEST_F(TableTest, TestBaseType)
        {
            Table _;
            Table* t = &_;
            TABLE_INIT(t, int);
            TABLE_SET(t, int, 0, 1);
            ASSERT_EQ(t->count, 1);
            TABLE_SET(t, int, 1, 5);
            ASSERT_EQ(t->count, 2);
            TABLE_SET(t, int, 2, 10);
            ASSERT_EQ(t->count, 3);
            TABLE_SET(t, int, 3, 15);
            ASSERT_TRUE(t->entries != NULL);
            ASSERT_EQ(t->count, 4);

            int ret;
            ASSERT_TRUE(table_get(t, 0, &ret));
            ASSERT_EQ(ret, 1);

            ASSERT_TRUE(table_get(t, 1, &ret));
            ASSERT_EQ(ret, 5);

            ASSERT_TRUE(table_get(t, 2, &ret));
            ASSERT_EQ(ret, 10);

            ASSERT_TRUE(table_get(t, 3, &ret));
            ASSERT_EQ(ret, 15);

            TABLE_SET(t, int, 2, 30);
            ASSERT_TRUE(table_get(t, 2, &ret));
            ASSERT_EQ(ret, 30);

            ASSERT_TRUE(table_del(t, 2));
            ASSERT_FALSE(table_get(t, 2, nullptr));

            ASSERT_TRUE(table_get(t, 0, &ret));
            ASSERT_EQ(ret, 1);

            ASSERT_TRUE(table_get(t, 1, &ret));
            ASSERT_EQ(ret, 5);

            ASSERT_TRUE(table_get(t, 3, &ret));
            ASSERT_EQ(ret, 15);

            ASSERT_TRUE(t->entries != NULL);
            ASSERT_EQ(t->count, 3);

            table_free(t);
        }

        TEST_F(TableTest, TestPointerType)
        {
            Table _;
            Table* t = &_;
            TABLE_INIT(t, B);

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

            ASSERT_TRUE(table_set(t, 0, &b1));
            ASSERT_TRUE(table_set(t, 100, &b2));
            ASSERT_TRUE(table_set(t, 100000, b3));

            ASSERT_EQ(t->count, 3);

            B b4;
            ASSERT_TRUE(table_get(t, 100000, &b4));
            ASSERT_EQ(b4.a.f, 300.0);
            ASSERT_EQ(b4.i, 300);
            ASSERT_EQ(b4.b->i, 3100);

            ASSERT_TRUE(table_get(t, 0, &b4));
            ASSERT_EQ(b4.a.f, 100.0);
            ASSERT_EQ(b4.i, 100);
            ASSERT_EQ(b4.b->i, 1100);

            ASSERT_TRUE(table_get(t, 100, &b4));
            ASSERT_EQ(b4.a.f, 200.0);
            ASSERT_EQ(b4.i, 200);
            ASSERT_EQ(b4.b->i, 2100);

            ASSERT_TRUE(table_del(t, 0));
            ASSERT_FALSE(table_get(t, 0, nullptr));
            ASSERT_EQ(t->count, 2);

            table_free(t);
        }

        TEST_F(TableTest, TestSetGet)
        {
            Table _;
            Table* t = &_;
            TABLE_INIT(t, int);

            for (int i = 0; i < TEST_COUNT; i++)
            {
                TABLE_SET(t, int, i, i);
            }

            int ret;
            for (int i = 0; i < TEST_COUNT; i++)
            {
                ASSERT_TRUE(table_get(t, i, &ret));
                ASSERT_EQ(ret, i);
            }

            table_free(t);
        }

        TEST_F(TableTest, TestDel)
        {
            Table _;
            Table* t = &_;
            TABLE_INIT(t, int);

            for (int i = 0; i < TEST_COUNT; i++)
            {
                TABLE_SET(t, int, i, i);
            }
            
            for (int i = 0; i < TEST_COUNT; i++)
            {
                ASSERT_TRUE(table_del(t, i));
                ASSERT_EQ(t->count, TEST_COUNT - i - 1);
            }

            table_free(t);
        }
    }
}
