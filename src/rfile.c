#include "rfile.h"

#include <string.h>

#include "rplatform.h"

#ifdef R_PLATFORM_WINDOWS
    #define SEP_CHAR '\\'
#else
    #define SEP_CHAR '/'
#endif

bool readFile(const char* path, char** content, int* size)
{
    FILE* file = fopen(path, "rb");
    if (file == NULL) {
        RLOG_ERROR("Could not open file \"%s\".\n", path);
        return false;
    }

    fseek(file, 0L, SEEK_END);
    const size_t fileSize = ftell(file);
    rewind(file);

    char* buffer = ALLOCATE(char, fileSize + 1);
    if (buffer == NULL) {
        RLOG_ERROR("Not enough memory to read \"%s\".\n", path);
        return false;
    }

    size_t bytesRead = fread(buffer, sizeof(char), fileSize, file);
    if (bytesRead < fileSize) {
        RLOG_ERROR("Could not read file \"%s\".\n", path);
        FREE_ARRAY(char, buffer, fileSize + 1);
        return false;
    }

    buffer[bytesRead] = '\0';
    fclose(file);

    *content = buffer;
    *size = bytesRead;
    return true;
}

void combinePath(const char* path, const char* path2, char** ret)
{
    if (path == NULL && path2 == NULL)
    {
        *ret = NULL;
        return;
    }
    else if (path == NULL && path2 != NULL)
    {
        *ret = ALLOCATE(char, strlen(path2) + 1);
        strcpy(*ret, path2);
        return;
    }
    else if (path != NULL && path2 == NULL)
    {
        *ret = ALLOCATE(char, strlen(path) + 1);
        strcpy(*ret, path);
        return;
    }

    int pathLen = strlen(path);
    int path2Len = strlen(path);

    *ret = ALLOCATE(char, pathLen + path2Len + 2);

    strcpy(*ret, path);
    if ((*ret)[pathLen - 1] == SEP_CHAR)
        pathLen--;
    else
        (*ret)[pathLen] = SEP_CHAR;

    if (path2[0] == SEP_CHAR)
        strcpy(*ret + 1 + pathLen, path2 + 1);
    else
        strcpy(*ret + 1 + pathLen, path2);
}