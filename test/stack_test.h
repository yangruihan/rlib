#pragma once

#include "gtest/gtest.h"

namespace RLibTest
{
    namespace RLibStackTest
    {
        class StackTest : public ::testing::Test
        {
        protected:
            void SetUp() override;
            void TearDown() override;
        };
    }
} // namespace RLibTest
