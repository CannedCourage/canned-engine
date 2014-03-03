#ifndef _ARRAY_DATA_CONVERTER_H_
#define _ARRAY_DATA_CONVERTER_H_

#include "Mesh\DataConverter.h"

template< typename T >
class ArrayDataConverter : public DataConverter
{
    const T* mSourceData;
    const int mSourceSize;
 
public:
    ArrayDataConverter( D3DDECLUSAGE usageType, int usageIndex, int& offsetInBytes,
            const T* sourceData, int sourceSize )
        : DataConverter( usageType, usageIndex, offsetInBytes )
        , mSourceData( sourceData )
        , mSourceSize( sourceSize ) {}
 
    const T& GetElement( int index )
    {
        assert( index >= 0 && index < mSourceSize );
        return mSourceData[index];
    }
};

#endif //_ARRAY_DATA_CONVERTER_H_