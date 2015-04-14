#include "fuzzylogicbehaviour.hpp"

////////////////////////////

FuzzyLogicTerm::FuzzyLogicTerm ()
{
    _createAttributes();
}

FuzzyLogicTerm::FuzzyLogicTerm ( QString name )
{
    HasName::Initialize( name );
    _createAttributes();
}

void FuzzyLogicTerm::_createAttributes ()
{
    addAttribute < unsigned int > ( "#Keypoints", 0 );
    addAttribute < KeyPoint* > ( "Keypoints", NULL );
}

ByteBuffer FuzzyLogicTerm::_toBuffer ()
{
    unsigned long long totalDataSize = sizeof ( unsigned int ) + get < unsigned int > ( "#Keypoints" ) * sizeof ( KeyPoint ), index = 0;
    ByteBuffer buffer ( totalDataSize );
    index = ::toBuffer ( buffer, index, get < unsigned int > ( "#Keypoints" ) );
    index = ::toBuffer ( buffer, index, get < KeyPoint* > ( "Keypoints" ), get < unsigned int > ( "#Keypoints" ) * sizeof ( KeyPoint ) );
    return buffer;
}

unsigned long long FuzzyLogicTerm::_fromBuffer ( const ByteBuffer& buffer, unsigned long long index )
{
    unsigned long long lindex = index;
    return lindex;
}

////////////////////////////

FuzzyLogicVariable::FuzzyLogicVariable ()
{
    _createAttributes();
}

FuzzyLogicVariable::FuzzyLogicVariable ( QString name )
{
    HasName::Initialize( name );
    _createAttributes();
}

void FuzzyLogicVariable::_createAttributes ()
{
    addAttribute < float > ( "Minimum", 0.0f );
    addAttribute < float > ( "Maximum", 1.0f );
    addNamedAttribute < FuzzyLogicTerm* > ( "Terms" );
}

ByteBuffer FuzzyLogicVariable::_toBuffer ()
{
    ByteBuffer terms;
    foreach ( QString name, getKeys < FuzzyLogicTerm* > ( "Terms" ) ) {
        FuzzyLogicTerm* var = get < FuzzyLogicTerm* > ( "Terms", name );
        ByteBuffer varBuf = var->convertToBuffer ();
        terms.append(varBuf);
    }
    unsigned long long totalSize = 2 * sizeof ( float ) + sizeof ( unsigned int ) + terms.getLength(), index = 0;
    ByteBuffer result (totalSize);
    index = ::toBuffer ( result, index, getSize < float > ( "Minimum" ) );
    index = ::toBuffer ( result, index, getSize < float > ( "Maximum" ) );
    index = ::toBuffer ( result, index, getSize < FuzzyLogicVariable* > ( "Terms" ) );
    index = ::toBuffer ( result, index, terms.getData(), terms.getLength() );
    return result;
}

unsigned long long FuzzyLogicVariable::_fromBuffer ( const ByteBuffer& buffer, unsigned long long index )
{
    unsigned long long lindex = index;
    return lindex;
}

////////////////////////////

FuzzyLogicProposition::FuzzyLogicProposition ()
{
    _createAttributes();
}

void FuzzyLogicProposition::_createAttributes ()
{
    addAttribute < QString > ( "Variable", "" );
    addAttribute < QString > ( "Term", "" );
}

ByteBuffer FuzzyLogicProposition::_toBuffer ()
{
    ByteBuffer var = ::toBuffer ( get<QString>("Variable") );
    ByteBuffer term = ::toBuffer ( get<QString>("Term") );
    unsigned long long totalDataSize = var.getLength()+term.getLength(), index = 0;
    ByteBuffer buffer ( totalDataSize );
    index = ::toBuffer ( buffer, index, var.getData(), var.getLength() );
    index = ::toBuffer ( buffer, index, term.getData(), term.getLength() );
    return buffer;
}

unsigned long long FuzzyLogicProposition::_fromBuffer ( const ByteBuffer& buffer, unsigned long long index )
{
    unsigned long long lindex = index;
    return lindex;
}

////////////////////////////

FuzzyLogicRule::FuzzyLogicRule ()
{
    _createAttributes();
}

void FuzzyLogicRule::_createAttributes ()
{
    addAttribute < FuzzyLogicProposition* > ( "Antecedent", NULL );
    addAttribute < FuzzyLogicProposition* > ( "Consequent", NULL );
    addAttribute < float > ( "Weight", 1.0 );
}

ByteBuffer FuzzyLogicRule::_toBuffer ()
{
    ByteBuffer ante = get < FuzzyLogicProposition* > ( "Antecedent" )->convertToBuffer();
    ByteBuffer cons = get < FuzzyLogicProposition* > ( "Consequent" )->convertToBuffer();
    unsigned long long totalDataSize = ante.getLength() + cons.getLength() + sizeof ( float ), index = 0;
    ByteBuffer result ( totalDataSize );
    index = ::toBuffer ( result, index, ante.getData(), ante.getLength() );
    index = ::toBuffer ( result, index, cons.getData(), cons.getLength() );
    index = ::toBuffer ( result, index, get < float > ( "Weight" ) );
    return result;
}

