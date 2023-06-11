#include <algorithm>
#include <vector>
#include <thread>
#include <iostream>
#include <ctime>
#include <mutex>
std::mutex mtx;


void get_sum(std::vector<std::vector<int>> allays, int& sum, int j, std::vector<int>& sums) 
{
    std::lock_guard<std::mutex> lock(mtx); // блокировка мьютекса,без него возникают проблемы с ссылками,выдают рандомные числа
    int a = allays[j].size();
    for (int u = 0; u < a; u++)
    {

        sum = sum + allays[j][u];
    };
    auto it = sums.begin() + j;
    sums.insert(it, sum);
    std::cout << "The sum is entered in total array " << j << "=" << sums[j] << std::endl;
    sum = 0;
};


int main()
{
    long int number;
    int count_threads;
    std::cout << "The program makes array from 1 to your number and gives you sums. Enter your number: ";
    std::cin >> number;
    std::cout << "How many threads do you want to add,if you enter 0,only main thread will work: ";
    std::cin >> count_threads;
    std::vector<int> data;

    for (int j = 1; j <= number; j++) { data.push_back(j); };
    int data_size = data.size();

    if (count_threads != 0)
    {
        unsigned int start_time = clock();// начальное время
        int i = 0;
        int size_array = number / count_threads;
        std::vector<std::vector<int>> allays;
        std::vector<int>sums(size_array);
        while (i < data_size)
        {
            std::vector<int> allay;
            for (int j = 0; j < size_array; j++)
            {
                allay.push_back(data[i]);
                i++;
            };
            allays.push_back(allay);
        };
        int as = allays.size();
        std::vector<std::thread> threads;

        for (int j = 0; j < as; j++)
        {
            int sum = 0;
            threads.push_back(std::thread(get_sum, allays, std::ref(sum), j, std::ref(sums)));
        };
        for (int i = 0; i < as; ++i) {
            threads[i].join();
        };
        int result = 0;
        for (int j = 0; j < size_array; j++)
        {
            result = result + sums[j];
        };
        std::cout << "Total result = " << result;
        unsigned int end_time = clock(); // конечное время
        std::cout << std::endl << "Time: " << end_time - start_time; // искомое время
    }
    else
    {
        unsigned int start_time = clock();// начальное время
        int result = 0;
        for (int j = 0; j < data_size; j++) { result = result + data[j]; };
        std::cout << "Total result = " << result;
        unsigned int end_time = clock(); // конечное время
        std::cout << std::endl << "Time: " << end_time - start_time; // искомое время
    };
    return 0;

};