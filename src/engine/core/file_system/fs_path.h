#ifndef FS_PATH_H_INCLUDED
#define FS_PATH_H_INCLUDED

//#include <xray/fs_path_string.h>

// TODO: for now
#include <ray/type/extensions.hpp>
using path_string = string;

namespace xray	{
namespace fs	{

RAY_CORE_API path_string		convert_to_native	(pcstr path);
RAY_CORE_API path_string		convert_to_portable (pcstr path);
RAY_CORE_API path_string		convert_to_portable (pcstr path);
RAY_CORE_API char* convert_to_portable_in_place	(char* const path);

} // namespace fs
} // namespace xray

#endif // FS_PATH_H_INCLUDED