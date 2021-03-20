#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <fstream>
#include <cstring>

#include "json.hpp"

String deserialize(const String& ref)
{
	String out("");
	
	for (size_t i = 0; i < ref.length(); i++)
	{
		if (ref[i] == '\\' && i + 1 < ref.length())
		{
			int plus = 2;
			if (ref[i + 1] == '\"')
			{
				out += '"';
			}
			else if (ref[i + 1] == '\\')
			{
				out += '\\';
			}
			else if (ref[i + 1] == '/')
			{
				out += '/';
			}
			else if (ref[i + 1] == 'b')
			{
				out += '\b';
			}
			else if (ref[i + 1] == 'f')
			{
				out += '\f';
			}
			else if (ref[i + 1] == 'n')
			{
				out += '\n';
			}
			else if (ref[i + 1] == 'r')
			{
				out += '\r';
			}
			else if (ref[i + 1] == 't')
			{
				out += '\t';
			}
			else if (ref[i + 1] == 'u' && i + 5 < ref.length())
			{
				unsigned long long v = 0;
				for (int j = 0; j < 4; j++)
				{
					v *= 16;
					if (ref[i + 2 + j] <= '9' && ref[i + 2 + j] >= '0') v += ref[i + 2 + j] - '0';
					if (ref[i + 2 + j] <= 'f' && ref[i + 2 + j] >= 'a') v += ref[i + 2 + j] - 'a' + 10;
				}
				out += (char)v;
				plus = 6;
			}
			i += plus - 1;
			continue;
		}
		out += ref[i];
	}
	return out;
}

String JsonValue::MakeSpace(int d)
{
	String s("");
	while (d--) s += "  ";
	return s;
}

String JsonValue::to_string_d(int d)
{
	if (_type == eJsonString) return String(String("\"") + String(_stringValue.c_str()) + String("\""));
	if (_type == eJsonNumber) return String(_stringValue.c_str());
	if (_type == eJsonBoolean) return String(_stringValue.c_str());
	if (_type == eJsonNull) return String("null");
	if (_type == eJsonObject)
	{
		String s = String("{\n");
		for (size_t i = 0; i < _properties.size(); i++)
		{
			s += MakeSpace(d) + String("\"") + String(_properties[i].first.c_str()) + String("\": ") + _properties[i].second.
				to_string_d(d + 1) + String(i == _properties.size() - 1 ? "" : ",") + String("\n");
		}
		s += MakeSpace(d - 1) + String("}");
		return s;
	}
	if (_type == eJsonArray)
	{
		String s("[");
		for (size_t i = 0; i < _array.size(); i++)
		{
			if (i) s += ", ";
			s += _array[i].to_string_d(d + 1);
		}
		s += "]";
		return s;
	}
	return String("##");
}

JsonValue::JsonValue()
{
	_type = eJsonUnknown;
}

JsonValue::JsonValue(JsonValueType tp)
{
	_type = tp;
}

String JsonValue::ToString()
{
	return to_string_d(1);
}

JsonValueType JsonValue::GetType()
{
	return _type;
}

void JsonValue::SetType(JsonValueType tp)
{
	_type = tp;
}

void JsonValue::AddProperty(String key, JsonValue v)
{
	_index[key.AsRawStr()] = _properties.size();
	_properties.push_back(std::make_pair(key, v));
}

void JsonValue::AddElement(JsonValue v)
{
	_array.push_back(v);
}

void JsonValue::SetString(String s)
{
	_stringValue = s;
}

int JsonValue::AsInt()
{
	std::stringstream ss;
	ss << std::string(_stringValue.c_str());
	int k;
	ss >> k;
	return k;
}

double JsonValue::AsDouble()
{
	std::stringstream ss;
	ss << std::string(_stringValue.c_str());
	double k;
	ss >> k;
	return k;
}

bool JsonValue::AsBool()
{
	if (_stringValue == "true") return true;
	return false;
}

void* JsonValue::AsNull()
{
	return NULL;
}

String JsonValue::AsString()
{
	return deserialize(String(_stringValue.c_str()));
}

u32 JsonValue::Size()
{
	if (_type == eJsonArray)
	{
		return (u32)_array.size();
	}
	if (_type == eJsonObject)
	{
		return (u32)_properties.size();;
	}
	return 0;
}

JsonValue& JsonValue::operator[](int i)
{
	if (_type == eJsonArray)
	{
		return _array[i];
	}
	if (_type == eJsonObject)
	{
		return _properties[i].second;
	}
	static JsonValue empty{};
	return empty;
}

JsonValue& JsonValue::operator[](String s)
{
	check(_index.find(s.AsRawStr()) != _index.end());
	return _properties[_index[s.AsRawStr()]].second;
}

JsonValue& JsonValue::operator[](const char* const str)
{
	return operator[](String(str));
}

struct JsonParser::Token
{
	String value;
	TokenType type;

	Token(String value = String(""), TokenType type = UNKNOWN) : value(value), type(type)
	{
	}
};

bool JsonParser::IsWhitespace(const char c)
{
	return isspace(c);
}

int JsonParser::NextWhitespace(const String& source, int i)
{
	while (i < (int)source.length())
	{
		if (source[i] == '"')
		{
			i++;
			while (i < (int)source.length() && (source[i] != '"' || source[i - 1] == '\\')) i++;
		}
		if (source[i] == '\'')
		{
			i++;
			while (i < (int)source.length() && (source[i] != '\'' || source[i - 1] == '\\')) i++;
		}
		if (IsWhitespace(source[i])) return i;
		i++;
	}
	return (int)source.length();
}

