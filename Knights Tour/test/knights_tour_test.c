#include <stdio.h>  /* printf */
#include <unistd.h> /* sleep */
#include <stdlib.h> /* system */
#include <time.h>   /* time */

#include "colourmod.h" /* color prints */
#include "knights_tour.h"

#define X_POS 0
#define Y_POS 0

void TestAllFunc();
void TestKnightTour(unsigned char *);
int TestWarnsdorff(unsigned char *);
void PrettyPrinting(unsigned char *);
void PrintArr(char *, int);

int main()
{
    TestAllFunc();
    return (0);
}

void TestAllFunc()
{
    int print_opt = 0;
    int status = 0;
    unsigned char path[70] = {0};
    unsigned char wpath[70] = {0};
    double delta_w, delta_b;
    clock_t start_time, end_time;

    start_time = clock();
    TestKnightTour(path);
    end_time = clock();
    delta_b = (double)(end_time - start_time) / (CLOCKS_PER_SEC / 1000);

    start_time = clock();
    status = TestWarnsdorff(wpath);
    end_time = clock();
    delta_w = (double)(end_time - start_time) / (CLOCKS_PER_SEC / 1000);
    switch (status)
    {
    case TRUE:
        PRINT(UNDERLINE, BG_BLACK);
        printf("Position [%d, %d] has a Knights Tour solution!\n", X_POS, Y_POS);
        printf("Which solution would you like to see?\n 1 -> Warnsdorff\n 2 -> Brute force\n 3 -> none\n");
        RESET(0);
        scanf("%d", &print_opt);
        switch (print_opt)
        {
        case (1):
            printf("Time for Warnsdorff was %fms\n", delta_w);
            sleep(2);
            PrettyPrinting(wpath);
            break;
        case (2):
            printf("Time for Brute Force was %fms\n", delta_b);
            sleep(2);
            PrettyPrinting(path);
            break;
        }
        break;
    case FALSE:
        PRINT(BOLD, FG_RED);
        printf("Position [%d, %d] doesnt have a Knights Tour solution!\n", X_POS, Y_POS);
        break;
    case TIMEOUT:
        PRINT(BOLD, FG_GREEN);
        printf("Position [%d, %d] Timed out!\n", X_POS, Y_POS);
        break;
    }
    printf("~ END OF KNIGHTS TOUR TEST ~\n");
}

int TestWarnsdorff(unsigned char *path)
{
    return (WarnsdorffKnightsTour(X_POS, Y_POS, path));
}

void TestKnightTour(unsigned char *path)
{
    KnightsTour(X_POS, Y_POS, path);
}

void PrettyPrinting(unsigned char *path)
{
    char buffer2[1000] = {0};
    int i = 0;
    system("clear");
    sleep(1);
    PRINT(BOLD, BG_RED);
    printf("       ~ step [%d] ~     \n\n", i);
    RESET(0);
    PrintArr(buffer2, i);
    for (i = 1; i < 65; i++)
    {
        buffer2[path[i - 1]] = i;
        sleep(1);
        system("clear");
        PRINT(BOLD, BG_RED);
        printf("       ~ step [%d] ~     \n\n", i);
        RESET(0);
        PrintArr(buffer2, i);
    }
}

void PrintArr(char *arr, int step)
{
    int i = 1;
    for (i = 1; i < 65; ++i)
    {
        for (i = 1; i < 65; i++)
        {
            if (arr[i - 1] == 0)
            {
                printf("%3d", arr[i - 1]);
            }
            else if (step == arr[i - 1])
            {
                PRINT(BOLD, BG_GREEN);
                printf("%3d", arr[i - 1]);
                RESET(0);
            }
            else
            {
                PRINT(BOLD, BG_BLACK);
                printf("%3d", arr[i - 1]);
                RESET(0);
            }
            if (i % 8 == 0)
            {
                printf("\n\n");
            }
        }
        printf("\n");
    }
}