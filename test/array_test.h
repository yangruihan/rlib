#pragma once

#include "gtest/gtest.h"

namespace RLibTest
{
    class ArrayTest : public ::testing::Test
    {
    protected:
        void SetUp() override;
        void TearDown() override;
    };
} // namespace RLibTest
