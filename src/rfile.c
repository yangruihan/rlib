#include "rfile.h"

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