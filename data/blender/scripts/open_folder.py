import bpy
import os

def getscenedir():
    return os.path.dirname(bpy.data.filepath)

path = os.path.abspath(os.path.join(getscenedir(), "../..", "resources/models"))
os.startfile(path)