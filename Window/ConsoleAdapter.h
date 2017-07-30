#ifndef _CONSOLE_ADAPTER_H_
#define _CONSOLE_ADAPTER_H_

class ConsoleAdapter
{
private:
protected:

	void SetupStandardStreams( void );
public:

	bool RedirectOutput = true;
	bool RedirectInput = false;

	ConsoleAdapter( void ) = default;
	ConsoleAdapter( bool Ouptut, bool Input );
	~ConsoleAdapter( void );

	void CreateConsole( void );
	void CreateConsole( bool Ouptut, bool Input );
	void DestroyConsole( void );
};

#endif //_CONSOLE_ADAPTER_H_