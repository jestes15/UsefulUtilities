#include <fstream>
#include <iostream>
#include <cmath>
#include <vector>

const double TIMESLOTLENGTH{51.2};
const uint8_t NUMBEROFSTATIONS{5};

// array holding data for each station
int nextSlot[NUMBEROFSTATIONS] = {0, 0, 0, 0, 0};
int collisions[NUMBEROFSTATIONS] = {0, 0, 0, 0, 0};

// input file path
std::ifstream random_numbers_file("Part1_rn.txt");

// randomly select the number from the file
int getNextRandom();
void printLineSeparator();

int main()
{
    int slot = 0;
    while (1)
    {
        printLineSeparator();
        std::cout << "Checking time slot: " << slot << std::endl;

        std::vector<int> stationsTransmitting;

        // display list of stations transmitting
        for (int station = 0; station < NUMBEROFSTATIONS; ++station)
        {
            if (nextSlot[station] == slot)
                stationsTransmitting.push_back(station);
        }

        if (stationsTransmitting.size() > 1)
        {
            for (int station : stationsTransmitting)
            {
                ++collisions[station];
                nextSlot[station] += (getNextRandom() % static_cast<int>(pow(2.0, collisions[station]))) + 1;
                printf("Station %d: backing off to slot %d collisions: %d\n", station, nextSlot[station], collisions[station]);
            }
        }
        else if (stationsTransmitting.size() == 1)
        {
            // print the transmitted station
            printf("Station %d has successfully transmitted\nin slot number %d, which is %.1f microseconds\n", stationsTransmitting[0], slot, TIMESLOTLENGTH * slot);
            break;
        }
        else
            printf("no stations have transmitted in this slot\n");

        printLineSeparator();
        std::cout << "\n\n";
        ++slot;
    }
}

int getNextRandom()
{
    int currentNumber;
    if (random_numbers_file >> currentNumber)
        return currentNumber;
    return 0;
}

void printLineSeparator()
{
    for (int i = 0; i < 50; ++i)
        std::cout << '-';
    std::cout << std::endl;
}