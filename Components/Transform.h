#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

class Transform
{
private:
protected:

	//Local
	//Translation
	//Rotation
	//Scaling

	//Global/World
	//Translation
	//Rotation
	//Scaling

	//Matrix order would be, local scaling, translation, rotation
	//Followed by global translation, rotation, scaling

	//Calculate locals and globals seperately, recalculate based on changes to one of the three matrices
public:

	Transform( void );
	~Transform( void );
};

#endif //_TRANSFORM_H_