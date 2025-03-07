#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

void save_schedule(int year, int month, int day, const char *event)
{
    FILE *file = fopen("schedule.csv", "a");
    if (!file)
    {
        printf("error: 予定を保存出来ません。\n");
        return ;
    }
    fprintf(file, "%d,%d,%d,%s\n", year, month, day, event);
    fclose(file);
    printf("予定を保存しました: %d/%d/%d - %s\n", year, month, day, event);
}

void list_schedule(int year, int month, int day)
{
    FILE *file = fopen("schedule.csv", "r");
    if (!file)
    {
        printf("予定データがまだありません\n");
        return ;
    }

    int y, m, d;
    char event[100];
    int found = 0;

    while (fscanf(file, "%d,%d,%d,%99[^\n]", &y, &m, &d, event) != EOF)
    {
        if (y == year && m == month && (day == 0 || d == day))
        {
            printf("%d/%d/%d: %s\n", y, m, d, event);
            found = 1;
        }
    }
    fclose(file);

    if (!found)
        printf("指定した日付には予定はありません\n");
}

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

void delete_schedule(int year, int month, int day)
{
    FILE *file = fopen("schedule.csv", "r");
    if (!file)
    {
        printf("予定データがありません\n");
        return ;
    }
    FILE *temp = fopen("temp.csv", "w");
    if (!temp)
    {
        printf("error: 削除用ファイルを作成できません\n");
        fclose(file);
        return ;
    }

    int y, m, d;
    char event[100];
    int found = 0;

    while (fscanf(file, "%d,%d,%d,%99[^\n]", &y, &m, &d, event) != EOF)
    {
        if (y == year && m == month && d == day)
            found = 1;
        else
            fprintf(temp, "%d,%d,%d,%s\n", y, m, d, event);
    }
    fclose(file);
    fclose(temp);

    if (found)
    {
        remove("schedule.csv");
        rename("temp.csv", "schedule.csv");
        printf("予定を削除しました: %d/%d/%d\n", year, month, day);
    }
    else
    {
        remove("temp.csv");
        printf("指定した日付には予定がありません\n");
    }
}

int main(int argc, char *argv[])
{
    char event[100] = "";
    int year = 0, month = 0, day = 0;
    int add_flag = 0, delete_flag = 0, list_flag = 0;

    for (int i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "-d") == 0 && i + 1 < argc)
        {
            sscanf(argv[i + 1], "%d-%d-%d", &year, &month, &day);
            i++;
        }

        if (strcmp(argv[i], "-a") == 0 && i + 1 < argc)
        {
            strcpy(event, argv[i + 1]);
            add_flag = 1;
            i++;
        }
        else if (strcmp(argv[i], "-delete") == 0)
            delete_flag = 1;
        else if (strcmp(argv[i], "-list") == 0)
            list_flag = 1;
    }

    if (add_flag && year && month && day)
    {
        save_schedule(year, month, day, event);
        return 0;
    }
    else if (delete_flag && year && month && day)
    {
        delete_schedule(year, month, day);
        return 0;
    }
    else if (list_flag && year && month)
    {
        if (day)
            list_schedule(year, month, day);
        else
            list_schedule(year, month, 0);
        return 0;
    }
    else
    {
        print_calender(year, month);
        return 0;
    }

    printf("使い方:\n");
    printf("  予定を追加:   ./a.out -a \"予定\" -d YYYY-MM-DD\n");
    printf("  予定を削除:   ./a.out -d YYYY-MM-DD -delete\n");
    printf("  予定を表示:   ./a.out -d YYYY-MM-DD -list\n");
    printf("  月の予定表示: ./a.out -d YYYY-MM -list\n");
    printf("オプションは順不同です！\n");
    return 1;
}