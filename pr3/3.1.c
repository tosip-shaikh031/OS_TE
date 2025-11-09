#include <stdio.h>
#include <stdlib.h>

struct Process {
    int pid;           // Process ID
    int arrival_time;  // Arrival time
    int burst_time;    // Burst time
    int completion_time;
    int waiting_time;
    int turnaround_time;
    int visited;       // For SJF
};

void fcfs(struct Process p[], int n);
void sjf(struct Process p[], int n);
void calculate_and_display(struct Process p[], int n, char algorithm[]);

// -------------------- MAIN FUNCTION --------------------
int main() {
    int n, choice;
    struct Process p[20];

    printf("Enter number of processes: ");
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        p[i].pid = i + 1;
        printf("\nEnter Arrival Time and Burst Time for Process P%d: ", p[i].pid);
        scanf("%d %d", &p[i].arrival_time, &p[i].burst_time);
        p[i].visited = 0;
    }

    while (1) {
        printf("\n-------------------- MENU --------------------");
        printf("\n1. FCFS Scheduling");
        printf("\n2. SJF (Non-Preemptive) Scheduling");
        printf("\n3. Exit");
        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                fcfs(p, n);
                break;
            case 2:
                sjf(p, n);
                break;
            case 3:
                printf("Exiting program...\n");
                exit(0);
            default:
                printf("Invalid choice. Try again.\n");
        }
    }

    return 0;
}

// -------------------- FCFS FUNCTION --------------------
void fcfs(struct Process p[], int n) {
    struct Process temp;
    int i, j;
    float total_wait = 0, total_tat = 0;

    // Sort by arrival time
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (p[j].arrival_time > p[j + 1].arrival_time) {
                temp = p[j];
                p[j] = p[j + 1];
                p[j + 1] = temp;
            }
        }
    }

    int time = 0;
    for (i = 0; i < n; i++) {
        if (time < p[i].arrival_time)
            time = p[i].arrival_time;
        time += p[i].burst_time;
        p[i].completion_time = time;
        p[i].turnaround_time = p[i].completion_time - p[i].arrival_time;
        p[i].waiting_time = p[i].turnaround_time - p[i].burst_time;
        total_wait += p[i].waiting_time;
        total_tat += p[i].turnaround_time;
    }

    calculate_and_display(p, n, "FCFS");
}

// -------------------- SJF FUNCTION --------------------
void sjf(struct Process p[], int n) {
    int completed = 0, time = 0, i, j;
    float total_wait = 0, total_tat = 0;

    // Reset visited flags
    for (i = 0; i < n; i++) p[i].visited = 0;

    while (completed < n) {
        int idx = -1;
        int min_bt = 9999;

        for (i = 0; i < n; i++) {
            if (p[i].arrival_time <= time && p[i].visited == 0) {
                if (p[i].burst_time < min_bt) {
                    min_bt = p[i].burst_time;
                    idx = i;
                }
            }
        }

        if (idx == -1) {
            time++;
            continue;
        }

        p[idx].visited = 1;
        time += p[idx].burst_time;
        p[idx].completion_time = time;
        p[idx].turnaround_time = p[idx].completion_time - p[idx].arrival_time;
        p[idx].waiting_time = p[idx].turnaround_time - p[idx].burst_time;

        total_wait += p[idx].waiting_time;
        total_tat += p[idx].turnaround_time;
        completed++;
    }

    calculate_and_display(p, n, "SJF (Non-Preemptive)");
}

// -------------------- DISPLAY FUNCTION --------------------
void calculate_and_display(struct Process p[], int n, char algorithm[]) {
    float total_wait = 0, total_tat = 0;

    printf("\n\n=== %s Scheduling ===\n", algorithm);
    printf("-------------------------------------------------\n");
    printf("PID\tAT\tBT\tCT\tTAT\tWT\n");
    printf("-------------------------------------------------\n");

    for (int i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\n", p[i].pid, p[i].arrival_time,
               p[i].burst_time, p[i].completion_time,
               p[i].turnaround_time, p[i].waiting_time);

        total_wait += p[i].waiting_time;
        total_tat += p[i].turnaround_time;
    }

    printf("-------------------------------------------------\n");
    printf("Average Waiting Time: %.2f\n", total_wait / n);
    printf("Average Turnaround Time: %.2f\n", total_tat / n);

    // Gantt Chart
    printf("\nGANTT CHART:\n");
    printf("-------------------------------------------------\n| ");
    for (int i = 0; i < n; i++) {
        printf("P%d | ", p[i].pid);
    }
    printf("\n-------------------------------------------------\n");

    int start = 0;
    printf("0");
    for (int i = 0; i < n; i++) {
        printf("\t%d", p[i].completion_time);
    }
    printf("\n");
}
