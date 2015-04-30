
///////////////////////////////////////

template < class InnerType > unsigned long long toBuffer ( ByteBuffer& buffer, unsigned long long index, const InnerType& value )
{
    if ( ( index + sizeof ( InnerType ) ) > buffer.getLength() ) std::cout << "****** SIZE ERROR *******" << std::endl;
    memcpy ( buffer.getData() + index, (void*) & value, sizeof ( InnerType ) );
    return index + sizeof ( InnerType );
}

template < class InnerType > unsigned long long fromBuffer ( const ByteBuffer& buffer, unsigned long long index, InnerType& value )
{
    memcpy ( (void*) & value, buffer.getData() + index, sizeof ( InnerType ) );
    return index + sizeof ( InnerType );
}

template < class InnerType > ByteBuffer toBuffer ( const InnerType& value ) {
    ByteBuffer result ( sizeof ( InnerType ) );
    unsigned int nindex = ::toBuffer ( result, 0, value );
    return result;
}

///////////////////////////////////////

template < class InnerType > unsigned long long toBuffer ( ByteBuffer& buffer, unsigned long long index, const InnerType*& value )
{
    if ( ( index + sizeof ( InnerType ) ) > buffer.getLength() ) std::cout << "****** SIZE ERROR *******" << std::endl;
    memcpy ( buffer.getData() + index, (void*) value, sizeof ( InnerType ) );
    return index + sizeof ( InnerType );
}

template < class InnerType > unsigned long long fromBuffer ( const ByteBuffer& buffer, unsigned long long index, InnerType*& value )
{
    memcpy ( (void*) value, buffer.getData() + index, sizeof ( InnerType ) );
    return index + sizeof ( InnerType );
}

template < class InnerType > ByteBuffer toBuffer ( const InnerType*& value ) {
    ByteBuffer result ( sizeof ( InnerType ) );
    unsigned int nindex = ::toBuffer ( result, 0, *value );
    return result;
}

///////////////////////////////////////

template < class InnerType > unsigned long long toBuffer ( ByteBuffer& buffer, unsigned long long index, const InnerType* value, const unsigned long long length )
{
    InnerType fake; ByteBuffer fakeb = ::toBuffer < InnerType > ( fake );
    if ( ( index + length * fakeb.getLength() ) > buffer.getLength() ) std::cout << "****** SIZE ERROR *******" << std::endl;
    if ( fakeb.getLength() == sizeof ( InnerType ) )
        memcpy ( buffer.getData() + index, (void*) value, length * sizeof ( InnerType ) );
    else {
        unsigned long long lindex = index;
        for ( unsigned long long i = 0 ; i < length ; i ++ )
            lindex = ::toBuffer < InnerType > ( buffer, lindex, value [ i ] );
    }
    return index + length * fakeb.getLength();
}

template < class InnerType > unsigned long long fromBuffer ( const ByteBuffer& buffer, unsigned long long index, InnerType* value, const unsigned long long length )
{
    InnerType fake; ByteBuffer fakeb = ::toBuffer < InnerType > ( fake );
    if ( fakeb.getLength() == sizeof ( InnerType ) )
        memcpy ( (void*) value, buffer.getData() + index, length * sizeof ( InnerType ) );
    else {
        unsigned long long lindex = index;
        for ( unsigned long long i = 0 ; i < length ; i ++ )
            lindex = ::fromBuffer < InnerType > ( buffer, lindex, value [ i ] );
    }
    return index + length * sizeof ( InnerType );
}

template < class InnerType > ByteBuffer toBuffer ( const InnerType* value, const unsigned long long length ) {
    InnerType fake; ByteBuffer fakeb = ::toBuffer < InnerType > ( fake );
    ByteBuffer result ( length * fakeb.getLength() );
    unsigned int nindex = ::toBuffer ( result, 0, value, length );
    return result;
}

///////////////////////////////////////

template < class InnerType > unsigned long long toBuffer ( ByteBuffer& buffer, unsigned long long index, const QList < InnerType >& value )
{
    if ( ( index + sizeof ( unsigned int ) + value.size() * sizeof ( InnerType ) ) > buffer.getLength() ) std::cout << "****** SIZE ERROR *******" << std::endl;
    unsigned int lindex = index;
    lindex = ::toBuffer < unsigned int > ( buffer, lindex, value.size () );
    for ( unsigned int i = 0 ; i < value.size () ; i ++ )
        lindex = ::toBuffer < InnerType > ( buffer, lindex, value [ i ] );
    return lindex;
}

template < class InnerType > unsigned long long fromBuffer ( const ByteBuffer& buffer, unsigned long long index, const QList < InnerType >& value )
{
    unsigned int length;
    unsigned long long lindex = index;
    lindex = ::fromBuffer ( buffer, lindex, length );
    for ( unsigned int i = 0 ; i < length ; i ++ ) {
        InnerType ivalue;
        lindex = ::fromBuffer < InnerType > ( buffer, lindex, ivalue );
        value.append(ivalue);
    }
    return lindex;
}

template < class InnerType > ByteBuffer toBuffer ( const QList < InnerType >& value ) {
    ByteBuffer result ( value.size() * sizeof ( InnerType ) );
    unsigned int nindex = ::toBuffer ( result, 0, value );
    return result;
}

///////////////////////////////////////

template < class InnerType > unsigned long long toBuffer ( ByteBuffer& buffer, unsigned long long index, const QList < InnerType* >& value )
{
    if ( ( index + sizeof ( unsigned int ) + value.size() * sizeof ( InnerType ) ) > buffer.getLength() ) std::cout << "****** SIZE ERROR *******" << std::endl;
    unsigned int lindex = index;
    lindex = ::toBuffer < unsigned int > ( buffer, lindex, value.size () );
    for ( unsigned int i = 0 ; i < value.size () ; i ++ )
        lindex = ::toBuffer < InnerType > ( buffer, lindex, *(value [ i ]) );
    return lindex;
}

template < class InnerType > unsigned long long fromBuffer ( const ByteBuffer& buffer, unsigned long long index, const QList < InnerType* >& value )
{
    unsigned int length;
    unsigned long long lindex = index;
    lindex = ::fromBuffer ( buffer, lindex, length );
    for ( unsigned int i = 0 ; i < length ; i ++ ) {
        InnerType* ivalue = new InnerType;
        lindex = ::fromBuffer < InnerType > ( buffer, lindex, *(ivalue) );
        value.append(ivalue);
    }
    return lindex;
}

template < class InnerType > ByteBuffer toBuffer ( const QList < InnerType* >& value ) {
    ByteBuffer result ( value.size() * sizeof ( InnerType ) );
    unsigned int nindex = ::toBuffer ( result, 0, value );
    return result;
}
