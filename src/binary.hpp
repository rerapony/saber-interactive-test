#pragma once

#include <iostream>

void helper(int number, char* bin_str, int i) {
    if (number > 1)
        helper(number / 2, bin_str, i-1);
    bin_str[i] = number%2 + '0';
}

/* Отрицательные числа можно представляются в двоичной форме разными способами
 * в задачи использовал метод "дополнительного кода" (two's complement)
 * Надеюсь, iostream использовать можно :) */

void binary(int number) {
    bool is_negative = false;
    if (number < 0) {
        is_negative = true;
        number *= (-1);
    }

    int size = 8*sizeof(int);
    char bin_str[size+1];
    for (int i = 0; i < size; ++i)
        bin_str[i] = '0';

    helper(number, bin_str, size-1);
    // если число отрицательное - используем дополнительный код (инвертируем строку и прибавляем 1)
    if (is_negative) {
        int i = size-1;
        for (; i >= 0; --i)
            if (bin_str[i] == '1')
                break;
        // нет единиц в числе (пример: INT32_MIN)
        if (i == -1)
            bin_str[0] = '1';

        for (int k = i-1 ; k >= 0; --k)
        {
            if (bin_str[k] == '1')
                bin_str[k] = '0';
            else
                bin_str[k] = '1';
        }
    }

    for (int i = 0; i < size; ++i) {
        if (i > 0 && i % 4 == 0)
            std::cout << ' ';
        std::cout << bin_str[i];
    }
}
