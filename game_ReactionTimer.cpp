#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
double now_ms(void) {
    LARGE_INTEGER f, t;
    QueryPerformanceFrequency(&f);
    QueryPerformanceCounter(&t);
    return (double)t.QuadPart * 1000.0 / (double)f.QuadPart;
}
void sleep_ms(int ms) { Sleep(ms); }
#else
#include <unistd.h>
double now_ms(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1000.0 + ts.tv_nsec / 1000000.0;
}
void sleep_ms(int ms) { usleep(ms * 1000); }
#endif

void clearInput(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main(void) {
    int choice, rounds = 0, i, played = 0;
    int score, totalScore = 0;
    double ms, times[20], best, worst, sum, avg, range, t1, t2;

    srand((unsigned)time(NULL));

    do {
        printf("\n===== REACTION TIMER =====\n");
        printf("1. Play rounds\n");
        printf("2. View stats\n");
        printf("3. Reset session\n");
        printf("4. Exit\n");
        printf("Choice: ");
        if (scanf("%d", &choice) != 1) { clearInput(); choice = 0; continue; }
        clearInput();

        switch (choice) {
            case 1:
                printf("How many rounds (1-20)? ");
                if (scanf("%d", &rounds) != 1) rounds = 0;
                clearInput();

                if (rounds < 1 || rounds > 20) {
                    printf("Invalid round count. Setting to 5.\n");
                    rounds = 5;
                }

                for (i = 0; i < rounds; i++) {
                    if (played >= 20) {
                        printf("Session full. Reset to continue.\n");
                        break;
                    }

                    printf("\nRound %d - get ready...\n", played + 1);
                    fflush(stdout);
                    sleep_ms(1000 + rand() % 3000);

                    printf(">>> PRESS ENTER NOW! <<<\n");
                    fflush(stdout);

                    t1 = now_ms();
                    clearInput();
                    t2 = now_ms();

                    ms = t2 - t1;
                    times[played] = ms;

                    if (ms < 150) {
                        if (ms < 100) score = 120;
                        else score = 100;
                        printf("Lightning fast! %.0f ms\n", ms);
                    } else if (ms < 300) {
                        if (ms < 220) score = 80;
                        else score = 65;
                        printf("Sharp. %.0f ms\n", ms);
                    } else if (ms < 500) {
                        if (ms < 400) score = 45;
                        else score = 30;
                        printf("Steady. %.0f ms\n", ms);
                    } else if (ms < 900) {
                        score = 15;
                        printf("A bit slow. %.0f ms\n", ms);
                    } else {
                        score = 5;
                        printf("Drifted off? %.0f ms\n", ms);
                    }

                    totalScore += score;
                    played++;
                    printf("Round score: %d | Total: %d\n", score, totalScore);
                }
                break;

            case 2:
                if (played == 0) {
                    printf("No rounds played yet.\n");
                } else {
                    sum = 0;
                    best = times[0];
                    worst = times[0];
                    for (i = 0; i < played; i++) {
                        sum += times[i];
                        if (times[i] < best) best = times[i];
                        if (times[i] > worst) worst = times[i];
                    }
                    avg = sum / played;
                    range = worst - best;

                    printf("\n--- SESSION STATS ---\n");
                    printf("Rounds   : %d\n", played);
                    printf("Best     : %.0f ms\n", best);
                    printf("Worst    : %.0f ms\n", worst);
                    printf("Average  : %.0f ms\n", avg);
                    printf("Score    : %d\n", totalScore);

                    if (range < 80) {
                        if (avg < 250) printf("Consistency: locked in\n");
                        else printf("Consistency: steady but slow\n");
                    } else if (range < 200) {
                        printf("Consistency: mostly even\n");
                    } else {
                        if (avg > 500) printf("Consistency: scattered, low focus\n");
                        else printf("Consistency: spiky, attention wandering\n");
                    }
                }
                break;

            case 3:
                played = 0;
                totalScore = 0;
                printf("Session reset.\n");
                break;

            case 4:
                printf("Final score: %d\n", totalScore);
                break;

            default:
                printf("Invalid option.\n");
        }
    } while (choice != 4);

    return 0;
}
