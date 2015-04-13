#ifndef FILEDESCRIPTOR_H
#define FILEDESCRIPTOR_H

#include "Utility/includes.hpp"

class FileDescriptor
{
public:
    FileDescriptor () {}
    FileDescriptor (QString filename, QString baseDirectory = "");
    QString fileDirectory;
    QString fileSubDirectory;
    QString fileBasename;
    QString fileExtension;
    QString fullFilename;
};

#endif // FILEDESCRIPTOR_H
