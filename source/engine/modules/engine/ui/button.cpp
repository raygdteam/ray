#include <engine/ui/button.hpp>

void UiButton::Tick()
{
	
}

UiButton::UiButton()
{
	_transform = new Transform;
	_renderData = new UiRenderData;

	_renderData->Type = 0b011;

	_renderData->State = 0;

	{
		UiRenderDataState& state = _renderData->StateDescriptions[0];

		state.NumVertices = 4;

		/*
			-1	-1
			 1	-1
			 1	 1
			-1	 1
		*/
	}
}
