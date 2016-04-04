#ifndef _CONSOLEADAPTER_H_
#define _CONSOLEADAPTER_H_

#define CONSOLE_INPUT 0x00000001
#define CONSOLE_OUTPUT 0x00000010

class ConsoleAdapter
{
private:
protected:
	
	void SetupStandardStreams( void );
public:

	unsigned long ConsoleType;

	ConsoleAdapter( unsigned long type = CONSOLE_INPUT | CONSOLE_OUTPUT );
	~ConsoleAdapter( void );

	void CreateConsole( void );
	void DestroyConsole( void );

	void AttachParentConsole( void );
};

#endif //_CONSOLEADAPTER_H_