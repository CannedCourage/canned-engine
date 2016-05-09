#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#include <iostream>
#include <string>
#include <JsonBox.h>

class Settings
{
private:
protected:

	const std::string Filename;
	
	std::string Delimiter;

	JsonBox::Object Root{};
	
	void SetDoubleInObject( std::string& Name, const double& Val, JsonBox::Value& Obj );
	void SetIntegerInObject( std::string& Name, const int& Val, JsonBox::Value& Obj );
	void SetStringInObject( std::string& Name, const std::string& Val, JsonBox::Value& Obj );
	void SetBoolInObject( std::string& Name, const bool& Val, JsonBox::Value& Obj );
	
	double GetDoubleFromObject( std::string& Name, const JsonBox::Value& Obj );
	int GetIntegerFromObject( std::string& Name, const JsonBox::Value& Obj );
	std::string GetStringFromObject( std::string& Name, const JsonBox::Value& Obj );
	bool GetBoolFromObject( std::string& Name, const JsonBox::Value& Obj );
public:

	Settings( const std::string& Filename = "", const std::string& Delimiter = "/" );
	~Settings( void );
	
	void SetDouble( const std::string& Name, const double& Val );
	void SetInteger( const std::string& Name, const int& Val );
	void SetString( const std::string& Name, const std::string& Val );
	void SetBool( const std::string& Name, const bool Val );

	double GetDouble( const std::string& Name );
	int GetInteger( const std::string& Name );
	std::string GetString( const std::string& Name );
	bool GetBool( const std::string& Name );

	void WriteFile( void );
	void ReadFile( void );
};

#endif //_SETTINGS_H_