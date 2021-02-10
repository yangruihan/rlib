#pragma once

#include "gtest/gtest.h"

namespace RLibTest
{
    namespace RLibFileTest
    {
        class FileTest : public ::testing::Test
        {
        protected:
            void SetUp() override;
            void TearDown() override;
        };
    }
} // namespace RLibTest
