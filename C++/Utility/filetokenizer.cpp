#include "Utility/filetokenizer.hpp"

FileTokenizer::FileTokenizer (FileDescriptor filename) :
    mDescriptor (filename)
{
    QString fname = filename.fileDirectory + "/" + filename.fileSubDirectory + "/" + filename.fileBasename + "." + filename.fileExtension;
    mFile = fopen( fname.toStdString().c_str (), "r");
}

FileTokenizer::~FileTokenizer () {
    fclose ( mFile );
}

bool FileTokenizer::feof () {
    return ::feof ( mFile );
}

void FileTokenizer::reset() {
    QString fname = mDescriptor.fileDirectory + "/" + mDescriptor.fileSubDirectory + "/" + mDescriptor.fileBasename + "." + mDescriptor.fileExtension;
    fclose ( mFile );
    mFile = fopen( fname.toStdString().c_str (), "r");
}

QString FileTokenizer::readLine () {
    fgets ( mCharLine, 1024, mFile );
    return mCharLine;
}

QStringList FileTokenizer::readTokens (bool doGetLine) {
    if ( doGetLine )
        fgets ( mCharLine, 1024, mFile );
    mQTLine = mCharLine;
    mQTLine = mQTLine.trimmed ();
    mTokens = mQTLine.split ( " ", QString::SkipEmptyParts );
    return mTokens;
}
