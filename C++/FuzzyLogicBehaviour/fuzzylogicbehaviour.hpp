#ifndef FUZZYLOGICBEHAVIOUR_H
#define FUZZYLOGICBEHAVIOUR_H

#include "Assets/assets.hpp"

typedef struct {
    float x, y, tangeantIn, tangentOut;
} KeyPoint;

class FuzzyLogicTerm :
        virtual public IResource
{
public:
    FuzzyLogicTerm ();
    FuzzyLogicTerm ( QString name );
protected:
    virtual void _createAttributes ();
    virtual ByteBuffer _toBuffer ();
    virtual unsigned long long _fromBuffer ( const ByteBuffer& buffer, unsigned long long index = 0 );
};

class FuzzyLogicVariable :
        virtual public IResource
{
public:
    FuzzyLogicVariable ();
    FuzzyLogicVariable ( QString name );
protected:
    virtual void _createAttributes ();
    virtual ByteBuffer _toBuffer ();
    virtual unsigned long long _fromBuffer ( const ByteBuffer& buffer, unsigned long long index = 0 );
};

class FuzzyLogicProposition :
        virtual public IResource
{
public:
    FuzzyLogicProposition ();
protected:
    virtual void _createAttributes ();
    virtual ByteBuffer _toBuffer ();
    virtual unsigned long long _fromBuffer ( const ByteBuffer& buffer, unsigned long long index = 0 );
};

class FuzzyLogicRuleBlock :
        virtual public IResource
{
public:
    FuzzyLogicRuleBlock ();
    FuzzyLogicRuleBlock ( QString name );
protected:
    virtual void _createAttributes ();
    virtual ByteBuffer _toBuffer ();
    virtual unsigned long long _fromBuffer ( const ByteBuffer& buffer, unsigned long long index = 0 );
};

class FuzzyLogicRule :
        virtual public IResource
{
public:
    FuzzyLogicRule ();
protected:
    virtual void _createAttributes ();
    virtual ByteBuffer _toBuffer ();
    virtual unsigned long long _fromBuffer ( const ByteBuffer& buffer, unsigned long long index = 0 );
};

class FuzzyLogicBehaviour :
        virtual public Assets::Behaviour,
        virtual public Tokenized
{
public:
    virtual ~FuzzyLogicBehaviour ();
    virtual void Usage ();
protected:
    FuzzyLogicBehaviour ();
    FuzzyLogicBehaviour (QString name);
    virtual void _createAttributes ();
    virtual bool fillFromTokenizer(FileTokenizer& theFile);
    virtual ByteBuffer _toBuffer ();
    virtual unsigned long long _fromBuffer ( const ByteBuffer& buffer, unsigned long long index = 0 );
private:
    friend class FuzzyLogicBehaviourLoader;
    friend class ResourceDescriptor < FuzzyLogicBehaviour >;
};

#endif // FUZZYLOGICBEHAVIOUR_H
