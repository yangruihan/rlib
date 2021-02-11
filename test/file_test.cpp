#include "file_test.h"

#include "rlib.h"

namespace RLibTest
{
    namespace RLibFileTest
    {
        void FileTest::SetUp()
        {
        }
        
        void FileTest::TearDown()
        {
        }

        TEST_F(FileTest, TestPathCombine)
        {
#ifdef R_PLATFORM_WINDOWS
            const char* path = "Test\\Path\\";
            const char* path2 = "test.txt";
#else
            const char* path = "Test/Path/";
            const char* path2 = "test.txt";
#endif

            char* ret;
            combinePath(path, path2, &ret);

#ifdef R_PLATFORM_WINDOWS
            EXPECT_STREQ(ret, "Test\\Path\\test.txt");
#else
            EXPECT_STREQ(ret, "Test/Path/test.txt");
#endif

            FREE(char, ret);
        }

        TEST_F(FileTest, TestPathCombine2)
        {
#ifdef R_PLATFORM_WINDOWS
            const char* path = "Test\\Path\\";
            const char* path2 = NULL;
#else
            const char* path = "Test/Path/";
            const char* path2 = NULL;
#endif

            char* ret;
            combinePath(path, path2, &ret);

#ifdef R_PLATFORM_WINDOWS
            EXPECT_STREQ(ret, "Test\\Path\\");
#else
            EXPECT_STREQ(ret, "Test/Path/");
#endif

            FREE(char, ret);
        }


        TEST_F(FileTest, TestPathCombine3)
        {
#ifdef R_PLATFORM_WINDOWS
            const char* path = NULL;
            const char* path2 = "test.txt";
#else
            const char* path = NULL;
            const char* path2 = "test.txt";
#endif

            char* ret;
            combinePath(path, path2, &ret);

#ifdef R_PLATFORM_WINDOWS
            EXPECT_STREQ(ret, "test.txt");
#else
            EXPECT_STREQ(ret, "test.txt");
#endif

            FREE(char, ret);
        }

        TEST_F(FileTest, TestPathCombine4)
        {
#ifdef R_PLATFORM_WINDOWS
            const char* path = "Test\\Path\\A\\B";
            const char* path2 = "C\\D\\test.txt";
#else
            const char* path = "Test/Path/A/B";
            const char* path2 = "C/D/test.txt";
#endif

            char* ret;
            combinePath(path, path2, &ret);

#ifdef R_PLATFORM_WINDOWS
            EXPECT_STREQ(ret, "Test\\Path\\A\\B\\C\\D\\test.txt");
#else
            EXPECT_STREQ(ret, "Test/Path/A/B/C/D/test.txt");
#endif

            FREE(char, ret);
        }

        TEST_F(FileTest, TestPathCombine5)
        {
#ifdef R_PLATFORM_WINDOWS
            const char* path = "Test\\Path\\A\\B\\";
            const char* path2 = "\\C\\D\\test.txt";
#else
            const char* path = "Test/Path/A/B/";
            const char* path2 = "/C/D/test.txt";
#endif

            char* ret;
            combinePath(path, path2, &ret);

#ifdef R_PLATFORM_WINDOWS
            EXPECT_STREQ(ret, "Test\\Path\\A\\B\\C\\D\\test.txt");
#else
            EXPECT_STREQ(ret, "Test/Path/A/B/C/D/test.txt");
#endif

            FREE(char, ret);
        }

        TEST_F(FileTest, TestPathCombine6)
        {
#ifdef R_PLATFORM_WINDOWS
            const char* path = "Test\\Path\\A\\B";
            const char* path2 = "\\C\\D\\test.txt";
#else
            const char* path = "Test/Path/A/B";
            const char* path2 = "/C/D/test.txt";
#endif

            char* ret;
            combinePath(path, path2, &ret);

#ifdef R_PLATFORM_WINDOWS
            EXPECT_STREQ(ret, "Test\\Path\\A\\B\\C\\D\\test.txt");
#else
            EXPECT_STREQ(ret, "Test/Path/A/B/C/D/test.txt");
#endif

            FREE(char, ret);
        }
    }
}