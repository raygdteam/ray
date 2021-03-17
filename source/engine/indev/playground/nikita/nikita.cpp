namespace ray::json
{
	enum type { none, key, colon, obracket, cbracket, boolean };
}

#include <stdio.h> 
#include <malloc.h>

#include <vector>
#include <string>

#include <map>
#include <any>

int main()
{
	auto file = fopen(".json", "rb");

	fseek(file, 0, SEEK_END);
	auto size = ftell(file);
	fseek(file, 0, SEEK_SET);

	char* result = (char*)malloc(size + 1);
	fread(result, sizeof(char), size, file);

	fclose(file);
	result[size] = 0;

	std::vector<std::string> tokens;
	std::vector<ray::json::type> types;

	for (int i = 0; result[i]; i++)
	{
		switch (result[i])
		{
		case 34: {
			std::string key; i++;
			for (bool completed = false; !completed; i++)
			{
				switch (result[i])
				{
				case 34:
					completed = true; i--;
					tokens.push_back(key);
					types.push_back(ray::json::type::key);
					break;
				default:
					key.push_back(result[i]);
					break;
				}
			}
			break;
		}
		case 58: {
			tokens.push_back({ result[i] });
			types.push_back(ray::json::type::colon);
			break;
		}
		case 123: {
			tokens.push_back({ result[i] });
			types.push_back(ray::json::type::obracket);
			break;
		}
		case 125: {
			tokens.push_back({ result[i] });
			types.push_back(ray::json::type::cbracket);
			break;
		}
		case 116:
			i++;
			if (result[i] == 114)
			{
				i++;
				if (result[i] == 117)
				{
					i++;
					if (result[i] == 101)
					{
						tokens.push_back({ 't', 'r', 'u', 'e' });
						types.push_back(ray::json::type::boolean);
						break;
					}
				}
			}
			break;
		default:
			break;
		}
	}

	std::map<const char*, std::any> json;

	for (int i = 0, j = 0; i < tokens.size(); i++)
	{
		switch (types[i])
		{
		case ray::json::key:
			i++;
			if (types[i] == ray::json::colon)
			{
				i++;
				if (types[i] == ray::json::obracket) {
					json.insert(std::pair<const char*, std::map<const char*, std::any>>(tokens[j].c_str(), { {"world", true} }));
					i = tokens.size();
					break;
				}
			}
			return -1;
			break;
		default:
			break;
		}
	}

	return 0;
}
