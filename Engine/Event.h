#ifndef _EVENT_H_
#define _EVENT_H_

#include <vector>
#include <functional>
#include <algorithm>

template<typename ReturnType, typename... Arguments>
size_t GetFunctionAddress( std::function<ReturnType ( Arguments... )> Function )
{
	typedef ReturnType ( FunctionType )( Arguments... );

	/*
	In a template definition, a dependent name that is not a member of the current instantiation
	is not considered to be a template name unless the disambiguation keyword "template" is used or unless
	it was already established as a template name:
	*/
	FunctionType ** functionPointerPointer = Function.template target<FunctionType*>();

	return (size_t) *functionPointerPointer;
}

template<class T>
class Event;

template<typename ReturnType, typename... Arguments>
class Event<ReturnType ( Arguments... )>
{
	using DelegateType = std::function<ReturnType ( Arguments... )>;
protected:

	std::vector<DelegateType> EventHandlers;
public:

	void operator+=( DelegateType Handler )
	{
		//Check if already in list
		auto result = std::find_if( EventHandlers.begin(), EventHandlers.end(), [ &Handler ]( DelegateType& Function )
		{
			if( GetFunctionAddress( Handler ) == GetFunctionAddress( Function ) )
			{
				return true;
			}
			else
			{
				return false;
			}
		});

		if( result == EventHandlers.end() )
		{
			EventHandlers.push_back( Handler );
		}
	}

	void operator-=( DelegateType Handler )
	{
		//Check if already in list
		auto result = std::find_if( EventHandlers.begin(), EventHandlers.end(), [ &Handler ]( DelegateType& Function )
		{
			if( GetFunctionAddress( Handler ) == GetFunctionAddress( Function ) )
			{
				return true;
			}
			else
			{
				return false;
			}
		});

		if( result != EventHandlers.end() )
		{
			EventHandlers.erase( result );
		}
	}

	void operator()( Arguments... Args )
	{
		for( auto handler : EventHandlers )
		{
			handler( Args... );
		}
	}
};

#endif //_EVENT_H_

//Check out this for another implementation
//http://stackoverflow.com/questions/23973914/c-like-delegates-in-c

//Use the following to compare function addresses:
//http://stackoverflow.com/questions/20833453/comparing-stdfunctions-for-equality

//How a variable number of template arguments are used
//http://en.cppreference.com/w/cpp/language/parameter_pack

//How the template works
//http://en.cppreference.com/w/cpp/language/partial_specialization
//http://stackoverflow.com/questions/2355816/transferring-signature-of-the-method-as-template-parameter-to-a-class