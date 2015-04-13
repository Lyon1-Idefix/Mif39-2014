#include "Utility/filedescriptor.hpp"

FileDescriptor::FileDescriptor (QString filename, QString baseDirectory) :
    fullFilename ( filename )
{
    QFileInfo fi ( filename );
    fileDirectory = fi.canonicalPath();
    if ( baseDirectory != "" ) {
        fileDirectory.replace(baseDirectory, "");
        fileSubDirectory = fileDirectory;
        fileDirectory = baseDirectory;
    }
    fileBasename = fi.baseName();
    fileExtension = fi.completeSuffix();
    fullFilename = fileDirectory + "/" + fileSubDirectory + "/" + fileBasename + "." + fileExtension;
}

