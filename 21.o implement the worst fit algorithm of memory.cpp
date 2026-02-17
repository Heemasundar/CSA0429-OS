#include <stdio.h>

// Function to allocate memory to processes using Worst Fit
void worstFit(int blockSize[], int m, int processSize[], int n) {
    int allocation[n]; // Stores block allocated to each process (-1 if not allocated)
    
    // Initialize all allocations as -1 (not allocated)
    for (int i = 0; i < n; i++) {
        allocation[i] = -1;
    }

    // Pick each process and find a suitable worst-fit block
    for (int i = 0; i < n; i++) {
        int worstIdx = -1;
        for (int j = 0; j < m; j++) {
            if (blockSize[j] >= processSize[i]) {
                if (worstIdx == -1 || blockSize[j] > blockSize[worstIdx]) {
                    worstIdx = j; // Update index of the largest suitable block
                }
            }
        }

        // If a suitable block was found, allocate it
        if (worstIdx != -1) {
            allocation[i] = worstIdx; // Store block index
            blockSize[worstIdx] -= processSize[i]; // Reduce block size
        }
    }

    // Display results
    printf("\nProcess No.\tProcess Size\tBlock No.\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t\t%d\t\t", i + 1, processSize[i]);
        if (allocation[i] != -1)
            printf("%d\n", allocation[i] + 1);
        else
            printf("Not Allocated\n");
    }
}

// Main function
int main() {
    int blockSize[] = {100, 500, 200, 300, 600}; // Available memory blocks
    int processSize[] = {212, 417, 112, 426}; // Process sizes
    int m = sizeof(blockSize) / sizeof(blockSize[0]); // Number of blocks
    int n = sizeof(processSize) / sizeof(processSize[0]); // Number of processes

    worstFit(blockSize, m, processSize, n); // Call Worst-Fit function

    return 0;
}
