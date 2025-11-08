// ASSIGNMENT NO. : 6.1
// ROLL NO. : 33127
// FIFO (FCFS) and LRU Page Replacement Algorithms with random page reference string

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 50

// Function to display current frame status
void displayFrames(int frames[], int frameSize) {
    printf("\n-------------\n|");
    for (int i = 0; i < frameSize; i++) {
        if (frames[i] == -1)
            printf("   |");
        else
            printf(" %d |", frames[i]);
    }
    printf("\n-------------\n");
}

// FIFO Algorithm
void fifo(int pages[], int n, int frameSize) {
    int frames[frameSize], front = 0, faults = 0, hits = 0;

    for (int i = 0; i < frameSize; i++)
        frames[i] = -1;

    printf("\n--- FIFO (FCFS) Page Replacement ---\n");

    for (int i = 0; i < n; i++) {
        int page = pages[i], flag = 0;

        for (int j = 0; j < frameSize; j++) {
            if (frames[j] == page) {
                hits++;
                flag = 1;
                printf("Page %d → Hit", page);
                break;
            }
        }

        if (!flag) {
            frames[front] = page;
            front = (front + 1) % frameSize;
            faults++;
            printf("Page %d → Miss", page);
        }

        displayFrames(frames, frameSize);
    }

    printf("\nTotal Page Faults: %d", faults);
    printf("\nTotal Page Hits: %d", hits);
    printf("\nHit Ratio = %.2f", (float)hits / n);
    printf("\nMiss Ratio = %.2f", (float)faults / n);
    printf("\nHit %% = %.2f%%", ((float)hits / n) * 100);
    printf("\nMiss %% = %.2f%%\n", ((float)faults / n) * 100);
}

// LRU Algorithm
void lru(int pages[], int n, int frameSize) {
    int frames[frameSize], recent[frameSize], time = 0, faults = 0, hits = 0;

    for (int i = 0; i < frameSize; i++) {
        frames[i] = -1;
        recent[i] = 0;
    }

    printf("\n--- LRU Page Replacement ---\n");

    for (int i = 0; i < n; i++) {
        int page = pages[i], flag = 0;

        for (int j = 0; j < frameSize; j++) {
            if (frames[j] == page) {
                hits++;
                recent[j] = ++time;
                flag = 1;
                printf("Page %d → Hit", page);
                break;
            }
        }

        if (!flag) {
            int minIndex = 0, min = recent[0];
            for (int j = 1; j < frameSize; j++) {
                if (recent[j] < min) {
                    min = recent[j];
                    minIndex = j;
                }
            }
            frames[minIndex] = page;
            recent[minIndex] = ++time;
            faults++;
            printf("Page %d → Miss", page);
        }

        displayFrames(frames, frameSize);
    }

    printf("\nTotal Page Faults: %d", faults);
    printf("\nTotal Page Hits: %d", hits);
    printf("\nHit Ratio = %.2f", (float)hits / n);
    printf("\nMiss Ratio = %.2f", (float)faults / n);
    printf("\nHit %% = %.2f%%", ((float)hits / n) * 100);
    printf("\nMiss %% = %.2f%%\n", ((float)faults / n) * 100);
}

// Main Function
int main() {
    int n, frameSize, choice;
    int pages[MAX];

    srand(time(0));

    printf("Enter number of pages (e.g., 10–20): ");
    scanf("%d", &n);

    printf("Generated Random Page Reference String:\n");
    for (int i = 0; i < n; i++) {
        pages[i] = rand() % 10; // random 0–9
        printf("%d ", pages[i]);
    }

    printf("\nEnter Frame Size (1–7): ");
    scanf("%d", &frameSize);

    do {
        printf("\n===== PAGE REPLACEMENT MENU =====");
        printf("\n1. FIFO");
        printf("\n2. LRU");
        printf("\n3. Exit");
        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            fifo(pages, n, frameSize);
            break;
        case 2:
            lru(pages, n, frameSize);
            break;
        case 3:
            printf("Exiting...\n");
            exit(0);
        default:
            printf("Invalid choice! Try again.\n");
        }
    } while (choice != 3);

    return 0;
}