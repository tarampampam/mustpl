#ifndef GUARD_UTILS_H
#define GUARD_UTILS_H

char *substr(const char *src, int m, int n);

char *readFile(const char *filePath);

void writeFile(const char *filePath, const char *text);

#endif //GUARD_UTILS_H
