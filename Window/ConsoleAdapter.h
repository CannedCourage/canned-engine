#ifndef _CONSOLE_ADAPTER_H_
#define _CONSOLE_ADAPTER_H_

class ConsoleAdapter
{
private:
protected:

	void SetupStandardStreams( void );
public:

	bool RedirectOutput = true;
	bool RedirectInput = true;

	ConsoleAdapter( void ) = default;
	ConsoleAdapter( bool Ouptut, bool Input );
	~ConsoleAdapter( void );

	void CreateConsole( void );
	void DestroyConsole( void );

	void AttachParentConsole( void );
};

#endif //_CONSOLE_ADAPTER_H_