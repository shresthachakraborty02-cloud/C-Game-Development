#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void instructions() {
    printf("\n--- INSTRUCTIONS ---\n");
    printf("An array of random numbers (0-99) is shown.\n");
    printf("You are given a number to find in the array.\n");
    printf("Enter its POSITION (1-based index).\n");
    printf("You get 3 wrong tries per round. 3rd wrong try ends the game.\n");
    printf("Faster you find it, more points you get.\n");
    printf("Array size increases by 2 each round (starts at 10, max 20).\n");
    printf("---------------------\n\n");
}

int calcPoints(double t) {
    int p = 100 - (int)(t * 5);
    if (p < 10) p = 10;
    return p;
}

void playGame() {
    srand(time(NULL));
    int size = 10, round = 1, totalScore = 0;

    while (size <= 20) {
        int arr[20];
        for (int i = 0; i < size; i++)
            arr[i] = rand() % 100;

        int idx = rand() % size;
        int target = arr[idx];

        printf("\nRound %d (size %d)\n", round, size);
        printf("Array: ");
        for (int i = 0; i < size; i++)
            printf("%d ", arr[i]);
        printf("\nFind position of: %d\n", target);

        time_t start = time(NULL);
        int tries = 0, correct = 0, pos;

        while (tries < 3) {
            printf("Enter position: ");
            scanf("%d", &pos);

            if (pos >= 1 && pos <= size && arr[pos - 1] == target) {
                correct = 1;
                break;
            }
            tries++;
            printf("Wrong! Tries left: %d\n", 3 - tries);
        }

        if (!correct) {
            printf("\n3 wrong tries. GAME OVER.\n");
            printf("Final Score: %d\n", totalScore);
            return;
        }

        double elapsed = difftime(time(NULL), start);
        int pts = calcPoints(elapsed);
        totalScore += pts;

        printf("Correct! Time: %.1f sec | Points: %d | Total: %d\n", elapsed, pts, totalScore);

        round++;
        size += 2;
    }

    printf("\nYou cleared all rounds!\nFinal Score: %d\n", totalScore);
}

int main() {
    int choice;
    do {
        printf("\n===== NUMBER SPOTTER =====\n");
        printf("1. Start Game\n2. Instructions\n3. Exit\n");
        printf("Choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: playGame(); break;
            case 2: instructions(); break;
            case 3: printf("Bye!\n"); break;
            default: printf("Invalid choice.\n");
        }
    } while (choice != 3);

    return 0;
}
