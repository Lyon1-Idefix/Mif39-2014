#include "attributecontainer.hpp"

AttributeContainer::AttributeContainer()
{
}

IResourceAttribute& AttributeContainer::operator[] ( QString name ) {
    return *(mAttributes [ name ]);
}
