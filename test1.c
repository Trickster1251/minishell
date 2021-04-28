#include <stdio.h>
#include "libft/libft.h"

int     who_more(char *str, char *str1)
{
    if (ft_strlen(str) >= ft_strlen(str1))
        return (ft_strlen(str1));
    return (ft_strlen(str));
}

char    **sorting(char **arr)
{
    int     i = 0;
    int     len;
    char    *tmp;

    while(arr[i + 1])
    {
        len = who_more(arr[i], arr[i + 1]);
        if ((ft_strncmp(arr[i], arr[i + 1], (size_t)len)) < 0)
        {
            tmp = arr[i];
            arr[i] = arr[i + 1];
            arr[i + 1] = tmp;
        }
        i++;
    }
    return (arr);
}

void qsort (int* arr, int left, int right)
{
    int i = left, j = right;
    int temp, pivot = arr[ (left+right)/2 ];

    while (i <= j)
    {
        while (arr[i] < pivot) i++;
        while (arr[j] > pivot) j--;

        if (i <= j)
        {
            if (arr[i] > arr[j])
            {
                temp = arr[i]; arr[i] = arr[j]; arr[j] = temp;
            }
            i++;
            j--;
        }
    };

    if (left < j) qsort (arr, left, j);
    if (i < right) qsort (arr, i, right);
}

int main(int argc, char **argv)
{
    //    char **arr = sorting(*argv);

    int    i = -1;
    char *tmp = *(argv);

    while(arr[++i])
    {
        ft_putendl_fd(arr[i], 1);
    }
}
