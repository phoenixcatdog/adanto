#include <iostream>
#include "inter_class_data.h"

int fill_digits(image_info *info,int num, QChar **digit_1,QChar **digit_2)
{
    int   d1;
    int   d2;

    if (num> 99)
    {return(-1);}

    d1 = num / 10;
    d2 = num - 10*d1;

    *digit_1 = &(info->all_digits[d1]);
    *digit_2 = &(info->all_digits[d2]);

    return(0);
}
