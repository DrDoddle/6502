#include <iostream>

#include "Doodle6502.h"

int main()
{
	Doodle6502* c6502 = new Doodle6502;

    while (true) {
        std::cout << "A: " << std::hex << +c6502->A << std::endl;
        std::cout << "X: " << std::hex << +c6502->X << std::endl;
        std::cout << "Y: " << std::hex << +c6502->Y << std::endl;
        std::cout << "SP: " << std::hex << +c6502->sp << std::endl;
        std::cout << "PC: " << std::hex << +c6502->pc << std::endl;
        std::cout << "P:  " << std::bitset<8>(c6502->p) << " (NV-BDIZC)" << std::endl;

        std::cout << "Debug Menu:\n";
        std::cout << "1. Step Instruction\n";
        std::cout << "2. Inspect Memory\n";
        std::cout << "3. Modify Memory\n";
        std::cout << "4. Modify Register\n";
        std::cout << "5. Exit Debug Menu\n";
        std::cout << "Enter choice: ";

        int choice;
        std::cin >> choice;

        switch (choice) {
        case 1:
            c6502->executeInstruction();
            break;
        default:
            std::cout << "Invalid choice. Try again.\n";
        }
    }



	std::cin.get();
	delete c6502;

	return 0;
}