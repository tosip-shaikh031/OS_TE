/*8.1 Implement and compare the total head movement for a given sequence of disk requests using two 
specific scheduling algorithms: 
• System Setup: A disk with cylinders numbered 0 to 499. 
• Current Head Position: 85 
• Pending Requests (FIFO order): 10, 229, 39, 400, 18, 145, 120, 480, 20, 250 
• Algorithms to Implement: 
• C-SCAN (Assume initial movement is towards 499). 
• C-LOOK (Assume initial movement is towards 499). 
• Output: Calculate and display the sequence of cylinder movements and the Average Seek Distance 
for both C-SCAN and C-LOOK. Conclude which algorithm performed better for this request set. */


#include <stdio.h>
#include <stdlib.h>

#define DISK_SIZE 500

// Function to sort the request array
void sort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

// Function for C-SCAN scheduling
int cscan(int requests[], int n, int head) {
    int total_movement = 0;
    int i, pos;

    sort(requests, n);

    // find position of head
    for (i = 0; i < n; i++) {
        if (requests[i] > head) {
            pos = i;
            break;
        }
    }

    printf("\nC-SCAN Order: %d", head);

    // move towards 499 first
    for (i = pos; i < n; i++) {
        printf(" -> %d", requests[i]);
        total_movement += abs(requests[i] - head);
        head = requests[i];
    }

    // jump from 499 to 0 (circular move)
    printf(" -> %d", DISK_SIZE - 1);
    total_movement += (DISK_SIZE - 1 - head); // move to 499 from current head
    total_movement += (DISK_SIZE - 1);        // jump 499→0
    printf(" -> %d", 0);

    head = 0;

    // continue serving remaining requests
    for (i = 0; i < pos; i++) {
        printf(" -> %d", requests[i]);
        total_movement += abs(requests[i] - head);
        head = requests[i];
    }

    printf("\nTotal Head Movement (C-SCAN): %d", total_movement);
    printf("\nAverage Seek Distance (C-SCAN): %.2f\n", (float)total_movement / n);

    return total_movement;
}

// Function for C-LOOK scheduling
int clook(int requests[], int n, int head) {
    int total_movement = 0;
    int i, pos;

    sort(requests, n);

    // find position of head
    for (i = 0; i < n; i++) {
        if (requests[i] > head) {
            pos = i;
            break;
        }
    }

    printf("\nC-LOOK Order: %d", head);

    // move towards higher requests
    for (i = pos; i < n; i++) {
        printf(" -> %d", requests[i]);
        total_movement += abs(requests[i] - head);
        head = requests[i];
    }

    // jump directly to the lowest request (not to 0)
    if (pos > 0) {
        total_movement += abs(head - requests[0]);
        head = requests[0];
    }

    // now service remaining requests
    for (i = 0; i < pos; i++) {
        printf(" -> %d", requests[i]);
        total_movement += abs(requests[i] - head);
        head = requests[i];
    }

    printf("\nTotal Head Movement (C-LOOK): %d", total_movement);
    printf("\nAverage Seek Distance (C-LOOK): %.2f\n", (float)total_movement / n);

    return total_movement;
}

int main() {
    int requests[] = {10, 229, 39, 400, 18, 145, 120, 480, 20, 250};
    int n = sizeof(requests) / sizeof(requests[0]);
    int head = 85;

    printf("Disk Scheduling Comparison\n");
    printf("---------------------------\n");
    printf("Head starts at: %d\n", head);
    printf("Requests: ");
    for (int i = 0; i < n; i++) printf("%d ", requests[i]);
    printf("\nDisk Range: 0–499\n");

    int total_cscan = cscan(requests, n, head);
    int total_clook = clook(requests, n, head);

    printf("\n--- Comparison ---\n");
    printf("Total Movement (C-SCAN): %d\n", total_cscan);
    printf("Total Movement (C-LOOK): %d\n", total_clook);

    if (total_cscan < total_clook)
        printf("=> C-SCAN performs better (less head movement).\n");
    else if (total_clook < total_cscan)
        printf("=> C-LOOK performs better (less head movement).\n");
    else
        printf("=> Both perform equally well for this request set.\n");

    return 0;
}
