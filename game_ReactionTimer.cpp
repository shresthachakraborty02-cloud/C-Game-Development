#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void clearInput(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void waitRandom(int minMs, int maxMs) {
    int waitMs = minMs + rand() % (maxMs - minMs);
    clock_t startTime = clock();
    while (((clock() - startTime) * 1000 / CLOCKS_PER_SEC) < waitMs);
}

int main(void) {
    int choice, rounds = 0, i, played = 0, streak = 0, bestStreak = 0;
    int score, totalScore = 0, diff = 2;
    int minWait[3] = {2000, 1000, 400};
    int maxWait[3] = {4000, 3000, 1500};
    int lightning[3] = {200, 150, 100};
    int sharp[3]     = {400, 300, 200};
    int steady[3]    = {650, 500, 350};
    int slow[3]      = {1000, 900, 600};
    double ms, times[20], best, worst, sum, avg, range;
    clock_t t1, t2;

    srand((unsigned)time(NULL));

    do {
        printf("\n===== REACTION TIMER =====\n");
        printf("1. Play rounds\n");
        printf("2. View stats\n");
        printf("3. Change difficulty (now: %s)\n", diff == 1 ? "Easy" : diff == 2 ? "Normal" : "Hard");
        printf("4. Reset session\n");
        printf("5. Exit\n");
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
                    waitRandom(minWait[diff - 1], maxWait[diff - 1]);

                    printf(">>> PRESS ENTER NOW! <<<\n");
                    fflush(stdout);

                    t1 = clock();
                    clearInput();
                    t2 = clock();

                    ms = (double)(t2 - t1) * 1000.0 / CLOCKS_PER_SEC;
                    times[played] = ms;

                    if (ms < lightning[diff - 1]) {
                        score = 120;
                        printf("Lightning fast! %.0f ms\n", ms);
                        streak++;
                    } else if (ms < sharp[diff - 1]) {
                        score = 80;
                        printf("Sharp. %.0f ms\n", ms);
                        streak++;
                    } else if (ms < steady[diff - 1]) {
                        score = 45;
                        printf("Steady. %.0f ms\n", ms);
                        streak = 0;
                    } else if (ms < slow[diff - 1]) {
                        score = 15;
                        printf("A bit slow. %.0f ms\n", ms);
                        streak = 0;
                    } else {
                        score = 5;
                        printf("Drifted off? %.0f ms\n", ms);
                        streak = 0;
                    }

                    if (streak >= 3) {
                        int bonus = 20;
                        score += bonus;
                        printf("Streak x%d! Bonus +%d\n", streak, bonus);
                    }
                    if (streak > bestStreak) bestStreak = streak;

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
                    printf("Rounds       : %d\n", played);
                    printf("Best         : %.0f ms\n", best);
                    printf("Worst        : %.0f ms\n", worst);
                    printf("Average      : %.0f ms\n", avg);
                    printf("Best streak  : %d\n", bestStreak);
                    printf("Score        : %d\n", totalScore);

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
                printf("Select difficulty - 1.Easy 2.Normal 3.Hard: ");
                if (scanf("%d", &diff) != 1 || diff < 1 || diff > 3) diff = 2;
                clearInput();
                printf("Difficulty set. Hard = shorter wait + tighter timing to score well. Easy = longer wait + looser timing.\n");
                break;

            case 4:
                played = 0;
                totalScore = 0;
                streak = 0;
                bestStreak = 0;
                printf("Session reset.\n");
                break;

            case 5:
                printf("Final score: %d\n", totalScore);
                break;

            default:
                printf("Invalid option.\n");
        }
    } while (choice != 5);

    return 0;
}
