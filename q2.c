// Inc headers and libraries
#include <stdio.h>

/**
 * @brief First-in-First-Out algorithm. Store the pages in memory.
 * When doing the page replacement, Swap the pages based on the pointer, which
 * always points to the oldest page in memory.
 * @param refString User input for the page replacement
 * @param pageFrames Number of avialable pages in the memory
 * @param size Size of the reference string
 */
void fifo(int refString[], int pageFrames, int size)
{
    /**Initialization to to first set each individual memory frame
     * to a initial value of -1. */
    int memFrames[pageFrames];
    for (int i = 0; i < pageFrames; i++){
        memFrames[i] = -1;
    }

    int index = -1;         //top element of the index
    int pageFaults = 0;     //counter for the number of page faults

    /**Double nested for loop to first go throught the reference string
     * And another fortraversing through the number of memory frames.
     */
    for (int i = 0; i < size; i++){
        int refString_num = refString[i];
        int flag = 0;
        for (int j = 0; j < pageFrames; j++){
            //If the reference string value is in any of the frames
            //We flag it to indicate no swap in pages needed.
            if (refString_num == memFrames[j]){
                flag = 1;
                break;
            }
        }
        // Checks for page fault
        if (flag != 1){
            index = (index + 1) % pageFrames;       //first we increase the pointer to the top element of memory frame mod size 
            memFrames[index] = refString_num;       //and set the memory frame to the ref_string number
            pageFaults++;
            printf("\nStep %d:  ", pageFaults);
            for (int j = 0; j < pageFrames; j++){
                printf("%d ", memFrames[j]);
            }
        }
    }
    printf("\nThere are %d page faults in this page replacement process.", pageFaults);
}

// Optimal page replacement
void optimal(int refString[], int pageFrames, int size)
{
    /**Initialization to to first set each individual memory frame
     * to a initial value of -1. */
    int memFrames[pageFrames];
    for (int i = 0; i < pageFrames; i++){
        memFrames[i] = -1;
    }

    // Initialize optimal variable to store most optimal element
    int optimal[pageFrames];
    int bestCase = 0;
    int flag = 0;           //Flag if a fault exist

    int found = 0;
    int contains = 0;

    // Page faults
    int pageFaults = 0;

    // Algorithm
    for (int i = 0; i < size; i++){
        int refString_num = refString[i];
        for (int i = 0; i < pageFrames; i++){       //initialize every page in optimal to be back to -1
            optimal[i] = -1;
        }
        for (int j = 0; j < pageFrames; j++){
            // if the reference string value is in any of the frames, we set found = 1
            if (memFrames[j] == refString_num){
                found = 1;
            }
            else if (memFrames[j] == -1 && !found){
                memFrames[j] = refString_num;
                pageFaults++;
                flag = 1;
                found = 1;
                break;
            }
        }

        // If found or empty
        if (found){
            found = 0;
        }
        // We replace the optimal index
        else{
            flag = 1;
            pageFaults++;

            for (int l = 0; l < pageFrames; l++){
                // Find future page number
                for (int k = i; k < size; k++){
                    int refString_num2 = refString[k];
                    // Find next page number index, insert into an array
                    if (memFrames[l] == refString_num2){
                        optimal[l] = k;
                        break;
                    }
                }
            }

            int temp = optimal[0];
            for (int m = 0; m < pageFrames; m++){
                // If number does not exist in future case, we replace it
                if (optimal[m] == -1){
                    bestCase = m;
                    contains = 1;
                    break;
                }
                if (contains){
                    contains = 0;
                }
                // Else we get the furthest value
                else if (temp < optimal[m]){
                    temp = optimal[m];
                    bestCase = m;
                }
            }
            memFrames[bestCase] = refString_num;
            bestCase = 0;
        }

        // If page fault exist, we print it
        if (flag){
            printf("\nStep %d:  ", pageFaults);
            for (int j = 0; j < pageFrames; j++){
                printf("%d ", memFrames[j]);
            }
            flag = 0;
        }
    }
    printf("\nThere are %d page faults in this page replacement process.", pageFaults);
}

// Least recently used (LRU)
void lru(int refString[], int pageFrames, int size)
{
    int memFrames[pageFrames];
    int counter[pageFrames];
    int pageFrames_val = pageFrames;
    int valcheck = 0;

    int flag = 0;
    int pageFaults = 0;

    // Always initialize value of memory frame to be -1 as initial value
    for (int i = 0; i < pageFrames; i++)
    {
        memFrames[i] = -1;
        counter[i] = pageFrames_val;
        pageFrames_val -= 1;
    }

    for (int i = 0; i < size; i++)
    {

        // Increment to find out the least recently used
        for (int i = 0; i < pageFrames; i++)
        {
            counter[i]++;
        }

        int refString_num = refString[i];

        if (valcheck > pageFrames - 1)
        {
            valcheck = 0;
        }

        // if the reference string value is in any of the frames, we set flag = 1
        for (int y = 0; y < pageFrames; y++)
        {
            if (memFrames[y] == refString_num)
            {
                flag = 1;
            }
        }

        if (flag)
        {
            flag = 0;

            for (int y = 0; y < pageFrames; y++)
            {
                if (memFrames[y] == refString_num)
                {
                    counter[y] = 0;
                }
            }
        }
        else
        {
            pageFaults++;

            // Checks for most occurence in the array
            int h = 0, hIndex = 0;
            for (int i = 0; i < pageFrames; i++)
            {
                if (counter[i] > h)
                {
                    h = counter[i];
                    hIndex = i;
                }
            }

            // We will replace the least recently used page number with the highst occurrence in memory frame
            memFrames[hIndex] = refString_num;
            counter[hIndex] = 0;

            // If page fault exist, we print it

            printf("\nStep %d:  ", pageFaults);
            for (int j = 0; j < pageFrames; j++)
            {
                printf("%d ", memFrames[j]);
            }
        }

        valcheck++;
    }

    printf("\nThere are %d page faults in this page replacement process.", pageFaults);
}
// MAIN
int main(void)
{
    int pageFrames;
    int refString_size;
    int algoChoice;
    // Prompt user to select 1 of the 3 PRA
    printf("Select a page replacement algorithm (1-3): \n");
    printf("1. First in first out (FIFO) \n");
    printf("2. Optimal page replacemenet \n");
    printf("3. Least recently used (LRU) \n");
    scanf("%d", &algoChoice);
    // Prompt user to insert number of available frames
    printf("Indicate the number of available frames (3-6): \n");
    scanf("%d", &pageFrames);
    // Prompt user to enter a reference string
    printf("Enter page size (20-30): \n");
    scanf("%d", &refString_size);

    int refString[refString_size];
    int size = sizeof(refString) / sizeof(int);

    printf("Enter the reference string (without commas): \n");
    printf("eg. 7 0 1 2 0 3 0 4 2 3 0 3 2 1 2 0 1 7 0 1\n");
    for (int i = 0; i < refString_size; i++)
    {
        scanf("%d", &refString[i]);
    }

    printf("#################\n");
    printf("Your referenced string is: \n");
    for (int i = 0; i < refString_size; i++)
    {
        printf("%d ", refString[i]);
    }

    // Run algorithm based on user choice
    if (algoChoice == 1)
    {
        fifo(refString, pageFrames, size);
    }
    else if (algoChoice == 2)
    {
        optimal(refString, pageFrames, size);
    }
    else
    {
        lru(refString, pageFrames, size);
    }

    return 0;
}