int JsonParser::SkipWhitespaces(const String& source, int i)
{
	while (i < (int)source.length())
	{
		if (!IsWhitespace(source[i])) return i;
		i++;
	}
	return -1;
}

std::vector<JsonParser::Token> JsonParser::Tokenize(String source)
{
	source += " ";
	std::vector<Token> tokens;
	int index = SkipWhitespaces(source, 0);
	while (index >= 0)
	{
		int next = NextWhitespace(source, index);
		String str = String(source.substr(index, next - index));

		size_t k = 0;
		while (k < str.length())
		{
			if (str[k] == '"')
			{
				size_t tmp_k = k + 1;
				while (tmp_k < str.length() && (str[tmp_k] != '"' || str[tmp_k - 1] == '\\')) tmp_k++;
				tokens.push_back(Token(String(str.substr(k + 1, tmp_k - k - 1)), STRING));
				k = tmp_k + 1;
				continue;
			}
			if (str[k] == '\'')
			{
				size_t tmp_k = k + 1;
				while (tmp_k < str.length() && (str[tmp_k] != '\'' || str[tmp_k - 1] == '\\')) tmp_k++;
				tokens.push_back(Token(String(str.substr(k + 1, tmp_k - k - 1)), STRING));
				k = tmp_k + 1;
				continue;
			}
			if (str[k] == ',')
			{
				tokens.push_back(Token(String(","), COMMA));
				k++;
				continue;
			}
			if (str[k] == 't' && k + 3 < str.length() && str.substr(k, 4) == "true")
			{
				tokens.push_back(Token(String("true"), BOOLEAN));
				k += 4;
				continue;
			}
			if (str[k] == 'f' && k + 4 < str.length() && str.substr(k, 5) == "false")
			{
				tokens.push_back(Token(String("false"), BOOLEAN));
				k += 5;
				continue;
			}
			if (str[k] == 'n' && k + 3 < str.length() && str.substr(k, 4) == "null")
			{
				tokens.push_back(Token(String("null"), NUL));
				k += 4;
				continue;
			}
			if (str[k] == '}')
			{
				tokens.push_back(Token(String("}"), CROUSH_CLOSE));
				k++;
				continue;
			}
			if (str[k] == '{')
			{
				tokens.push_back(Token(String("{"), CROUSH_OPEN));
				k++;
				continue;
			}
			if (str[k] == ']')
			{
				tokens.push_back(Token(String("]"), BRACKET_CLOSE));
				k++;
				continue;
			}
			if (str[k] == '[')
			{
				tokens.push_back(Token(String("["), BRACKET_OPEN));
				k++;
				continue;
			}
			if (str[k] == ':')
			{
				tokens.push_back(Token(String(":"), COLON));
				k++;
				continue;
			}
			if (str[k] == '-' || (str[k] <= '9' && str[k] >= '0'))
			{
				size_t tmp_k = k;
				if (str[tmp_k] == '-') tmp_k++;
				while (tmp_k < str.size() && ((str[tmp_k] <= '9' && str[tmp_k] >= '0') || str[tmp_k] == '.')) tmp_k++;
				tokens.push_back(Token(String(str.substr(k, tmp_k - k)), NUMBER));
				k = tmp_k;
				continue;
			}
			tokens.push_back(Token(String(str.substr(k)), UNKNOWN));
			k = str.length();
		}

		index = SkipWhitespaces(source, next);
	}
	// for (int i=0;i<tokens.size();i++) {
	// cout << i << " " << tokens[i].value << endl;
	// }
	return tokens;
}


JsonValue JsonParser::JsonParse(std::vector<Token> v, int i, int& r)
{
	JsonValue current;
	if (v[i].type == CROUSH_OPEN)
	{
		current.SetType(eJsonObject);
		int k = i + 1;
		while (v[k].type != CROUSH_CLOSE)
		{
			String key = v[k].value;
			k += 2; // k+1 should be ':'
			int j = k;
			JsonValue vv = JsonParse(v, k, j);
			current.AddProperty(key, vv);
			k = j;
			if (v[k].type == COMMA) k++;
		}
		r = k + 1;
		return current;
	}
	if (v[i].type == BRACKET_OPEN)
	{
		current.SetType(eJsonArray);
		int k = i + 1;
		while (v[k].type != BRACKET_CLOSE)
		{
			int j = k;
			JsonValue vv = JsonParse(v, k, j);
			current.AddElement(vv);
			k = j;
			if (v[k].type == COMMA) k++;
		}
		r = k + 1;
		return current;
	}
	if (v[i].type == NUMBER)
	{
		current.SetType(eJsonNumber);
		current.SetString(v[i].value);
		r = i + 1;
		return current;
	}
	if (v[i].type == STRING)
	{
		current.SetType(eJsonString);
		current.SetString(v[i].value);
		r = i + 1;
		return current;
	}
	if (v[i].type == BOOLEAN)
	{
		current.SetType(eJsonBoolean);
		current.SetString(v[i].value);
		r = i + 1;
		return current;
	}
	if (v[i].type == NUL)
	{
		current.SetType(eJsonNull);
		current.SetString(String("null"));
		r = i + 1;
		return current;
	}
	return current;
}

JsonValue JsonParser::Parse(const String& str)
{
	int k;
	return JsonParse(Tokenize(str), 0, k);
}
