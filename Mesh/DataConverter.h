#ifndef _DATA_CONVERTER_H_
#define _DATA_CONVERTER_H_

#include <vector>

class DataConverter
{
protected:
    D3DDECLUSAGE mUsageType;
    int mUsageIndex;
    int mOffset;
 
public:
    DataConverter( D3DDECLUSAGE usageType, int usageIndex, int offsetInBytes )
        : mUsageType( usageType )
        , mUsageIndex( usageIndex )
        , mOffset( offsetInBytes )
    {}
 
    virtual int Size() = 0;
    virtual void CopyType( std::vector<D3DVERTEXELEMENT9>& out_Type ) = 0;
    virtual void CopyData( BYTE* destination, int index ) = 0;
};

#endif //_DATA_CONVERTER_H_