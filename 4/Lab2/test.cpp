#include <iostream>

int main() {
    int k = 0;
    do {
        k++;
    } while (k < 5);
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 100; j++) {
            k += i * j;
        }
    }
    for (int i = 0; i < 10; i++) {
        if (i == 5) {
            break;
        }
    }
    for (int i = 0; i < 5; i++) {
        if (i == 2) {
            continue;
        }
    }
    int choice = 2;
    switch (choice) {
        case 1:
            std::cout << "Choice is 1" << std::endl;
            break;
        case 2:
            std::cout << "Choice is 2" << std::endl;
            break;
        case 3:
            std::cout << "Choice is 3" << std::endl;
            break;
        default:
            std::cout << "Invalid choice" << std::endl;
            break;
    }
    return 0;
}