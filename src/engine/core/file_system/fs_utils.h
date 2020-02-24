#ifndef XRAY_FS_UTILS_H_INCLUDED
#define XRAY_FS_UTILS_H_INCLUDED

namespace xray {
namespace fs {

enum			open_file_enum : u32 {	open_file_create		=	1 << 0, 
									open_file_truncate		=	1 << 1,
									open_file_read			=	1 << 2, 
									open_file_write			=	1 << 3, 
									open_file_append		=	1 << 4,		};

RAY_CORE_API bool		open_file					(FILE** out_dest, 
																 open_file_enum open_flags, 
																 pcstr file_name,
																 bool assert_on_fail = true);

RAY_CORE_API void					verify_path_is_portable 	(pcstr path_str);
RAY_CORE_API bool calculate_file_size(u64* const out_file_size, pcstr const file_path);
RAY_CORE_API bool calculate_file_size(u64* const out_file_size, FILE* const file);
RAY_CORE_API bool					make_dir					(pcstr path);
RAY_CORE_API bool			  		make_dir_r					(pcstr dir_path, bool create_last = true); // recursively creates directories
RAY_CORE_API void					directory_part_from_path	(string* out_result, pcstr path);
RAY_CORE_API pcstr					file_name_from_path			(pcstr path);
RAY_CORE_API void					file_name_with_no_extension_from_path	(string* out_result, pcstr path);
RAY_CORE_API pcstr					extension_from_path			(pcstr path);

struct path_info
{
	enum type_enum					{ type_nothing, type_file, type_folder };
	type_enum						type;
	u32								file_size;
	u32								file_last_modify_time;

	path_info						() : type(type_nothing), file_size(0) {}
};

RAY_CORE_API path_info::type_enum	get_path_info				(path_info * out_path_info, pcstr path);

} // namespace fs
} // namespace xray

#endif // #ifndef XRAY_FS_UTILS_H_INCLUDED