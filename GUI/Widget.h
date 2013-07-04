#ifndef _WIDGET_H_
#define _WIDGET_H_

#include "Maths\Vector.h"

class Widget
{
private:
protected:

	Vector position;
public:

	Widget( void );
	~Widget( void );

	void Load( void );
	void Unload( void );
	void Init( void );
	void Update( void );
	void Render( void );
	void CleanUp( void );

	void Move( float x, float y, float z );
};

#endif //_WIDGET_H_