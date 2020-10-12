#pragma once

namespace ui
{
	template <typename type>
	class object
	{
	public:
		object();
		~object();

		virtual type process() = 0;

	protected:
		const FVector<2>& position;
		const FVector<2>& size;
	};
}