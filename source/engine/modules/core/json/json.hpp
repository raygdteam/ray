#pragma once
#include <iostream>
#include <vector>
#include <map>

#include <core/lib/array.hpp>
#include <core/lib/string.hpp>

enum JsonValueType
{
	eJsonString,
	eJsonObject,
	eJsonArray,
	eJsonBoolean,
	eJsonNumber,
	eJsonNull,
	eJsonUnknown
};

class RAY_CORE_API JsonValue
{
	String MakeSpace(int);
	String _stringValue;
	JsonValueType _type;
	std::vector<std::pair<String, JsonValue>> _properties;
	std::map<std::string, size_t> _index;
	std::vector<JsonValue> _array;
	String to_string_d(int);
public:
	JsonValue();
	JsonValue(JsonValueType);
	String ToString();
	JsonValueType GetType();
	void SetType(JsonValueType);
	void AddProperty(String key, JsonValue v);
	void AddElement(JsonValue v);
	void SetString(String s);
	int AsInt();
	double AsDouble();
	bool AsBool();
	void* AsNull();
	String AsString();
	u32 Size();
	JsonValue& operator[](int i);
	JsonValue& operator[](String s);
	JsonValue& operator[](const char* const str);
};

class RAY_CORE_API JsonParser
{
private:
	enum TokenType
	{
		UNKNOWN,
		STRING,
		NUMBER,
		CROUSH_OPEN,
		CROUSH_CLOSE,
		BRACKET_OPEN,
		BRACKET_CLOSE,
		COMMA,
		COLON,
		BOOLEAN,
		NUL
	};

	struct Token;
	static bool IsWhitespace(const char c);
	static int NextWhitespace(const String& source, int i);
	static int SkipWhitespaces(const String& source, int i);

	static std::vector<Token> Tokenize(String source);
	static JsonValue JsonParse(std::vector<Token> v, int i, int& r);
public:
	static JsonValue Parse(const String& str);
};
