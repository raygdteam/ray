#pragma once
#include "ishader.hpp"

class IModelComponent
{
public:
	IModelComponent();
	virtual ~IModelComponent() = 0;

	virtual void Load(const ray_string&, bool) = 0;
	virtual void Draw(IShader&) = 0;

	ray_string& GetPath() { return m_Path; }
	bool GetGammaCorrection() { return m_bGammaCorrection; }
	void SetPath(ray_string path) { this->m_Path = path; }
	void SetGammaCorrection(bool GammaCorrection) { this->m_bGammaCorrection = GammaCorrection; }
	
private:
	ray_string m_Path;
	bool m_bGammaCorrection;

};

IModelComponent* CreateModelComponent();
