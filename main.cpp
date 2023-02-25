#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int quicksort(vector<int> &arr, int left, int right)
{
    if (right <= left)
    {
        return 0;
    }

    int pivot = arr[(left + right) / 2];
    int i = left - 1;
    int j = right + 1;

    int quick_comp_count = 0;

    while (true)
    {
        do
        {
            i++;
            quick_comp_count++;
        } while (arr[i] < pivot);

        do
        {
            j--;
            quick_comp_count++;
        } while (pivot < arr[j]);

        if (j <= i)
        {
            break;
        }

        swap(arr[i], arr[j]);
    }

    quicksort(arr, left, j);
    quicksort(arr, j + 1, right);
    return quick_comp_count;
}

void merge(vector<int> &arr, int left, int mid, int right)
{
    int l, m, n;
    vector<int> temp(right - left + 1);

    l = left;
    m = mid + 1;
    n = 0;

    while (l <= mid && m <= right)
    {
        if (arr[l] < arr[m])
        {
            temp[n] = arr[l];
            n++;
            l++;
        }
        else
        {
            temp[n] = arr[m];
            n++;
            m++;
        }
    }

    while (l <= mid)
    {
        temp[n] = arr[l];
        n++;
        l++;
    }

    while (m <= right)
    {
        temp[n] = arr[m];
        n++;
        m++;
    }

    for (l = left; l <= right; l++)
    {
        arr[l] = temp[l - left];
    }
}

void merge_sort(vector<int> &arr, int left, int right)
{
    if (left < right)
    {
        int mid = (left + right) / 2;
        merge_sort(arr, left, mid);
        merge_sort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

int main()
{
    // Open input file
    ifstream input_file("input.txt");
    ofstream output_file("heath_quick_right.txt");

    if (!input_file)
    {
        cout << "Failed to open input file." << endl;
        return 1;
    }

    if (!output_file)
    {
        cout << "Failed to open output file." << endl;
        return 1;
    }

    // Read input array from file
    int size = 0;
    input_file >> size;

    vector<int> quicksort_arr(size);
    vector<int> mergesort_arr(size);

    int temp = 0;
    for (int s = 0; s < size; s++)
    {
        input_file >> temp;
        quicksort_arr[s] = temp;
        mergesort_arr[s] = temp;
    }

    // Close input file
    input_file.close();

    // Sort the array using quicksort
    std::cout << quicksort(quicksort_arr, 0, quicksort_arr.size() - 1);

    // Sort the array using merge sort
    merge_sort(mergesort_arr, 0, size - 1);

    // Write sorted array to output file
    for (int s = 0; s < quicksort_arr.size(); s++)
        output_file << quicksort_arr[s] << " ";

    // Close input and output file
    output_file.close();

    // Open input and output file
    output_file.open("heath_merge_right.txt");

    if (!output_file)
    {
        cerr << "Failed to open output file." << endl;
        return 1;
    }

    // Write sorted array to output file
    for (int s = 0; s < size; s++)
    {
        output_file << mergesort_arr[s] << " ";
    }

    // Close output file
    output_file.close();

    return 0;
}