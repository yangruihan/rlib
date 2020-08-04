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
            TABLE_SET(t, int, 1, 5);
            TABLE_SET(t, int, 2, 10);
            TABLE_SET(t, int, 3, 15);
            ASSERT_TRUE(t->entries != NULL);

            int ret;
            ASSERT_TRUE(table_get(t, 1, &ret));
            ASSERT_EQ(ret, 5);

            ASSERT_TRUE(table_get(t, 2, &ret));
            ASSERT_EQ(ret, 10);

            ASSERT_TRUE(table_get(t, 3, &ret));
            ASSERT_EQ(ret, 15);
        }
    }
}