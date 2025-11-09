#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

typedef struct {
    int PID, AT, BT, CT, TAT, WT, Priority;
} Process;

void swap(Process *a, Process *b) {
    Process temp = *a;
    *a = *b;
    *b = temp;
}

void sortByArrival(Process p[], int n) {
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (p[j].AT > p[j + 1].AT) swap(&p[j], &p[j + 1]);
}

void sortByPID(Process p[], int n) {
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (p[j].PID > p[j + 1].PID) swap(&p[j], &p[j + 1]);
}

void printTable(Process p[], int n) {
    printf("\n---------------------------------------------------\n");
    printf("PID\tAT\tBT\tPR\tCT\tTAT\tWT\n");
    printf("---------------------------------------------------\n");
    float sumTAT = 0, sumWT = 0;
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\n", 
            p[i].PID, p[i].AT, p[i].BT, p[i].Priority, p[i].CT, p[i].TAT, p[i].WT);
        sumTAT += p[i].TAT;
        sumWT += p[i].WT;
    }
    printf("---------------------------------------------------\n");
    printf("Average TAT: %.2f\n", sumTAT / n);
    printf("Average WT : %.2f\n", sumWT / n);
}

void printGanttChart(int pid[], int start[], int count, int endTime) {
    printf("\nGantt Chart:\n");

    for (int i = 0; i < count; i++) printf("------");
    printf("-\n");

    for (int i = 0; i < count; i++) printf("| P%-2d ", pid[i]);
    printf("|\n");

    for (int i = 0; i < count; i++) printf("------");
    printf("-\n");

    printf("%-4d", start[0]);
    for (int i = 1; i < count; i++) printf(" %-5d", start[i]);
    printf(" %-4d\n", endTime);
}

// FCFS
void fcfs(Process p[], int n) {
    sortByArrival(p, n);
    int time = 0;
    int ganttPid[n], ganttStart[n], gIndex = 0;

    for (int i = 0; i < n; i++) {
        if (time < p[i].AT) time = p[i].AT;
        ganttPid[gIndex] = p[i].PID; 
        ganttStart[gIndex++] = time;

        time += p[i].BT;
        p[i].CT = time;
        p[i].TAT = p[i].CT - p[i].AT;
        p[i].WT = p[i].TAT - p[i].BT;
    }

    printGanttChart(ganttPid, ganttStart, gIndex, p[n - 1].CT);
    sortByPID(p, n);
    printTable(p, n);
}

// SJF Non-Preemptive
void sjfNonPreemptive(Process p[], int n) {
    int completed = 0, time = 0;
    bool done[n];
    for (int i = 0; i < n; i++) done[i] = false;

    int ganttPid[n], ganttStart[n], gIndex = 0;

    while (completed < n) {
        int idx = -1, minBT = INT_MAX;
        for (int i = 0; i < n; i++) {
            if (p[i].AT <= time && !done[i] && p[i].BT < minBT) {
                minBT = p[i].BT;
                idx = i; 
            }
        }
        if (idx == -1) { time++; continue; }

        ganttPid[gIndex] = p[idx].PID; 
        ganttStart[gIndex++] = time;

        time += p[idx].BT; 
        p[idx].CT = time;
        p[idx].TAT = p[idx].CT - p[idx].AT;
        p[idx].WT = p[idx].TAT - p[idx].BT;
        done[idx] = true;
        completed++;
    }

    printGanttChart(ganttPid, ganttStart, gIndex, time);
    sortByPID(p, n);
    printTable(p, n);
}

// SJF Preemptive
void sjfPreemptive(Process p[], int n) {
    int rem[n], completed = 0, time = 0, prev = -1;
    bool done[n];
    for (int i = 0; i < n; i++) { rem[i] = p[i].BT; done[i] = false; }

    int ganttPid[n * 20], ganttStart[n * 20], gIndex = 0;

    while (completed < n) { 
        int idx = -1, min = INT_MAX;
        for (int i = 0; i < n; i++) {
            if (p[i].AT <= time && !done[i] && rem[i] < min && rem[i] > 0) {
                min = rem[i]; idx = i;
            }
        }
        if (idx == -1) { time++; continue; }

        if (prev != idx) {
            ganttStart[gIndex] = time;
            ganttPid[gIndex++] = p[idx].PID;
            prev = idx;
        }

        rem[idx]--; time++;
        if (rem[idx] == 0) {
            p[idx].CT = time;
            p[idx].TAT = p[idx].CT - p[idx].AT;
            p[idx].WT = p[idx].TAT - p[idx].BT;
            done[idx] = true;
            completed++;
        }
    }

    printGanttChart(ganttPid, ganttStart, gIndex, time);
    sortByPID(p, n);
    printTable(p, n);
}

