#include "AssetGenerics/generics.hpp"

void __attachGenerics(void) __attribute__((constructor));
void __detachGenerics(void) __attribute__((destructor));

void __attachGenerics(void)
{
    std::cout << "Attaching AssetGenerics library" << std::endl;
}

void __detachGenerics(void)
{
    std::cout << "Detaching AssetGenerics library" << std::endl;
}
