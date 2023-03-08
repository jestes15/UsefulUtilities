#include <fstream>
#include <iostream>
#include <cmath>
#include <vector>

typedef double f64;

const f64 TIMESLOTLENGTH{51.2};
const u8 NUMBER_OF_STATIONS{5};

// array holding data for each station
int nextSlot[NUMBER_OF_STATIONS] = {0, 0, 0, 0, 0};
int collisions[NUMBER_OF_STATIONS] = {0, 0, 0, 0, 0};

int getNextRandom(std::ifstream file)
{
    int currentNumber;
    if (file >> currentNumber)
        return currentNumber;
    return 0;
}

void printLineSeparator();

int main()
{
    std::ifstream rand_numbers("Part1_rn.txt");
    int slot = 0;
    while (1)
    {
        printLineSeparator();
        std::cout << "Checking time slot: " << slot << std::endl;

        std::vector<int> stationsTransmitting;

        // display list of stations transmitting
        for (int station = 0; station < NUMBER_OF_STATIONS; ++station)
        {
            if (nextSlot[station] == slot)
                stationsTransmitting.push_back(station);
        }

        if (stationsTransmitting.size() > 1)
        {
            for (int station : stationsTransmitting)
            {
                ++collisions[station];
                nextSlot[station] += (getNextRandom(rand_numbers) % static_cast<int>(std::pow(2.0, collisions[station]))) + 1;
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

void printLineSeparator()
{
    for (int i = 0; i < 50; ++i)
        std::cout << '-';
    std::cout << std::endl;
}