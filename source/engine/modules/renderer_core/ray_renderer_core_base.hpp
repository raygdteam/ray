#pragma once

namespace ray::renderer_core_api
{
class IRRCBase
{
public:
	void SetInstance (void* instance) { _instance = instance; }
	void* GetInstance() { return _instance; }

protected:
	void* _instance;

};
}