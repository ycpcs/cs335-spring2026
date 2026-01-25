#include <stdio.h>
#include <stdlib.h>

int x = 100;

struct Vector3
{
    float x, y, z;
};

int main()
{
    int a = 2;
    float b = 3.141592;
    
    struct Vector3 v;
    v.x = 10;
    v.y = 11;
    v.z = 12;

    static int y;

    int* ptr = (int*) malloc(2 * sizeof(int));
    ptr[0] = 10;
    ptr[1] = 20;

    free(ptr);

    return 1;
}
