import bpy
import os
import math

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
        pos = m.location
        rota = str(round(math.degrees(m.rotation_euler.z)))
        f.write('\nCreateSettlement("' + namecleaned(m.name) + '", {' + str(round(pos.x, 2)) + ', ' + str(round(pos.z, 2)) + ', ' + str(round(pos.y, 2)) + '});')

    f.close()
    
    return path

os.startfile(export_scene('scene.txt'))