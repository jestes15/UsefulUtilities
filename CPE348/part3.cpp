// part1c.cpp
#define NUMBEROFSTATIONS 5

#include <fstream>
#include <iostream>
#include <cmath>
#include <vector>

const double TIMESLOTLENGTH = 51.2;

// data hold by array for each station
int nextSlot[NUMBEROFSTATIONS] = {0, 0, 0, 0, 0};
int collisions[NUMBEROFSTATIONS] = {0, 0, 0, 0, 0};
bool hasTx[NUMBEROFSTATIONS] = {0, 0, 0, 0, 0};

// input file object
std::ifstream random_numbers_file("Part1_rn.txt");

int getNextRandom();
bool hasAllTx();
void printLineSeparator();

int main()
{
    int slot = 0;
    do
    {
        std::vector<int> stationsTransmitting;

        for (int station = 0; station < NUMBEROFSTATIONS; ++station)
        {
            if (nextSlot[station] == slot)
            {
                stationsTransmitting.push_back(station);
            }
        }
        // check if station has transmiited or not
        if (stationsTransmitting.size() > 1)
        {
            for (int station : stationsTransmitting)
            {
                ++collisions[station];

                nextSlot[station] += (getNextRandom() % (int)pow(2.0, collisions[station])) + 1;
            }
        }
        else if (stationsTransmitting.size() == 1)
        {
            int station = stationsTransmitting[0];
            // print if stations has stranmitted succefully
            printf("* Station %d has successfully transmitted\n in slot number %d, which is %.1f microseconds\n", station, slot, TIMESLOTLENGTH * slot);
            hasTx[station] = true;
            collisions[station] = 0;
            nextSlot[station] += 6;
            std::cout << " current tx state [0:4]: ";
            printf(" current tx state [0:4]: [%d %d %d %d %d]\n\n", hasTx[0], hasTx[1], hasTx[2], hasTx[3], hasTx[4]);
        }

        ++slot;
    } while (!hasAllTx());
}

bool hasAllTx()
{
    for (auto tx : hasTx)
    {
        if (!tx)
            return false;
    }
    return true;
}

int getNextRandom()
{
    int currentNum;
    if (random_numbers_file >> currentNum)
        return currentNum;
    return 0; // false if EOF
}

void printLineSeparator()
{
    for (int i = 0; i < 50; ++i)
        std::cout << '-';

    std::cout << std::endl;
}