import bpy
import os
from pathlib import Path

def getscenedir():
    return os.path.dirname(bpy.data.filepath)

def export_scene(path):
    # path fixing (many other validations to be added here)
    path = os.path.abspath(path)

    exportname = Path(bpy.data.filepath).stem

    # create directory
    outpath = os.path.join(path)

    try:
        os.mkdir(outpath)
    except FileExistsError:
        # directory already exists
        pass
            
    bpy.ops.export_scene.gltf(
        filepath=os.path.join(outpath, exportname +'.glb'), 
        check_existing=False,
        export_keep_originals=False, 
        export_texcoords=True, 
        export_normals=True, 
        export_tangents=False, 
        export_materials='EXPORT', # 'NONE' 
        export_colors=False, 
        export_attributes=False, 
        use_mesh_edges=False, 
        use_mesh_vertices=False, 
        export_cameras=False, 
        use_selection=False, 
        use_visible=False, 
        use_renderable=False, 
        use_active_collection_with_nested=True, 
        use_active_collection=False, 
        use_active_scene=False, 
        export_extras=False, 
        export_yup=True, 
        export_apply=True,
        export_animations=True, 
        export_frame_range=False, 
        export_frame_step=1, 
        export_force_sampling=True, 
        export_animation_mode='ACTIONS', 
        export_nla_strips_merged_animation_name='Animation', 
        export_def_bones=True, 
        export_hierarchy_flatten_bones=False,
        export_optimize_animation_size=False, 
        export_optimize_animation_keep_anim_armature=True, 
        export_optimize_animation_keep_anim_object=False, 
        export_negative_frame='SLIDE', 
        export_anim_slide_to_zero=True, # important for Bones Positiond 
        export_bake_animation=False, 
        export_anim_single_armature=True, 
        export_reset_pose_bones=True, 
        export_current_frame=False, 
        export_rest_position_armature=True, 
        export_anim_scene_split_object=True, 
        export_skins=True, 
        export_influence_nb=4, 
        export_all_influences=False, 
        export_morph=True, 
        export_morph_normal=True, 
        export_morph_tangent=False, 
        export_morph_animation=True, 
        export_morph_reset_sk_data=True, 
        export_lights=False, 
        export_try_sparse_sk=True, 
        export_try_omit_sparse_sk=False, 
        export_gpu_instances=False, 
        export_nla_strips=True, 
        export_original_specular=False
    )

    return outpath

path = os.path.abspath(os.path.join(getscenedir(), "../..", "resources/models"))
os.startfile(export_scene(os.path.realpath(path)))