// Round Robin
void roundRobin(Process p[], int n) {
    int tq;
    printf("Enter Time Quantum: ");
    scanf("%d", &tq);

    sortByArrival(p, n);
    int rem[n], complete = 0, time = 0, visited[n];
    for (int i = 0; i < n; i++) { rem[i] = p[i].BT; visited[i] = 0; }

    int queue[1000], front = 0, rear = 0;
    int ganttPid[n * 10], ganttStart[n * 10], gIndex = 0;

    queue[rear++] = 0; visited[0] = 1;

    while (complete < n) {
        int idx = queue[front++];

        if (time < p[idx].AT) time = p[idx].AT;
        ganttPid[gIndex] = p[idx].PID;
        ganttStart[gIndex++] = time;

        if (rem[idx] > tq) {
            rem[idx] -= tq; time += tq;
        } else {
            time += rem[idx]; rem[idx] = 0;
            complete++;
            p[idx].CT = time;
            p[idx].TAT = p[idx].CT - p[idx].AT;
            p[idx].WT = p[idx].TAT - p[idx].BT;
        }

        for (int i = 0; i < n; i++)
            if (!visited[i] && p[i].AT <= time) { queue[rear++] = i; visited[i] = 1; }

        if (rem[idx] > 0) queue[rear++] = idx;

        if (front == rear) {
            for (int i = 0; i < n; i++)
                if (!visited[i]) { queue[rear++] = i; visited[i] = 1; break; }
        }
    }

    printGanttChart(ganttPid, ganttStart, gIndex, time);
    sortByPID(p, n);
    printTable(p, n);
}

// Priority Non-Preemptive
void priorityNonPreemptive(Process p[], int n) {
    int completed = 0, time = 0;
    bool done[n]; 
    for (int i = 0; i < n; i++) done[i] = false;

    int ganttPid[n], ganttStart[n], gIndex = 0;

    while (completed < n) {
        int idx = -1, highest = INT_MAX;
        for (int i = 0; i < n; i++) {
            if (p[i].AT <= time && !done[i] && p[i].Priority < highest) {
                highest = p[i].Priority; idx = i;
            }
        }
        if (idx == -1) { time++; continue; }

        ganttPid[gIndex] = p[idx].PID;
        ganttStart[gIndex++] = time;

        time += p[idx].BT;
        p[idx].CT = time;
        p[idx].TAT = p[idx].CT - p[idx].AT;
        p[idx].WT = p[idx].TAT - p[idx].BT;
        done[idx] = true;
        completed++;
    }

    printGanttChart(ganttPid, ganttStart, gIndex, time);
    sortByPID(p, n);
    printTable(p, n);
}

// Priority Preemptive
void priorityPreemptive(Process p[], int n) {
    int rem[n], completed = 0, time = 0, prev = -1;
    bool done[n];
    for (int i = 0; i < n; i++) { rem[i] = p[i].BT; done[i] = false; }

    int ganttPid[n * 20], ganttStart[n * 20], gIndex = 0;

    while (completed < n) {
        int idx = -1, highest = INT_MAX;
        for (int i = 0; i < n; i++) {
            if (p[i].AT <= time && !done[i] && rem[i] > 0 && p[i].Priority < highest) {
                highest = p[i].Priority; idx = i;
            }
        }
        if (idx == -1) { time++; continue; }

        if (prev != idx) {
            ganttStart[gIndex] = time;
            ganttPid[gIndex++] = p[idx].PID;
            prev = idx;
        }

        rem[idx]--; time++;
        if (rem[idx] == 0) {
            p[idx].CT = time;
            p[idx].TAT = p[idx].CT - p[idx].AT;
            p[idx].WT = p[idx].TAT - p[idx].BT;
            done[idx] = true;
            completed++;
        }
    }

    printGanttChart(ganttPid, ganttStart, gIndex, time);
    sortByPID(p, n);
    printTable(p, n);
}

int main() {
    int n;
    printf("Enter number of processes: ");
    scanf("%d", &n);

    Process p[n];
    for (int i = 0; i < n; i++) {
        p[i].PID = i + 1;
        printf("Arrival Time of P%d: ", i + 1); scanf("%d", &p[i].AT);
        printf("Burst Time of P%d: ", i + 1);   scanf("%d", &p[i].BT);
        printf("Priority of P%d : ", i + 1); scanf("%d", &p[i].Priority);
    }

    int choice;
    bool running = true;
    while (running) {
        printf("\n==== Scheduling Menu ====\n");
        printf("1. FCFS\n");
        printf("2. SJF (Preemptive)\n");
        printf("3. SJF (Non-Preemptive)\n");
        printf("4. Round Robin\n");
        printf("5. Priority (Preemptive)\n");
        printf("6. Priority (Non-Preemptive)\n");
        printf("7. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: fcfs(p, n); break;
            case 2: sjfPreemptive(p, n); break;
            case 3: sjfNonPreemptive(p, n); break;
            case 4: roundRobin(p, n); break;
            case 5: priorityPreemptive(p, n); break;
            case 6: priorityNonPreemptive(p, n); break;
            case 7: running = false; printf("Exiting...\n"); break;
            default: printf("Invalid choice!\n");
        }
    }
    return 0;
}
