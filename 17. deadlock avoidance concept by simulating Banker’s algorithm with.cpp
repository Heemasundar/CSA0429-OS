#include <stdio.h>
#include <stdbool.h>

#define MAX_PROCESSES 5
#define MAX_RESOURCES 3

int available[MAX_RESOURCES];
int maximum[MAX_PROCESSES][MAX_RESOURCES];
int allocation[MAX_PROCESSES][MAX_RESOURCES];
int need[MAX_PROCESSES][MAX_RESOURCES];
int processes, resources;

void calculateNeed() {
    for (int i = 0; i < processes; i++)
        for (int j = 0; j < resources; j++)
            need[i][j] = maximum[i][j] - allocation[i][j];
}

bool isSafe() {
    int work[MAX_RESOURCES];
    bool finish[MAX_PROCESSES] = {false};

    for (int i = 0; i < resources; i++)
        work[i] = available[i];

    int safeSeq[MAX_PROCESSES], count = 0;

    while (count < processes) {
        bool found = false;
        for (int i = 0; i < processes; i++) {
            if (!finish[i]) {
                bool canAllocate = true;
                for (int j = 0; j < resources; j++)
                    if (need[i][j] > work[j])
                        canAllocate = false;

                if (canAllocate) {
                    for (int j = 0; j < resources; j++)
                        work[j] += allocation[i][j];

                    safeSeq[count++] = i;
                    finish[i] = true;
                    found = true;
                }
            }
        }
        if (!found)
            return false; // System is in an unsafe state
    }

    printf("Safe Sequence: ");
    for (int i = 0; i < processes; i++)
        printf("P%d ", safeSeq[i]);
    printf("\n");

    return true;
}

bool requestResources(int processID, int request[]) {
    for (int i = 0; i < resources; i++)
        if (request[i] > need[processID][i] || request[i] > available[i])
            return false;

    for (int i = 0; i < resources; i++) {
        available[i] -= request[i];
        allocation[processID][i] += request[i];
        need[processID][i] -= request[i];
    }

    if (isSafe())
        return true;

    // If not safe, rollback the allocation
    for (int i = 0; i < resources; i++) {
        available[i] += request[i];
        allocation[processID][i] -= request[i];
        need[processID][i] += request[i];
    }
    return false;
}

void printState() {
    printf("\nCurrent System State:\n");
    printf("Process\t Allocation\t Maximum\t Need\n");

    for (int i = 0; i < processes; i++) {
        printf("P%d\t ", i);
        for (int j = 0; j < resources; j++)
            printf("%d ", allocation[i][j]);
        printf("\t ");
        for (int j = 0; j < resources; j++)
            printf("%d ", maximum[i][j]);
        printf("\t ");
        for (int j = 0; j < resources; j++)
            printf("%d ", need[i][j]);
        printf("\n");
    }

    printf("Available Resources: ");
    for (int i = 0; i < resources; i++)
        printf("%d ", available[i]);
    printf("\n");
}

int main() {
    printf("Enter number of processes: ");
    scanf("%d", &processes);
    printf("Enter number of resources: ");
    scanf("%d", &resources);

    printf("Enter available resources: ");
    for (int i = 0; i < resources; i++)
        scanf("%d", &available[i]);

    printf("Enter maximum resource matrix:\n");
    for (int i = 0; i < processes; i++)
        for (int j = 0; j < resources; j++)
            scanf("%d", &maximum[i][j]);

    printf("Enter allocated resource matrix:\n");
    for (int i = 0; i < processes; i++)
        for (int j = 0; j < resources; j++)
            scanf("%d", &allocation[i][j]);

    calculateNeed();
    printState();

    if (isSafe())
        printf("System is in a safe state.\n");
    else {
        printf("System is in an unsafe state. Deadlock possible!\n");
        return 0;
    }

    int processID, request[MAX_RESOURCES];
    printf("Enter process ID for resource request: ");
    scanf("%d", &processID);
    printf("Enter requested resources: ");
    for (int i = 0; i < resources; i++)
        scanf("%d", &request[i]);

    if (requestResources(processID, request)) {
        printf("Request granted.\n");
        printState();
    } else {
        printf("Request denied. Unsafe state detected!\n");
    }

    return 0;
}