unsigned long long FuzzyLogicRule::_fromBuffer ( const ByteBuffer& buffer, unsigned long long index )
{
    unsigned long long lindex = index;
    return lindex;
}

////////////////////////////

FuzzyLogicRuleBlock::FuzzyLogicRuleBlock ()
{
    _createAttributes();
}

FuzzyLogicRuleBlock::FuzzyLogicRuleBlock ( QString name )
{
    HasName::Initialize( name );
    _createAttributes();
}

void FuzzyLogicRuleBlock::_createAttributes ()
{
    addIndexedAttribute < FuzzyLogicRule* > ( "Rules" );
}

ByteBuffer FuzzyLogicRuleBlock::_toBuffer ()
{
    ByteBuffer rules;
    for ( unsigned int i = 0 ; i < getSize < FuzzyLogicRule* > ( "Rules" ) ; i ++ ) {
        FuzzyLogicRule* grp = get < FuzzyLogicRule* > ( "Rules", i );
        ByteBuffer grpBuf = grp->convertToBuffer ();
        rules.append(grpBuf);
    }
    unsigned long long totalDataSize = sizeof ( unsigned int ) + rules.getLength(), index = 0;
    ByteBuffer result ( totalDataSize );
    index = ::toBuffer ( result, index, getSize < FuzzyLogicRule* > ( "Rules" ) );
    index = ::toBuffer ( result, index, rules.getData(), rules.getLength() );
    return result;
}

unsigned long long FuzzyLogicRuleBlock::_fromBuffer ( const ByteBuffer& buffer, unsigned long long index )
{
    unsigned long long lindex = index;
    return lindex;
}

////////////////////////////

FuzzyLogicBehaviour::FuzzyLogicBehaviour ()
{
    _createAttributes ();
}

FuzzyLogicBehaviour::FuzzyLogicBehaviour (QString name)
{
    HasName::Initialize( name );
    _createAttributes ();
}

FuzzyLogicBehaviour::~FuzzyLogicBehaviour ()
{
}

void FuzzyLogicBehaviour::_createAttributes ()
{
    addNamedAttribute < FuzzyLogicVariable* > ( "Inputs" );
    addNamedAttribute < FuzzyLogicVariable* > ( "Outputs" );
    addNamedAttribute < FuzzyLogicRuleBlock* > ( "RuleBlocks" );
}

void FuzzyLogicBehaviour::Usage()
{
    std::cout << getUUID().toString().toStdString() <<
                 " && " << getName ().toStdString() << std::endl;
}

bool FuzzyLogicBehaviour::fillFromTokenizer(FileTokenizer& theFile)
{
    QStringList m_tokens = theFile.getTokens();
    while ( true ) {
        if ( theFile.feof() ) break;
        m_tokens = theFile.readTokens ();
    }
    return true;
}

ByteBuffer FuzzyLogicBehaviour::_toBuffer () {
    ByteBuffer inputs, outputs, ruleblocks;
    foreach ( QString name, getKeys < FuzzyLogicVariable* > ( "Inputs" ) ) {
        FuzzyLogicVariable* var = get < FuzzyLogicVariable* > ( "Inputs", name );
        ByteBuffer varBuf = var->convertToBuffer ();
        inputs.append(varBuf);
    }
    foreach ( QString name, getKeys < FuzzyLogicVariable* > ( "Outputs" ) ) {
        FuzzyLogicVariable* var = get < FuzzyLogicVariable* > ( "Outputs", name );
        ByteBuffer varBuf = var->convertToBuffer ();
        outputs.append(varBuf);
    }
    foreach ( QString name, getKeys < FuzzyLogicRuleBlock* > ( "RuleBlocks" ) ) {
        FuzzyLogicRuleBlock* var = get < FuzzyLogicRuleBlock* > ( "RuleBlocks", name );
        ByteBuffer varBuf = var->convertToBuffer ();
        ruleblocks.append(varBuf);
    }
    unsigned long long totalSize = 3 * sizeof ( unsigned int ) + inputs.getLength() + outputs.getLength() + ruleblocks.getLength(), index = 0;
    ByteBuffer result (totalSize);
    index = ::toBuffer ( result, index, getSize < FuzzyLogicVariable* > ( "Inputs" ) );
    index = ::toBuffer ( result, index, inputs.getData(), inputs.getLength() );
    index = ::toBuffer ( result, index, getSize < FuzzyLogicVariable* > ( "Outputs" ) );
    index = ::toBuffer ( result, index, outputs.getData(), outputs.getLength() );
    index = ::toBuffer ( result, index, getSize < FuzzyLogicRuleBlock* > ( "RuleBlocks" ) );
    index = ::toBuffer ( result, index, ruleblocks.getData(), ruleblocks.getLength() );
    return result;
}

unsigned long long FuzzyLogicBehaviour::_fromBuffer ( const ByteBuffer& buffer, unsigned long long index ) {
    unsigned long long lindex = index;
    return lindex;
}
