#pragma once

#include "gtest/gtest.h"

namespace RLibTest
{
    namespace RLibTableTest
    {
        struct A
        {
            double f;
        };

        struct B
        {
            int i;
            A a;
            B* b;
        };

        class TableTest : public ::testing::Test
        {
        protected:
            void SetUp() override;
            void TearDown() override;
        };
    }
} // namespace RLibTest
