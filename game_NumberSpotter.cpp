#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void instructions(void) {
    printf("\n--- HOW TO PLAY ---\n");
    printf("1. A list of numbers shows up. One number is your TARGET.\n");
    printf("2. Type the target's position in the list (1st number = 1, 2nd = 2, etc.)\n");
    printf("3. Stuck? Type 0 for a free hint. It won't be counted as wrong.\n");
    printf("4. Too many wrong guesses = round over.\n");
    printf("5. Answer fast and without hints to earn bonus points.\n");
    printf("6. Each round the list gets a bit bigger.\n");
    printf("-------------------\n\n");
}

int calcPoints(double t) {
    int p = 100 - (int)(t * 5);
    if (p < 10) p = 10;
    return p;
}

int contains(int *arr, int n, int val) {
    for (int i = 0; i < n; i++)
        if (arr[i] == val) return 1;
    return 0;
}

void fillUnique(int *arr, int n) {
    for (int i = 0; i < n; i++) {
        int v;
        do {
            v = rand() % 1000;
        } while (contains(arr, i, v));
        arr[i] = v;
    }
}

void playGame(int diff) {
    srand((unsigned)time(NULL));

    int startSize[3] = {10, 15, 20};
    int step[3]       = {3, 4, 5};
    int maxSize[3]    = {25, 35, 45};
    int maxTries[3]   = {4, 3, 2};

    int size = startSize[diff - 1];
    int round = 1, totalScore = 0, streak = 0, bestStreak = 0;

    while (size <= maxSize[diff - 1]) {
        int *arr = (int *)malloc(size * sizeof(int));
        fillUnique(arr, size);

        int idx = rand() % size;
        int target = arr[idx];

        printf("\nRound %d (size %d)\n", round, size);
        printf("List: ");
        for (int i = 0; i < size; i++)
            printf("%d ", arr[i]);
        printf("\nFind position of: %d  (0 = hint)\n", target);

        time_t start = time(NULL);
        int tries = 0, correct = 0, pos, usedHint = 0, firstGuess = 1;

        while (tries < maxTries[diff - 1]) {
            printf("Enter position: ");
            if (scanf("%d", &pos) != 1) {
                while (getchar() != '\n');
                continue;
            }

            if (pos == 0) {
                usedHint = 1;
                int mid = size / 2;
                if (idx < mid) printf("Hint: it's in the FIRST half of the list.\n");
                else printf("Hint: it's in the SECOND half of the list.\n");
                continue;
            }

            if (pos >= 1 && pos <= size && arr[pos - 1] == target) {
                correct = 1;
                break;
            }
            tries++;
            firstGuess = 0;
            printf("Wrong! Tries left: %d\n", maxTries[diff - 1] - tries);
        }

        if (!correct) {
            printf("\n%d wrong tries. GAME OVER.\n", maxTries[diff - 1]);
            printf("Final Score: %d | Best streak: %d\n", totalScore, bestStreak);
            free(arr);
            return;
        }

        double elapsed = difftime(time(NULL), start);
        int pts = calcPoints(elapsed);

        if (!usedHint && firstGuess) {
            streak++;
            if (streak >= 3) {
                int bonus = 15 * (streak - 2);
                pts += bonus;
                printf("Combo x%d! Bonus +%d\n", streak, bonus);
            }
        } else {
            streak = 0;
        }
        if (streak > bestStreak) bestStreak = streak;

        totalScore += pts;
        printf("Correct! Time: %.1f sec | Points: %d | Total: %d\n", elapsed, pts, totalScore);

        free(arr);
        round++;
        size += step[diff - 1];
    }

    printf("\nYou cleared all rounds!\nFinal Score: %d | Best streak: %d\n", totalScore, bestStreak);
}

int main(void) {
    int choice, diff = 2;
    do {
        printf("\n===== NUMBER SPOTTER =====\n");
        printf("1. Start Game\n");
        printf("2. Instructions\n");
        printf("3. Change difficulty (now: %s)\n", diff == 1 ? "Easy" : diff == 2 ? "Normal" : "Hard");
        printf("4. Exit\n");
        printf("Choice: ");
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
            case 1: playGame(diff); break;
            case 2: instructions(); break;
            case 3:
                printf("Select difficulty - 1.Easy 2.Normal 3.Hard: ");
                if (scanf("%d", &diff) != 1 || diff < 1 || diff > 3) diff = 2;
                printf("Difficulty set.\n");
                break;
            case 4: printf("Bye!\n"); break;
            default: printf("Invalid choice.\n");
        }
    } while (choice != 4);

    return 0;
}
