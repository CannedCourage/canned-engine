#ifndef _ASSIMP_DATA_CONVERTER_H_
#define _ASSIMP_DATA_CONVERTER_H_

#include "Mesh\ArrayDataConverter.h"

struct aiVector3DToFloat2Converter : public ArrayDataConverter<aiVector3D>
{
    aiVector3DToFloat2Converter( D3DDECLUSAGE usageType, int usageIndex,
        int& offsetInBytes, const aiVector3D* sourceData, int sourceSize )
    : ArrayDataConverter<aiVector3D>( usageType, usageIndex, offsetInBytes, 
        sourceData, sourceSize ) 
    {}
 
    int Size() { return 2 * sizeof(float); }
 
    virtual void CopyType( std::vector<D3DVERTEXELEMENT9>& out_Type )
    {
        D3DVERTEXELEMENT9 _result = 
        {
            0, mOffset, D3DDECLTYPE_FLOAT2,
            D3DDECLMETHOD_DEFAULT, mUsageType, mUsageIndex 
        };
 
        out_Type.push_back( _result );
    }
 
    void CopyData( BYTE* destination, int elementIndex )
    {
        const aiVector3D& element = GetElement( elementIndex );
 
        float data[] = { element[0], element[1] };
        memcpy( destination + mOffset, data, Size() );
    }
};

#endif //_ASSIMP_DATA_CONVERTER_H_