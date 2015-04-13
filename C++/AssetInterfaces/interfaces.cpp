#include "AssetInterfaces/interfaces.hpp"

void __attachInterfaces(void) __attribute__((constructor));
void __detachInterfaces(void) __attribute__((destructor));

void __attachInterfaces(void)
{
    std::cout << "Attaching AssetInterfaces library" << std::endl;
}

void __detachInterfaces(void)
{
    std::cout << "Detaching AssetInterfaces library" << std::endl;
}

