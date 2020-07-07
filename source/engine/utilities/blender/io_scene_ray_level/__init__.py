bl_info = {
    "name": "ray_level format",
    "description": "Import-Export ray_level files",
    "blender": (2, 80, 0),
    "location": "File > Import-Export",
    "category": "Import-Export"
}

import bpy

from .export import export

def register():
    bpy.utils.register_class(export)
    bpy.types.TOPBAR_MT_file_export.append(topbar_mt_export_function)

def unregister():
    bpy.utils.unregister_class(export)
    bpy.types.TOPBAR_MT_file_export.remove(topbar_mt_export_function)

def topbar_mt_export_function(self, context):
    self.layout.operator(export.bl_idname)

if __name__ == '__main__':
    register()