#ifndef _RECT_H_
#define _RECT_H_

#include "Maths\Vector.h"

class Rect
{
private:
protected:
public:

	float top, left, bottom, right;

	Rect( void );
	~Rect( void );

	Vector topLeft( void );
	Vector bottomLeft( void );
	Vector topRight( void );
	Vector bottomRight( void );
};

#endif //_RECT_H_