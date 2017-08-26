#pragma once

#ifndef _DEBUG_H_
#define _DEBUG_H_

#define ASSERT _ASSERTE
 
#ifdef _DEBUG
#define VERIFY ASSERT
#else
#define VERIFY(expression) (expression)
#endif

#endif