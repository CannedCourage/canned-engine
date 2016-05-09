#ifndef _IPROCESS_H_
#define _IPROCESS_H_

#include "Engine/Time.h"

class iProcess
{
private:
protected:
public:

	virtual void Start( void ) = 0;
	virtual void Update( const EngineDuration& DeltaT ) = 0;
	virtual void End( void ) = 0;
};

#endif _IPROCESS_H_