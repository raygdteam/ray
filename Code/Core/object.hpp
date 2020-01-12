#pragma once
#include <ray/type/extensions.hpp>
#include <spdlog/spdlog.hpp>

#define GENERATED_BODY protected: \
				std::shared_ptr<spdlog::logger> log() override { static auto _log = spdlog::create<spdlog::sinks::wincolor_stdout_sink_mt>(__class_name()); return _log; } \
				public: pcstr __class_name() override \
				{ return typeid(*this).name() + 6; } \
				pcstr __class_filename() { return __FILE__; }

namespace ray
{
	class object
	{
	protected:
		virtual std::shared_ptr<spdlog::logger> log() = 0;
	public:
		virtual constexpr pcstr __class_name() = 0;
		virtual constexpr pcstr __class_filename() = 0;

	};
}