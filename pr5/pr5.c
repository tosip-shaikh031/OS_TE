//Problem Statement : 5.1 Develop a C program to implement Banker’s algorithm. Assume suitable input required to demonstrate 
//the results. Using the banker’s algorithm, determine whether or not the state is unsafe. If the state is safe, 
//illustrate the order in which the processes may complete. Otherwise, illustrate why the state is unsafe.




#include <stdio.h>
#define N 5   // number of processes
#define M 3   // number of resources

int alloc[N][M];  // Allocation Matrix
int max[N][M];    // Maximum Need Matrix
int avail[M];     // Available Resources
int need[N][M];   // Need Matrix

// Function to calculate Need matrix = Max - Allocation
void calculateNeed() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            need[i][j] = max[i][j] - alloc[i][j];
        }
    }
}

// Function to check if system is in safe state
int isSafe() {
    int finish[N] = {0};
    int safeSeq[N];
    int work[M];

    for (int i = 0; i < M; i++)
        work[i] = avail[i];  // initially available

    int count = 0;

    while (count < N) {
        int found = 0;
        for (int p = 0; p < N; p++) {
            if (finish[p] == 0) {
                int canAllocate = 1;
                for (int j = 0; j < M; j++) {
                    if (need[p][j] > work[j]) {
                        canAllocate = 0;
                        break;
                    }
                }

                if (canAllocate) {
                    for (int j = 0; j < M; j++)
                        work[j] += alloc[p][j];
                    safeSeq[count++] = p;
                    finish[p] = 1;
                    found = 1;
                }
            }
        }

        if (!found) {
            printf("\nSystem is NOT in a Safe State!\n");
            return 0;
        }
    }

    printf("\nSystem is in a Safe State.\nSafe Sequence: ");
    for (int i = 0; i < N; i++)
        printf("P%d ", safeSeq[i]);
    printf("\n");
    return 1;
}

// Function to handle a resource request from a process
void resourceRequest() {
    int req[M], p;
    printf("Enter process number (0 to %d): ", N - 1);
    scanf("%d", &p);

    printf("Enter request for %d resources: ", M);
    for (int i = 0; i < M; i++)
        scanf("%d", &req[i]);

    // Step 1: Check if request ≤ need
    for (int i = 0; i < M; i++) {
        if (req[i] > need[p][i]) {
            printf("Error: Process has exceeded its maximum claim!\n");
            return;
        }
    }

    // Step 2: Check if request ≤ available
    for (int i = 0; i < M; i++) {
        if (req[i] > avail[i]) {
            printf("Resources not available, process must wait.\n");
            return;
        }
    }

    // Step 3: Pretend to allocate and check for safety
    for (int i = 0; i < M; i++) {
        avail[i] -= req[i];
        alloc[p][i] += req[i];
        need[p][i] -= req[i];
    }

    if (isSafe()) {
        printf("Request can be granted safely.\n");
    } else {
        printf("Request CANNOT be granted safely — rolling back.\n");
        for (int i = 0; i < M; i++) {  // rollback changes
            avail[i] += req[i];
            alloc[p][i] -= req[i];
            need[p][i] += req[i];
        }
    }
}

int main() {
    printf("\n--- BANKER'S ALGORITHM ---\n");

    printf("\nEnter Allocation Matrix (%d x %d):\n", N, M);
    for (int i = 0; i < N; i++)
        for (int j = 0; j < M; j++)
            scanf("%d", &alloc[i][j]);

    printf("\nEnter Maximum Matrix (%d x %d):\n", N, M);
    for (int i = 0; i < N; i++)
        for (int j = 0; j < M; j++)
            scanf("%d", &max[i][j]);

    printf("\nEnter Available Resources for %d types: ", M);
    for (int i = 0; i < M; i++)
        scanf("%d", &avail[i]);

    calculateNeed();

    int choice;
    while (1) {
        printf("\n\n--- BANKER'S ALGORITHM MENU ---");
        printf("\n1. Display Need Matrix");
        printf("\n2. Check for Safe State");
        printf("\n3. Make a Resource Request");
        printf("\n4. Exit");
        printf("\nEnter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("\nNeed Matrix:\n");
                for (int i = 0; i < N; i++) {
                    for (int j = 0; j < M; j++)
                        printf("%d ", need[i][j]);
                    printf("\n");
                }
                break;
            case 2:
                isSafe();
                break;
            case 3:
                resourceRequest();
                break;
            case 4:
                return 0;
            default:
                printf("Invalid choice!\n");
        }
    }
}
