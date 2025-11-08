#include <stdio.h>
#include <stdlib.h>
#define MAX 10
int n, m;
int alloc[MAX][MAX], maxm[MAX][MAX], avail[MAX];
int need[MAX][MAX];
int finish[MAX], safeSeq[MAX];
void calculateNeed() {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            need[i][j] = maxm[i][j] - alloc[i][j];
}
int isSafe() {
    int work[MAX];
    for (int i = 0; i < m; i++)
        work[i] = avail[i];
    for (int i = 0; i < n; i++)
        finish[i] = 0;
    int count = 0;
    while (count < n) {
        int found = 0;
        for (int i = 0; i < n; i++) {
            if (finish[i] == 0) {
                int j;
                for (j = 0; j < m; j++)
                    if (need[i][j] > work[j])
                        break;

                if (j == m) {
                    for (int k = 0; k < m; k++)
                        work[k] += alloc[i][k];
                    safeSeq[count++] = i;
                    finish[i] = 1;
                    found = 1;
                }
            }
        }
        if (found == 0)
            break;
    }
    if (count == n) {
        printf("\nThe system is in a SAFE STATE.\nSafe sequence: ");
        for (int i = 0; i < n; i++)
            printf("P%d ", safeSeq[i]);
        printf("\n");
        return 1;
    } else {
        printf("\nThe system is in an UNSAFE STATE.\n");
        return 0;
    }
}
void requestResources() {
    int p, req[MAX];
    printf("\nEnter process number making request (0 to %d): ", n - 1);
    scanf("%d", &p);
    printf("Enter request for %d resources: ", m);
    for (int i = 0; i < m; i++)
        scanf("%d", &req[i]);
    for (int i = 0; i < m; i++) {
        if (req[i] > need[p][i]) {
            printf("\nError: Process has exceeded its maximum claim.\n");
            return;
        }
    }
    for (int i = 0; i < m; i++) {
        if (req[i] > avail[i]) {
            printf("\nResources not available currently. Process must wait.\n");
            return;
        }
    }
    for (int i = 0; i < m; i++) {
        avail[i] -= req[i];
        alloc[p][i] += req[i];
        need[p][i] -= req[i];
    }
    if (isSafe()) {
        printf("Request can be GRANTED safely.\n");
    } else {
        printf("Request CANNOT be granted (would lead to UNSAFE state).\n");
        for (int i = 0; i < m; i++) {
            avail[i] += req[i];
            alloc[p][i] -= req[i];
            need[p][i] += req[i];
        }
    }
}
void readDataFromFile() {
    FILE *fp = fopen("state.txt", "r");
    if (fp == NULL) {
        perror("Error opening file");
        exit(1);
    }
    fscanf(fp, "Number of Processes %d", &n);
    fscanf(fp, " Number of Resources %d", &m);
    printf("\nReading data for %d processes and %d resources...\n", n, m);
    printf("\nEnter Allocation Matrix:\n");
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            fscanf(fp, "%d", &alloc[i][j]);
    printf("Enter Max Matrix:\n");
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            fscanf(fp, "%d", &maxm[i][j]);
    printf("Enter Available Resources:\n");
    for (int j = 0; j < m; j++)
        fscanf(fp, "%d", &avail[j]);
    fclose(fp);
}
int main() {
    readDataFromFile();
    calculateNeed();
    printf("\n--- Allocation Matrix ---\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++)
            printf("%d ", alloc[i][j]);
        printf("\n");
    }
    printf("\n--- Max Matrix ---\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++)
            printf("%d ", maxm[i][j]);
        printf("\n");
    }
    printf("\n--- Available ---\n");
    for (int j = 0; j < m; j++)
        printf("%d ", avail[j]);
    printf("\n");
    calculateNeed();
    printf("\n--- Need Matrix ---\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++)
            printf("%d ", need[i][j]);
        printf("\n");
    }
    isSafe();
    requestResources();
    return 0;
}