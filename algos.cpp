//
// Created by Stefan Postnikov on 18.02.2022.
//

/*
Контрольные вопросы

1) Случайные числа в библиотеке random генерируются с помощью классов генераторов псевдослучайных чисел, например mt19937.
Генератор получает при конструировании seed -- число, посредоством обработки которого генератор получается "случайные" числа.
Для моделирования некоторого распределения используются специальные классы, которые обрабатывают случайные числа, полученные каким-либо генератором, так, чтобы результат был распределён согласно некоторому распределению, например Бернуллевскому, Пуассоновскому итд.

2) Формально, итераторы - это типы, для которых определены операторы *, ++ а также выполнены свойста CopyConstructible, CopyAssignable итд. 
В зависимости от типа итераторы input/output iterator, forward iterator итд, должны быть выполнены и другие условия.
Диапазон - это пара итераторов, подразумевается, что проходится как интервал: [first, last). 
Стандартные библиотеки работают с итераторами, а не с контейнерами для универсальности: алгоритм можно применить для пары итераторов даже для кастомного контейнера или диапазона. 

*/

#include <iostream>

#include <vector>
#include <algorithm>
#include <numeric>
#include <iterator>
#include <random>
#include <chrono>

template<typename T>
std::ostream& operator<<(std::ostream& out,const std::vector<T>& container){
    out << '[';
    std::copy(std::begin(container), std::prev(std::end(container)), std::ostream_iterator<T>(out, ", "));
    out << *(std::prev(std::end(container))) << ']';
    return out;
}


int main() {
    auto seed = std::chrono::steady_clock::now().time_since_epoch().count();
    auto generator = std::mt19937(seed);
    auto distribution = std::uniform_int_distribution<>(1, 100);

    std::vector<int> seq1(10);

    std::iota(seq1.begin(),seq1.end(), 1);

    std::copy_n(std::istream_iterator<int>(std::cin), 5, std::back_inserter(seq1));

    std::shuffle(seq1.begin(), seq1.end(), generator);

    seq1.erase(std::unique(seq1.begin(), seq1.end()), seq1.end());

    auto odd_counter = std::count_if(seq1.begin(), seq1.end(), [](int x){ return x % 2 != 0; });
    std::cout << "Number of odd numbers in seq1: " << odd_counter << std::endl;

    int max1 = *std::max_element(seq1.begin(), seq1.end());
    int min1 = *std::min_element(seq1.begin(), seq1.end());
    std::cout << "Max in seq1: " << max1 << "; Min in seq2: " << min1 << std::endl; 

    auto prime_it = find_if(seq1.begin(), seq1.end(), 
            [](int x){
                bool is_prime = false;
                for (int i = 2; i <= x/2+1; ++i) {
                    is_prime = x%i != 0;
                }
                return is_prime;
            });
    if (prime_it != seq1.end())
        std::cout << "Prime number in seq1: " << *prime_it << std::endl;
    else
        std::cout << "No prime numbers in seq1" << std::endl;

    std::transform(seq1.begin(), seq1.end(), seq1.begin(), [](int x) { return x*x; });

    std::vector<int> seq2(seq1.size());
    std::generate(seq2.begin(), seq2.end(), 
            [&generator, &distribution](){
            return distribution(generator);
            });

    int sum2 = std::accumulate(seq2.begin(), seq2.end(), 0);
    std::cout << "Sum of seq2: " << sum2 << std::endl;

    std::fill_n(seq2.begin(), 3, 1);

    std::vector<int> seq3;

    std::transform(seq1.begin(), seq1.end(), seq2.begin(), std::back_inserter(seq3), 
            [](int x, int y){ return x - y; });

    std::transform(seq3.begin(), seq3.end(), seq3.begin(), 
            [](int x){
                if (x < 0) return 0;
                else return x;
            });

    seq3.erase(std::remove(seq3.begin(), seq3.end(), 0), seq3.end());

    std::reverse(seq3.begin(), seq3.end());

    std::partial_sort(seq3.begin(), seq3.begin()+3, seq3.end(), [](int lhs, int rhs){ return lhs > rhs; });
    std::cout << "Top 3 max elements of seq3: " << seq3[0] << ", " << seq3[1] << ", " << seq3[2] << std::endl;

    std::sort(seq1.begin(), seq1.end());
    std::sort(seq2.begin(), seq2.end());

    std::vector<int> seq4;
    std::merge(seq1.begin(), seq1.end(),
            seq2.begin(), seq2.end(),
            std::back_inserter(seq4));

    auto eq_range1 = std::equal_range(seq4.begin(), seq4.end(), 1);
    auto dist = std::distance(eq_range1.first, eq_range1.second);

    std::cout << "Distance of 1 in seq4: " << dist << std::endl;

    std::cout << "seq1: " << seq1 << std::endl
            << "seq2: " << seq2 << std::endl
            << "seq3: " << seq3 << std::endl
            << "seq4: " << seq4 << std::endl;

    return 0;
}