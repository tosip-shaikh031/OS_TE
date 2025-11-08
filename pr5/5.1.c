#include <stdio.h>
int main() {
    int n, m, i, j, k;
    printf("Enter number of processes: ");
    scanf("%d", &n);
    printf("Enter number of resource types: ");
    scanf("%d", &m);
    int alloc[n][m], max[n][m], avail[m];
    int need[n][m], finish[n], safeSeq[n];
    printf("\nEnter Allocation Matrix:\n");
    for (i = 0; i < n; i++)
        for (j = 0; j < m; j++)
            scanf("%d", &alloc[i][j]);
    printf("\nEnter Max Matrix:\n");
    for (i = 0; i < n; i++)
        for (j = 0; j < m; j++)
            scanf("%d", &max[i][j]);
    printf("\nEnter Available Resources:\n");
    for (j = 0; j < m; j++)
        scanf("%d", &avail[j]);
    for (i = 0; i < n; i++)
        for (j = 0; j < m; j++)
            need[i][j] = max[i][j] - alloc[i][j];
    printf("\nNeed Matrix:\n");
    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++)
            printf("%d ", need[i][j]);
        printf("\n");
    }
    for (i = 0; i < n; i++) finish[i] = 0;
    int count = 0;
    int work[m];
    for (i = 0; i < m; i++)
        work[i] = avail[i];
    while (count < n) {
        int found = 0;
        for (i = 0; i < n; i++) {
            if (finish[i] == 0) {
                int canExecute = 1;
                for (j = 0; j < m; j++) {
                    if (need[i][j] > work[j]) {
                        canExecute = 0;
                        break;
                    }
                }
                if (canExecute) {
                    for (k = 0; k < m; k++)
                        work[k] += alloc[i][k];
                    safeSeq[count++] = i;
                    finish[i] = 1;
                    found = 1;
                }
            }
        }
        if (found == 0) {
            break;
        }
    }
    int flag = 1;
    for (i = 0; i < n; i++) {
        if (finish[i] == 0) {
            flag = 0;
            break;
        }
    }
    if (flag == 1) {
        printf("\nThe system is in a SAFE STATE.\nSafe sequence: ");
        for (i = 0; i < n; i++)
            printf("P%d ", safeSeq[i]);
        printf("\n");
    } else {
        printf("\nThe system is in an UNSAFE STATE.\n");
        printf("Processes that could not complete: ");
        for (i = 0; i < n; i++)
            if (finish[i] == 0)
                printf("P%d ", i);
        printf("\n");
    }
    return 0;
}