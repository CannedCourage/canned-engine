//Settings class

#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#include <iostream>
#include <JsonBox.h>

class Settings
{
public:

	Settings( const char* _filename = "", const char* _delimiter = "/" );
	~Settings( void );
	
	void SetDouble( std::string name, const double& value );
	void SetInteger( std::string name, const int& value );
	void SetString( std::string name, const std::string& value );
	void SetBool( std::string name, const bool value );

	void SetDouble( const char* name, const double& value );
	void SetInteger( const char* name, const int& value );
	void SetString( const char* name, const std::string& value );
	void SetBool( const char* name, const bool value );

	double GetDouble( std::string name );
	int GetInteger( std::string name );
	std::string GetString( std::string name );
	bool GetBool( std::string name );

	double GetDouble( const char* name );
	int GetInteger( const char* name );
	std::string GetString( const char* name );
	bool GetBool( const char* name );
	
	void WriteFile( void );
	void ReadFile( void );
protected:

	const char* filename;
	JsonBox::Object root;
	std::string delimiter;
	
	void SetDoubleInObject( std::string& name, const double& value, JsonBox::Value& object );
	void SetIntegerInObject( std::string& name, const int& value, JsonBox::Value& object );
	void SetStringInObject( std::string& name, const std::string& value, JsonBox::Value& object );
	void SetBoolInObject( std::string& name, const bool value, JsonBox::Value& object );
	
	double GetDoubleFromObject( std::string& name, const JsonBox::Value& object );
	int GetIntegerFromObject( std::string& name, const JsonBox::Value& object );
	std::string GetStringFromObject( std::string& name, const JsonBox::Value& object );
	bool GetBoolFromObject( std::string& name, const JsonBox::Value& object );
private:

};

#endif //_SETTINGS_H_