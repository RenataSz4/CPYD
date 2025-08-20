#include <thread>
#include <vector>
#include <random>
#include <algorithm>
#include <print>

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

        std::println("Thread {} sum: {}", id, total);
    }

    int getTotal() { return total; }

    int getId() { return id; }
};


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
    std::sort(sums.begin(), sums.end(), [](const auto& a, const auto& b) {
        return a->getTotal() > b->getTotal();
    });

    std::println("Highest total: Thread {} sum: {}", sums[0]->getId(), sums[0]->getTotal());

    return 0;
}