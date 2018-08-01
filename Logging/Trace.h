#ifndef _TRACE_H_
#define _TRACE_H_

#include <string>

#ifdef _DEBUG
#define TRACE(expression) Trace(__FILE__,__LINE__,expression)
#else
#define TRACE __noop
#endif

void Trace( const std::string& File, unsigned int Line, const std::string& Msg );

#endif //_TRACE_H_