#pragma once

const char* format(const char* format);

template<typename type>
const char* format(const char* format, type argument);

template<typename type, typename ... Arguments>
const char* format(const char* format, type argument, Arguments ... arguments);