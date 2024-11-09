#include "../include/incl.h"

#include "../external/mlib/m-dict.h"
#include "../external/mlib/m-string.h"

// Let's define a dictionary of 'unsigned int' --> 'char
DICT_DEF2(CharFreqMap, char, M_BASIC_OPLIST, u8, M_BASIC_OPLIST)

// Let's create a synonym for its oplist.
#define M32_OPLIST                                                   \
    DICT_OPLIST(CharFreqMap, M_BASIC_OPLIST, M_BASIC_OPLIST)

void arr_i_print(const int *arr, const int len)
{
    int max = 0;
    for (int i = 0; i < len; ++i)
        if (abs(arr[i]) > max)
            max = abs(arr[i]);
    const int digits = (int)log10(max + 1) + 2;
    printf("[");
    for (int i = 0; i < len; ++i) {
        printf("%*d", digits, arr[i]);
        if (i < len - 1)
            printf(",");
    }
    printf("]");
}

void arr_f_print(double *arr, int len)
{
    int max = 0;
    for (int i = 0; i < len; ++i)
        if (fabs(arr[i]) > max)
            max = fabs(arr[i]);
    int digits = (int)log10(max + 1) + 2;
    printf("[");
    for (int i = 0; i < len; ++i) {
        printf("%*f", digits, arr[i]);
        if (i < len - 1)
            printf(",");
    }
    printf("]");
}

size_t duplicate_count(string_t text)
{
    size_t retval = 0;
    CharFreqMap_t map;
    CharFreqMap_init(map);
    for (int i = 0; i < string_size(text); ++i) {
        unsigned char c = string_get_char(text, i);
        if (isalpha(c))
            CharFreqMap_set_at(map, toupper(c),
                CharFreqMap_get(map, toupper(c)) == NULL
                    ? 1
                    : *CharFreqMap_get(map, toupper(c)) + 1);
        else
            CharFreqMap_set_at(map, c,
                CharFreqMap_get(map, c) == NULL
                    ? 1
                    : *CharFreqMap_get(map, c) + 1);
    }
    CharFreqMap_it_t it;
    for (CharFreqMap_it(it, map); !CharFreqMap_end_p(it);
         CharFreqMap_next(it)) {
        struct CharFreqMap_pair_s *item = CharFreqMap_ref(it);
        if (item->value > 1)
            retval++;
    }
    CharFreqMap_clear(map);
    return retval;
}

