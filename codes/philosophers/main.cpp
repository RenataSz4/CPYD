#include<iostream>
#include<memory>
#include<thread>
#include<string>
#include<print>
#include<mutex>
#include<condition_variable>

class Fork {
        bool busy;
        std::mutex mutex;
        std::condition_variable taken;
        std::string_view name;
    public:
        void pickUp(std::string_view name) {
            {
                std::unique_lock lock(mutex);
                taken.wait(lock, [&] { return !busy; });
                busy = true;
                this->name = name;
            }
        }

        void putDown() {
            busy = false;
            taken.notify_one();
        }
        bool Busy() const {
            return busy;
        }
};

class Philosopher {
    private:
        std::shared_ptr<Fork> leftFork;
        std::shared_ptr<Fork> rightFork;
        std::string name;
    public:
        void eat() {
            while (true) {
                std::println("{} picking left fork",name);
                std::println("{} picking right fork",name);
                leftFork->pickUp(name);
                if (rightFork->Busy()) {
                    leftFork->putDown();
                    continue;
                };
                rightFork->pickUp(name);
                std::println("{} started eating", name);
                std::this_thread::sleep_for(std::chrono::seconds(2));
                std::println("{} done eating", name);
                std::println("{} put left fork down", name);
                leftFork->putDown();
                std::println("{} put right fork down", name);
                rightFork->putDown();
            }
        }
        std::shared_ptr<Fork> &LeftFork() {
            return leftFork;
        }
        std::shared_ptr<Fork> &RightFork() {
            return rightFork;
        }
        Philosopher(std::string name):name(name) {
        }
};


int main() {
    std::shared_ptr<Fork> fork1 = std::make_shared<Fork>();
    std::shared_ptr<Fork> fork2 = std::make_shared<Fork>();
    std::shared_ptr<Fork> fork3 = std::make_shared<Fork>();
    std::shared_ptr<Fork> fork4 = std::make_shared<Fork>();
    std::shared_ptr<Fork> fork5 = std::make_shared<Fork>();

    Philosopher shrek("Shrek");
    Philosopher donkey("Donkey");
    Philosopher fiona("Fiona");
    Philosopher puss("Puss");
    Philosopher dragon("Dragon");

    shrek.LeftFork() = fork1;
    shrek.RightFork() = fork2;

    donkey.LeftFork() = fork2;
    donkey.RightFork() = fork3;

    fiona.LeftFork() = fork3;
    fiona.RightFork() = fork4;

    puss.LeftFork() = fork4;
    puss.RightFork() = fork5;

    dragon.LeftFork() = fork5;
    dragon.RightFork() = fork1;

    std::thread threadShrek([&](){
        shrek.eat();
    });

    std::thread threadDonkey([&]() {
        donkey.eat();
    });

    std::thread threadFiona([&]() {
        fiona.eat();
    });

    std::thread threadPuss([&]() {
        puss.eat();
    });

    std::thread threadDragon([&]() {
        dragon.eat();
    });

    threadShrek.join(); //para que el thread principal espere
    threadDonkey.join();
    threadFiona.join();
    threadPuss.join();
    threadDragon.join();

    return 0;
}