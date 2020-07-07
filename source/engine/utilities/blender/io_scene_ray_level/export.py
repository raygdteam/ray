import bpy

import json

import os

def collapse_json(text, indent=12):
    """Compacts a string of json data by collapsing whitespace after the
    specified indent level

    NOTE: will not produce correct results when indent level is not a multiple
    of the json indent level
    """
    initial = " " * indent
    out = []  # final json output
    sublevel = []  # accumulation list for sublevel entries
    pending = None  # holder for consecutive entries at exact indent level
    for line in text.splitlines():
        if line.startswith(initial):
            if line[indent] == " ":
                # found a line indented further than the indent level, so add
                # it to the sublevel list
                if pending:
                    # the first item in the sublevel will be the pending item
                    # that was the previous line in the json
                    sublevel.append(pending)
                    pending = None
                item = line.strip()
                sublevel.append(item)
                if item.endswith(","):
                    sublevel.append(" ")
            elif sublevel:
                # found a line at the exact indent level *and* we have sublevel
                # items. This means the sublevel items have come to an end
                sublevel.append(line.strip())
                out.append("".join(sublevel))
                sublevel = []
            else:
                # found a line at the exact indent level but no items indented
                # further, so possibly start a new sub-level
                if pending:
                    # if there is already a pending item, it means that
                    # consecutive entries in the json had the exact same
                    # indentation and that last pending item was not the start
                    # of a new sublevel.
                    out.append(pending)
                pending = line.rstrip()
        else:
            if pending:
                # it's possible that an item will be pending but not added to
                # the output yet, so make sure it's not forgotten.
                out.append(pending)
                pending = None
            if sublevel:
                out.append("".join(sublevel))
            out.append(line)
    return "\n".join(out)

class export(bpy.types.Operator):
    bl_idname = "file.ray_level_export"
    bl_label = "ray_level (.ray_level)"
    bl_options = {'REGISTER'}

    filepath = bpy.props.StringProperty(name = "File Path", maxlen = 1024, subtype = "DIR_PATH")

    def execute(self, context):
        bpy.ops.object.select_all(action = 'DESELECT')

        meshes = {}
        lights = {}
        cameras = {}

        for object in context.scene.objects:
            context.view_layer.objects.active = object
            object.select_set(state = True)

            if object.type == "MESH":
                meshes[bpy.context.active_object.name.lower()] = {}
                meshes[bpy.context.active_object.name.lower()]["position"] = [object.location.x, object.location.y, object.location.z]
                meshes[bpy.context.active_object.name.lower()]["rotation"] = [object.rotation_euler.x, object.rotation_euler.y, object.rotation_euler.z]
                meshes[bpy.context.active_object.name.lower()]["scale"] = [object.scale.x, object.scale.y, object.scale.z]
                meshes[bpy.context.active_object.name.lower()]["vertices"] = []
                meshes[bpy.context.active_object.name.lower()]["normals"] = []
                meshes[bpy.context.active_object.name.lower()]["indices"] = []
                vertex_number = -1
                for vertex in object.data.vertices:
                    vertex_number += 1
                    meshes[bpy.context.active_object.name.lower()]["vertices"].append([vertex.co.x, vertex.co.y, vertex.co.z])
                    meshes[bpy.context.active_object.name.lower()]["normals"].append([vertex.normal.x, vertex.normal.y, vertex.normal.z])
                    meshes[bpy.context.active_object.name.lower()]["indices"].append(vertex_number)

            if object.type == "LIGHT":
                lights[bpy.context.active_object.name.lower()] = {}
                lights[bpy.context.active_object.name.lower()]["position"] = [object.location.x, object.location.y, object.location.z]
                lights[bpy.context.active_object.name.lower()]["rotation"] = [object.rotation_euler.x, object.rotation_euler.y, object.rotation_euler.z]
                lights[bpy.context.active_object.name.lower()]["scale"] = [object.scale.x, object.scale.y, object.scale.z]

            if object.type == "CAMERA":
                cameras[bpy.context.active_object.name.lower()] = {}
                cameras[bpy.context.active_object.name.lower()]["position"] = [object.location.x, object.location.y, object.location.z]
                cameras[bpy.context.active_object.name.lower()]["rotation"] = [object.rotation_euler.x, object.rotation_euler.y, object.rotation_euler.z]
                cameras[bpy.context.active_object.name.lower()]["scale"] = [object.scale.x, object.scale.y, object.scale.z]

            object.select_set(state = False)

        with open(os.path.join(self.filepath, "scene.ray_level.meshes"), "w") as file:
            file.write(collapse_json(json.dumps(meshes, indent = 4), indent = 8))

        with open(os.path.join(self.filepath, "scene.ray_level.lights"), "w") as file:
            file.write(collapse_json(json.dumps(lights, indent = 4), indent = 8))

        with open(os.path.join(self.filepath, "scene.ray_level.cameras"), "w") as file:
            file.write(collapse_json(json.dumps(cameras, indent = 4), indent = 8))

        return {'FINISHED'}

    def invoke(self, context, event):
        self.filepath = ""
        context.window_manager.fileselect_add(self)
        return {'RUNNING_MODAL'}