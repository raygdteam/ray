import json
import bpy
import os

bl_info = {
    "name": "Ray Scene",
    "blender": (2, 82, 7), # change me?
    "category": "Export",
    "location": "Info > File > Export > Ray Scene Export",
}

def register():
    bpy.utils.register_class(RayExport)
    bpy.types.TOPBAR_MT_file_export.append(menu_function)

def unregister():
    bpy.utils.unregister_class(RayExport)
    bpy.types.TOPBAR_MT_file_export.remove(menu_function)

def menu_function(self, context):
    self.layout.operator(RayExport.bl_idname)

class RayExport(bpy.types.Operator):
    bl_idname = "file.ray_scene_export"
    bl_label = "Ray Scene Export"
    bl_options = {'REGISTER'}

    filepath = bpy.props.StringProperty(name = "File Path", maxlen = 1024, subtype = "DIR_PATH")

    def execute(self, context):
        bpy.ops.object.select_all(action = 'DESELECT')

        scene = {"scene": {}}

        for object in context.scene.objects:
            context.view_layer.objects.active = object
            object.select_set(state=True)

            if object.type == 'MESH':
                scene["scene"][bpy.context.active_object.name] = {}
                scene["scene"][bpy.context.active_object.name]["vertices"] = []
                for vertex in object.data.vertices:
                    scene["scene"][bpy.context.active_object.name]["vertices"].append([vertex.co.x, vertex.co.y, vertex.co.z])

            object.select_set(state = False)

        with open(os.path.join(self.filepath, "scene.json"), "w") as write_file:
            json.dump(scene, write_file)

        return {'FINISHED'}

    def invoke(self, context, event):
        self.filepath = ""
        context.window_manager.fileselect_add(self)
        return {'RUNNING_MODAL'}

if __name__ == "__main__":
    register()