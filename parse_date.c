//
// Created by Radek Juppa on 21/01/2018.
//

#include "parse_date.h"
#include <stdio.h>


/* ____________________________________________________________________________

    int strparsetime(const char *str, const char *format, struct tm *tm1)

    This is replacement of strptime(),
    because it is missing on Windows
   ____________________________________________________________________________
*/
int strparsetime(const char *str, struct tm *tm1){
    int year = 0, month = 0, day = 0;
    sscanf( str, "%d-%d-%d ", &year, &month, &day );
    tm1->tm_isdst = -1;
    tm1->tm_year = year - 1900;
    tm1->tm_mon = month - 1;
    tm1->tm_mday = day;
    tm1->tm_hour = 0;
    tm1->tm_min = 0;
    tm1->tm_sec = 0;
    if( year<2000 ){
        return 1;
    }
    return 0;
}