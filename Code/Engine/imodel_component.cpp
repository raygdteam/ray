#include "pch.h"
#include "imodel_component.hpp"
#include "Level.hpp"


IModelComponent::IModelComponent()
	:
	 m_Path(TEXT(""))
	,m_bGammaCorrection(false)
{
}


IModelComponent::~IModelComponent() {};

IModelComponent* CreateModelComponent()
{
	return nullptr;
}
