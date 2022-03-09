// Inc headers and libraries
#include <stdio.h>

// First in first out (FIFO)
void fifo(int refString[], int pageFrames, int size)
{

    // Top element index
    int index = -1;

    // Page faults
    int pageFaults = 0;

    // Always initialize value of memory frame to be -1 as initial value
    int memFrames[pageFrames];
    for (int i = 0; i < pageFrames; i++)
    {
        memFrames[i] = -1;
    }

    // Algorithm
    for (int i = 0; i < size; i++)
    {

        int refString_num = refString[i];
        int flag = 0;

        for (int j = 0; j < pageFrames; j++)
        {
            // If the reference string value is in any of the frames, we flag it
            if (refString_num == memFrames[j])
            {
                flag = 1;
                break;
            }
        }
        // Checks for page fault
        if (flag != 1)
        {
            index = (index + 1) % pageFrames;
            memFrames[index] = refString_num;
            pageFaults++;
            printf("\nStep %d:  ", pageFaults);
            for (int j = 0; j < pageFrames; j++)
            {
                printf("%d ", memFrames[j]);
            }
        }
    }

    printf("\nThere are %d page faults in this page replacement process.", pageFaults);
}

// Optimal page replacement
void optimal(int refString[], int pageFrames, int size)
{

    int no_of_frames, no_of_pages, frames[10], pages[30], temp[10], flag1, flag2, flag3, i, j, k, pos, max, faults = 0, step = 0;
    no_of_frames = pageFrames;
    no_of_pages = size;
    for (i = 0; i < no_of_pages; ++i)
    {
        pages[i] = refString[i];
    }

    for (i = 0; i < no_of_frames; ++i)
    {
        frames[i] = -1;
    }

    for (i = 0; i < no_of_pages; ++i)
    {
        flag1 = flag2 = 0;

        for (j = 0; j < no_of_frames; ++j)
        {
            if (frames[j] == pages[i])
            {
                flag1 = flag2 = 1;
                break;
            }
        }

        if (flag1 == 0)
        {
            for (j = 0; j < no_of_frames; ++j)
            {
                if (frames[j] == -1)
                {
                    faults++;
                    frames[j] = pages[i];
                    flag2 = 1;
                    break;
                }
            }
            step++;
            printf("\nStep %d:  ", step);
            for (int j = 0; j < no_of_frames; ++j)
            {
                printf("%d ", frames[j]);
            }
        }

        if (flag2 == 0)
        {
            flag3 = 0;

            for (j = 0; j < no_of_frames; ++j)
            {
                temp[j] = -1;

                for (k = i + 1; k < no_of_pages; ++k)
                {
                    if (frames[j] == pages[k])
                    {
                        temp[j] = k;
                        break;
                    }
                }
            }

            for (j = 0; j < no_of_frames; ++j)
            {
                if (temp[j] == -1)
                {
                    pos = j;
                    flag3 = 1;
                    break;
                }
            }

            if (flag3 == 0)
            {
                max = temp[0];
                pos = 0;

                for (j = 1; j < no_of_frames; ++j)
                {
                    if (temp[j] > max)
                    {
                        max = temp[j];
                        pos = j;
                    }
                }
            }

            frames[pos] = pages[i];
            faults++;
        }
    }
    printf("\nThere are %d page faults in this page replacement process.", faults);
}

// Least recently used (LRU)
int LRU(int time[], int n)
{
    int i, minimum = time[0], pos = 0;

    for (i = 1; i < n; ++i)
    {
        if (time[i] < minimum)
        {
            minimum = time[i];
            pos = i;
        }
    }

    return pos;
}

void lru(int refString[], int pageFrames, int size)
{
    int no_of_frames, no_of_pages, frames[10], pages[30], counter = 0, time[10], flag1, flag2, i, j, pos, faults = 0, step = 0;
    no_of_frames = pageFrames;
    no_of_pages = size;
    for (i = 0; i < no_of_pages; ++i)
    {
        pages[i] = refString[i];
    }

    for (i = 0; i < no_of_frames; ++i)
    {
        frames[i] = -1;
    }

    for (i = 0; i < no_of_pages; ++i)
    {
        flag1 = flag2 = 0;

        for (j = 0; j < no_of_frames; ++j)
        {
            if (frames[j] == pages[i])
            {
                counter++;
                time[j] = counter;
                flag1 = flag2 = 1;
                break;
            }
        }

        if (flag1 == 0)
        {
            for (j = 0; j < no_of_frames; ++j)
            {
                if (frames[j] == -1)
                {
                    counter++;
                    faults++;
                    frames[j] = pages[i];
                    time[j] = counter;
                    flag2 = 1;
                    break;
                }
            }
            step++;
            printf("\nStep %d:  ", step);
            for (int j = 0; j < no_of_frames; ++j)
            {
                printf("%d ", frames[j]);
            }
        }

        if (flag2 == 0)
        {
            pos = LRU(time, no_of_frames);
            counter++;
            faults++;
            frames[pos] = pages[i];
            time[pos] = counter;
        }
    }

    printf("\nThere are %d page faults in this page replacement process.", faults);
}

// MAIN
int main(void)
{
    int pageFrames;
    int refString_size;
    int algoChoice;
    char algoName[30];
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

    printf("Enter the reference string: \n");
    printf("eg. 7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2, 1, 2, 0, 1, 7, 0, 1\n");
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