#ifndef FILETOKENIZER_H
#define FILETOKENIZER_H

#include "Utility/includes.hpp"
#include "Utility/filedescriptor.hpp"

class FileTokenizer
{
public:
    FileTokenizer (FileDescriptor filename);
    ~FileTokenizer ();
    void reset ();
    bool feof ();
    QString readLine ();
    QStringList readTokens (bool doGetLine = true);
    inline QString getLine () { return mQTLine; }
    inline QStringList getTokens () { return mTokens; }
    FileDescriptor mDescriptor;
protected:
    FILE* mFile;
    char mCharLine [ 1024 ];
    QString mQTLine;
    QStringList mTokens;
};

#endif // FILETOKENIZER_H
