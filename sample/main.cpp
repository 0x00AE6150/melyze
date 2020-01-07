#include <cstdint>
#include <cstdio>

int main(int argc, char* argv[]) {
    uint64_t* hackMe = new uint64_t[1] { 0 };

    printf("Address: 0x%llx\n", (uint64_t)hackMe);

    while (1) {
        printf("Tell me a number: ");
        uint64_t new_value;
        scanf("%ld", &new_value);
        printf("Current number is %ld\n", *hackMe);
        *hackMe = new_value;
    }
}
