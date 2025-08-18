#include <iostream>
#include <thread>
#include <vector>
#include <random>
#include <string>
#include <mutex>

class RandomSum {
private:
    int id;
    int total;

public:
    RandomSum(int id) : id(id), total(0) {}

    void operator()() {
        // Random number generator setup
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, 1000);

        // Sum
        for (int i = 0; i < 100; ++i) {
            total += dis(gen);
        }

        std::lock_guard<std::mutex> lock(mtx);
        std::cout << "Thread " << id << " sum: " << total << std::endl;
    }

    int getTotal() { return total; }

    int getId() { return id; }

    static std::mutex mtx;
};

std::mutex RandomSum::mtx;

int main() {
    std::vector<std::thread> threads;
    std::vector<std::unique_ptr<RandomSum>> sums;

    // Objects initialization
    for (int i = 1; i <= 10; ++i) {
        sums.push_back(std::make_unique<RandomSum>(i));
        threads.emplace_back(std::ref(*sums.back()));
    }

    for (auto& thread : threads) {
        thread.join();
    }

    // Thread with the highest total
    int bestId = sums[0]->getId();
    int bestTotal = sums[0]->getTotal();

    for (const auto& sum : sums) {
        if (sum->getTotal() > bestTotal) {
            bestTotal = sum->getTotal();
            bestId = sum->getId();
        }
    }

    std::cout << "Highest total: Thread " << bestId << " total sum " << bestTotal << std::endl;

    return 0;
}