#include <iostream>
#include <cstring>
#include "MemoryTool.h"

int main() {
    MemoryTool memory_tool;
    const char* game_package = "com.axlebolt.standoff2";

    int pid = memory_tool.getPID(game_package);
    if (pid == -1) {
        std::cerr << "Failed to get PID for package: " << game_package << std::endl;
        return 1;
    }

    PMAPS maps = memory_tool.readmaps(pid);
    if (maps == nullptr) {
        std::cerr << "Failed to read memory maps." << std::endl;
        return 1;
    }

    long int base_address = 0;
    for (int i = 0; i < maps->count; i++) {
        if (strstr(maps->entries[i].name, "libil2cpp.so") != nullptr) {
            base_address = strtoul(maps->entries[i].start, nullptr, 16);
            std::cout << "Base address of libil2cpp.so: " << std::hex << base_address << std::endl;
            break;
        }
    }

    if (base_address == 0) {
        std::cerr << "libil2cpp.so not found in memory maps." << std::endl;
        return 1;
    }

    long int offset = 0x13D5800;
    long int target_address = base_address + offset;

    unsigned char current_value;
    if (memory_tool.MemoryRead(target_address, &current_value, sizeof(current_value))) {
        std::cout << "Current value at offset: " << (int)current_value << std::endl;
    }
    else {
        std::cerr << "Failed to read memory at target address." << std::endl;
        return 1;
    }

    unsigned char new_value = 0x00, 0x00, 0x00, 0x3E, 0x32, 0xF3, 0x2F, 0xE1, 0xF0, 0x41, 0x2D, 0xE9, 0xA0, 0xD0, 0x4D, 0xE2;
    if (memory_tool.MemoryWrite(target_address, &new_value, sizeof(new_value))) {
        std::cout << "Successfully wrote new value to memory." << std::endl;
    }
    else {
        std::cerr << "Failed to write memory at target address." << std::endl;
        return 1;
    }

    memory_tool.ClearResults();
    return 0;
}
