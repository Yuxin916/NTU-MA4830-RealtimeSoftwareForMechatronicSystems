#include <stdio.h>
#include <stdlib.h>

#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include "print.h"

int main()
{
    char *filename = "title.txt";
    FILE *fptr = NULL;

    if ((fptr = fopen(filename, "r")) == NULL)
    {
        fprintf(stderr, "error opening %s\n", filename);
        return 1;
    }

    print_image(fptr);

    while(true) { // outer loop
        // key in dimension -> dimension_result
        
        // object selection(dimension_result) -> shape_or_object_selected

        // calculation function (shape_or_object_selected) -> calculation_result
            // unit selection
            // param selection

        // display(calculatioon_result)
        // histroy/calculate/exit
    }

    return 0;
}