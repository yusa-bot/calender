#include <stdio.h>
#include <time.h>
#include <stdlib.h>

typedef struct 
{
    int month;
    int day;
} Holiday;

Holiday holidays[] =
{
    {1, 1},
    {2, 11},
    {4, 29},
    {5, 3}, 
    {5, 4}, 
    {5, 5},
    {11, 3},
    {11, 23}
};

int num_holidays = sizeof(holidays) / sizeof(holidays[0]);

int is_holiday(int month, int day)
{
    for (int i = 0; i < num_holidays; i++)
    {
        if (holidays[i].month == month && holidays[i].day == day)
            return 1;
    }
    return 0;
}

void print_calender(int year, int month)
{
    struct tm time_info = {0};
    time_info.tm_year = year - 1900;
    time_info.tm_mon = month - 1;
    time_info.tm_mday = 1;

    mktime(&time_info);

    int start_day = time_info.tm_wday;
    int days_in_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
        days_in_month[1] = 29;

    printf("\n %d年 %d月\n", year, month);
    printf(" 日 月 火 水 木 金 土\n");

    for (int i = 0; i < start_day; i++)
        printf("   ");

    int days = days_in_month[month - 1];

    for (int day = 1; day <= days; day++)
    {
        int current_day = (start_day + day - 1) % 7;
        char holiday_name[20] = "";

        if (is_holiday(month, day))
            printf("\033[31m%3d*\033[0m", day);
        else if (current_day == 0)
            printf("\033[31m%3d\033[0m", day);
        else if (current_day == 6)
            printf("\033[34m%3d\033[0m", day);
        else
            printf("%3d", day);
        if (current_day == 6)
            printf("\n");
    }
    printf("\n");
}

int main(int argc, char *argv[])
{
    int year, month;

    if (argc == 3)
    {
        year = atoi(argv[1]);
        month = atoi(argv[2]);
        if (month < 1 || month > 12)
        {
            printf("error: 有効な月を入力してください\n");
            return 1;
        }
    }
    else if (argc == 1)
    {
        time_t t = time(NULL);
        struct tm *now = localtime(&t);
        year = now->tm_year + 1900;
        month = now->tm_mon + 1;
    }
    else
    {
        printf("使い方: %s 年 月\n", argv[0]);
        return 1;
    }
    print_calender(year, month);
    return 0;
}