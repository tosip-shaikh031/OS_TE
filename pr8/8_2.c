/* Implement and compare the total head movement for a given sequence of disk requests using two 
specific scheduling algorithms: 
• System Setup: A disk with cylinders numbered 0 to 499. 
• Current Head Position: 185 
• Pending Requests (FIFO order): 20, 229, 39, 450, 18, 145, 120, 380, 20, 250 
• Algorithms to Implement: 
• SCAN (Assume initial movement is towards 499). 
• LOOK (Assume initial movement is towards 499). 
• Output: Calculate and display the sequence of cylinder movements and the Average Seek Distance 
for both SCAN and LOOK. Conclude which algorithm performed better for this request set.*/

#include <stdio.h>
#include <stdlib.h>

#define MAX 20
#define DISK_SIZE 500

// Function to sort requests in ascending order
void sort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++)
        for (int j = i + 1; j < n; j++)
            if (arr[i] > arr[j]) {
                int temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
}

// SCAN Disk Scheduling
// Motion : from current head to 499, then reverse to 0
void scan(int req[], int n, int head) {
    int total = 0;
    sort(req, n);

    int i;
    for (i = 0; i < n; i++)
        if (req[i] > head) break;

    printf("\nSCAN Order: %d", head);

    // Move towards 499 first
    for (int j = i; j < n; j++) {
        total += abs(req[j] - head);
        head = req[j];
        printf(" -> %d", head);
    }

    // Now go till end (499)
    total += (DISK_SIZE - 1 - head);
    head = DISK_SIZE - 1;
    printf(" -> %d", head);

    // Reverse direction and go to remaining
    for (int j = i - 1; j >= 0; j--) {
        total += abs(req[j] - head);
        head = req[j];
        printf(" -> %d", head);
    }

    printf("\nTotal head movement (SCAN): %d", total);
    printf("\nAverage Seek Distance: %.2f\n", (float)total / n);
}

// LOOK Disk Scheduling
// Motion : from current head to highest request, then reverse to lowest request
void look(int req[], int n, int head) {
    int total = 0;
    sort(req, n);

    int i;
    for (i = 0; i < n; i++)
        if (req[i] > head) break;

    printf("\nLOOK Order: %d", head);

    // Move towards highest request
    for (int j = i; j < n; j++) {
        total += abs(req[j] - head);
        head = req[j];
        printf(" -> %d", head);
    }

    // Reverse direction but no move till end
    for (int j = i - 1; j >= 0; j--) {
        total += abs(req[j] - head);
        head = req[j];
        printf(" -> %d", head);
    }

    printf("\nTotal head movement (LOOK): %d", total);
    printf("\nAverage Seek Distance: %.2f\n", (float)total / n);
}

int main() {
    int n = 10;
    int requests[] = {20, 229, 39, 450, 18, 145, 120, 380, 20, 250};
    int head = 185;

    printf("Initial Head Position: %d\n", head);
    printf("Disk Size: 0 to %d\n", DISK_SIZE - 1);

    scan(requests, n, head);
    look(requests, n, head);

    printf("\nConclusion: LOOK avoids unnecessary movement to end of disk,");
    printf("\nso it generally gives less total head movement than SCAN.\n");

    return 0;
}

// Out of look scan cscan clook arrange with efficiency
// order with total movement : 