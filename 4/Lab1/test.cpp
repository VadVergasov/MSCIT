#include <iostream>

int main() {
    int a = 5;
    int b = 3;
    int c;

    c = a + b;
    c = a - b;
    c = a * b;
    c = a / b;
    c = a % b;

    bool result;
    result = a == b;
    result = a != b;
    result = a > b;
    result = a < b;
    result = a >= b;
    result = a <= b;

    bool p = true;
    bool q = false;

    result = p && q;
    result = p || q;
    result = !p;

    unsigned int x = 0x0F;
    unsigned int y = 0x0A;
    unsigned int z;

    z = x & y;
    z = x | y;
    z = x ^ y;
    z = ~x;
    z = x << 2;
    z = x >> 2;

    int d = 10;

    d += 5;
    d -= 3;
    d *= 2;
    d /= 4;
    d %= 3;
    d &= 5;
    d |= 7;
    d ^= 2;
    d <<= 1;
    d >>= 2;

    int age = 25;
    std::string status;

    status = (age >= 18) ? "Adult" : "Child";

    for (int i = 0; i < 5; i++) {
        int k = 0;
    }

    int j = 0;
    while (j < 5) {
        j++;
    }

    int k = 0;
    do {
        k++;
    } while (k < 5);

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

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
        }
    }

    if (a > b) {
        a += b;
    }

    if (b > a) {
        b += a;
    } else {
        c += b;
    }

    if (a > b) {
        if (a > c) {
            a += c;
        } else {
            a += b;
        }
    } else {
        if (b > c) {
        } else {
            b += c;
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