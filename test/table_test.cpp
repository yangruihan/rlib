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
            ASSERT_TRUE(t->_zeroValue != NULL);
            ASSERT_TRUE(t->_emptyMem != NULL);
            ASSERT_EQ(*t->_zeroValue, 1);
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
        }

        TEST_F(TableTest, TestBaseType)
        {
            Table _;
            Table* t = &_;
            TABLE_INIT(t, int);
            TABLE_SET(t, int, 0, 1);
            TABLE_SET(t, int, 1, 5);
            TABLE_SET(t, int, 2, 10);
            TABLE_SET(t, int, 3, 15);
            ASSERT_TRUE(t->entries != NULL);

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
        }
    }
}
