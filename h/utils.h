#ifndef UTILS_H
#define UTILS_H

#define OPEN_FILE(var, name, mode)         \
    FILE* var = fopen(name, mode);         \
    if (var == NULL) return ERR_OPEN_FILE;

static int myMin(int a, int b)
{
    return a < b ? a : b;
}

static int myMax(int a, int b)
{
    return a > b ? a : b;
}

#endif // UTILS_H