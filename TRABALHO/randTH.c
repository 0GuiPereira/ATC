#include <stdlib.h>


static const int randomTable[] = { 123, 456, 789, 234, 567, 890, 345, 678, 901, 432 };

// Function to generate a random value between min and max
double getRandomValue(double min, double max) {
    // Select a random index from the table
    int index = rand() % (sizeof(randomTable) / sizeof(randomTable[0]));

    // Map the selected index to the desired range
    return min + (randomTable[index] / (double)RAND_MAX) * (max - min);
}