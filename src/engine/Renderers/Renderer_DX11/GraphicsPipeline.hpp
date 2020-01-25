#pragma once

namespace ray::renderer::directx11
{

	struct InputAssemblerStageDescr
	{

	};

	struct TesselationStageDescr
	{

	};

	struct RasterizationStageDescr
	{

	};

	struct OutputMergerStageDescr
	{

	};

	class GraphicsPipeline
	{
	public:
		GraphicsPipeline();
		~GraphicsPipeline();

		void SetInputAssemblerStage(InputAssemblerStageDescr&);
		void SetTesselationStage(TesselationStageDescr&);
		void SetRasterizationStage(RasterizationStageDescr&);
		void SetOutputMergerStage(OutputMergerStageDescr&);

	};

}

