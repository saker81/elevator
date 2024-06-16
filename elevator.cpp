#include <iostream>
#include <queue>
#include <list>
#include <vector>
#include <algorithm>
#include <chrono>
#include <thread>
#include <sstream>  // for std::istringstream
#include <iomanip>  // for std::setw
#include <stack>    // for std::stack

// Constants for elevator states
const int IDLE = 0;
const int UP = 1;
const int DOWN = -1;

// Function to center align text
void printCentered(const std::string &text, int width) {
    int padding = (width - text.size()) / 2;
    std::cout << std::setw(padding) << "" << text << std::endl;
}

class Elevator {
private:
    int currentFloor;
    int direction;
    std::queue<int> requestQueue;  // Queue to hold floor requests
    std::list<int> floorsVisited;  // Linked list to maintain floors visited
    std::vector<bool> floorRequests;  // Array list to track floor requests
    std::stack<int> requestHistory;  // Stack to track request history

    void printElevatorStatus() {
        std::cout << "\nElevator Status:\n";
        for (int i = floorRequests.size() - 1; i > 0; --i) {
            if (i == currentFloor) {
                std::cout << "[ " << i << " ] <-- Elevator\n";
            } else {
                std::cout << "[ " << i << " ]\n";
            }
        }
        std::cout << std::endl;
    }

public:
    Elevator(int numFloors) {
        currentFloor = 1;
        direction = IDLE;
        floorRequests.resize(numFloors + 1, false);  // Resize vector to numFloors + 1, initialized to false
    }

    void requestFloor(int floor) {
        if (floor < 1 || floor >= floorRequests.size()) {
            std::cout << "Invalid floor request: " << floor << std::endl;
            return;
        }

        if (floorRequests[floor]) {
            std::cout << "Floor " << floor << " already requested." << std::endl;
            return;
        }

        floorRequests[floor] = true;
        requestQueue.push(floor);
        requestHistory.push(floor);  // Track request history
        processRequests();
    }

    void move() {
        if (requestQueue.empty()) {
            std::cout << "No more requests, staying idle." << std::endl;
            direction = IDLE;
            return;
        }

        int nextFloor = requestQueue.front();
        requestQueue.pop();

        if (nextFloor > currentFloor) {
            direction = UP;
        } else if (nextFloor < currentFloor) {
            direction = DOWN;
        } else {
            std::cout << "Already at floor " << currentFloor << std::endl;
            return;
        }

        while (currentFloor != nextFloor) {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));  // Simulate movement delay
            currentFloor += direction;
            printElevatorStatus();
            std::cout << "Moving to floor " << currentFloor << "...\n";
            floorsVisited.push_back(currentFloor);
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));  // Simulate door opening delay
        std::cout << "Reached floor " << currentFloor << std::endl;
        floorRequests[currentFloor] = false;  // Clear request for the current floor
        processRequests();
    }

    void processRequests() {
        while (!requestQueue.empty()) {
            move();
        }
    }

    void printFloorsVisited() {
        std::cout << "Floors visited: ";
        for (auto floor : floorsVisited) {
            std::cout << floor << " ";
        }
        std::cout << std::endl;
    }

    void idleTime(int seconds) {
        std::cout << "Elevator is idle for " << seconds << " seconds." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(seconds));
    }

    void viewStatus() {
        std::cout << "\nCurrent Elevator Status:\n";
        printElevatorStatus();
        std::cout << "Floors visited so far: ";
        for (auto floor : floorsVisited) {
            std::cout << floor << " ";
        }
        std::cout << "\nRequest history (most recent last): ";
        std::stack<int> tempStack = requestHistory;  // Copy stack for printing
        std::vector<int> history;
        while (!tempStack.empty()) {
            history.push_back(tempStack.top());
            tempStack.pop();
        }
        std::reverse(history.begin(), history.end());
        for (auto floor : history) {
            std::cout << floor << " ";
        }
        std::cout << std::endl;
    }
};

int main() {
    const int terminalWidth = 50;
    std::cout << "\n\n";
    printCentered("*", terminalWidth);
    printCentered("*                     *", terminalWidth);
    printCentered("* Welcome to the      *", terminalWidth);
    printCentered("* Elevator Simulation *", terminalWidth);
    printCentered("*                     *", terminalWidth);
    printCentered("*", terminalWidth);
    std::cout << "\n\n";

    int numFloors;
    std::cout << "Enter the number of floors in the building: ";
    std::cin >> numFloors;

    Elevator elevator(numFloors);

    char choice;
    std::string input;
    while (true) {
        std::cout << "\nOptions:\n";
        std::cout << "1. Enter a single floor request\n";
        std::cout << "2. Enter multiple floor requests (comma-separated)\n";
        std::cout << "3. Simulate idle time\n";
        std::cout << "4. View elevator status\n";
        std::cout << "q. Quit\n";
        std::cout << "Choose an option: ";
        std::cin >> choice;

        if (choice == 'q') {
            break;
        }

        switch (choice) {
            case '1': {
                std::cout << "Enter floor number to go to: ";
                int floorRequest;
                std::cin >> floorRequest;
                elevator.requestFloor(floorRequest);
                break;
            }
            case '2': {
                std::cout << "Enter floor numbers to go to (comma-separated): ";
                std::cin.ignore();
                std::getline(std::cin, input);
                std::istringstream ss(input);
                std::string token;
                while (std::getline(ss, token, ',')) {
                    int floorRequest = std::stoi(token);
                    elevator.requestFloor(floorRequest);
                }
                break;
            }
            case '3': {
                std::cout << "Enter idle time in seconds: ";
                int seconds;
                std::cin >> seconds;
                elevator.idleTime(seconds);
                break;
            }
            case '4': {
                elevator.viewStatus();
                break;
            }
            default: {
                std::cout << "Invalid option. Please try again." << std::endl;
                break;
            }
        }
    }

    std::cout << "\n*** Simulation ended. Floors visited: ***\n";
    elevator.printFloorsVisited();

    return 0;
}