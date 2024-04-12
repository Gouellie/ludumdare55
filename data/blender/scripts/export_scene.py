import bpy
import os

def namecleaned(name):
    return name.split('.', 1)[0]

def getscenedir():
    return os.path.dirname(bpy.data.filepath)

def export_scene(path):
    path = os.path.join(getscenedir(), path)

    f = open(path, 'w')

    bpy.ops.object.select_by_type(extend=False, type='MESH')
    meshes = bpy.context.selected_objects

    for m in meshes:
        f.write(namecleaned(m.name))
        f.write('\n ' + str(m.location))
        f.write('\n ' + str(m.rotation_euler))
        f.write('\n')        
    f.close()
    
    return path

os.startfile(export_scene('scene.txt'))