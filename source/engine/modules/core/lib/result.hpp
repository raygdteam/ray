#pragma once

template<typename Type, typename ErrorType>
struct Result
{
	Type Data;
	ErrorType Error;

	[[nodiscard]]
	bool IsSuccess() const noexcept
	{ return (int)Error == 0; }
};
