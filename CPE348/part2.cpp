#include <fstream>
#include <iostream>
#include <cmath>
#include <vector>

const double TIMESLOTLENGTH = 51.2;
const int8_t NUMBER_OF_STATIONS{5};

// arrays holding data for each station
int nextSlot[NUMBER_OF_STATIONS] = {0, 0, 0, 0, 0};
int collisions[NUMBER_OF_STATIONS] = {0, 0, 0, 0, 0};

// input file path
std::ifstream random_numbers_file("Part1_rn.txt");

// gets random number from the file
int getNextRandom();

int getMaxSlot();

void printLineSeparator();

int main()
{
    int slot = 0;
    do
    {
        printLineSeparator();
        std::cout << "Checking time slot: " << slot << std::endl;

        std::vector<int> stationsTransmitting;

        // collect a list of stations
        for (int station = 0; station < NUMBER_OF_STATIONS; ++station)
        {
            if (nextSlot[station] == slot)
            {
                stationsTransmitting.push_back(station);
            }
        }

        if (stationsTransmitting.size() > 1)
        {
            for (int station : stationsTransmitting)
            {
                ++collisions[station];

                nextSlot[station] += (getNextRandom() % (int)pow(2.0, collisions[station])) + 1;

                printf("- Station %d: backing off to slot %d collisions: %d\n", station, nextSlot[station], collisions[station]);
            }
        }
        else if (stationsTransmitting.size() == 1)
        {
            printf("* Station %d has successfully transmitted\n in slot number %d, which is %.1f microseconds\n", stationsTransmitting[0], slot, TIMESLOTLENGTH * slot);
        }
        else
        {
            printf("X no stations have transmitted in this slot\n");
        }

        printLineSeparator();
        std::cout << "\n\n";

        ++slot;
    } while (getMaxSlot() >= slot);
}

int getMaxSlot()
{
    int maxValue = 0;
    for (auto s : nextSlot)
    {
        if (s > maxValue)
            maxValue = s;
    }
    return maxValue;
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