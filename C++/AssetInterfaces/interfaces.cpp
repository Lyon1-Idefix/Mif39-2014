#include "AssetInterfaces/interfaces.hpp"

void __attach(void) __attribute__((constructor));
void __detach(void) __attribute__((destructor));

void __attach(void)
{
    std::cout << "Attaching AssetInterfaces library" << std::endl;
}

void __detach(void)
{
    std::cout << "Detaching AssetInterfaces library" << std::endl;
}

