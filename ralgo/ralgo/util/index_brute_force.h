#ifndef RALGO_UTIL_INDEX_BRUTE_FORCE_H
#define RALGO_UTIL_INDEX_BRUTE_FORCE_H

#include <igris/compiler.h>

__BEGIN_DECLS

/**
Функция перебирает комбинации выборок неповторяющихся arrsize элементов из
elements вариантов. Левый индекс всегда меньше правого.

Количество шагов алгоритма определяется количеством сочетаний elements по
arrsize.

n = elements
m = arrsize

C = n! / (n-m)! / m!

Для n = 5, m = 3: C = 5! / (5-3)! / 3! = 5*4*3*2*1 / 1*2 / 1*2*3 = 10

Пример работы функции:

N    0 1 2 3 4       arr:

1	[x y z 0 0]     0 1 2
2	[x y 0 z 0]     0 1 3
3	[x y 0 0 z]     0 1 4
4	[x 0 y z 0]     0 2 3
5	[x 0 y 0 z]     0 2 4
6	[x 0 0 y z]     0 3 4
7	[0 x y z 0]     1 2 3
8	[0 x y 0 z]     1 2 4
9	[0 x 0 y z]     1 3 4
10	[0 0 x y z]     2 3 4
*/
void index_brute_force_init(int *arr, int arrsize);

int index_brute_force_next(int *arr, int arrsize, int elements);

__END_DECLS

#endif
