#!/usr/bin/env python2
# -*- coding: utf-8 -*-
"""
Converts OBJ file format to an internal representation.

The format is:
    1. int32 "VC" - vertex count 
    2. float32[3] * "VC" - "vertex coordinates"
    3. float32[2] * "VC" - "texture coordinates"
    
Created on Sat Jan 26 18:28:01 2019

@author: egslava
"""

if "data" not in globals():
#    pass
#else:
#     filename = "Cube.obj"
#     filename = "Plane.obj"
     filename = "Warehouse.obj"
#     filename = "Warehouse-Triangulated.obj"
     with open(filename, "r") as obj_file:    
#         print(help(obj_file))
         data = obj_file.read()
         
hash_count = 0
empty_count = 0
v_count = 0
vt_count = 0
vn_count = 0
n_count = 0
f_count = 0
unsupported_count = 0
unknown_count = 0

v_indexes = set()
vt_indexes = set()
vn_indexes = set()
__indexes = set()

positions = []
tex_coords= []
normals   = []

unidx_positions = []
unidx_tex_coords = []
unidx_normals = []
unidx_faces = []  # [ [ [1/nil/6],  [4//6],  [8//6] ], ...]

# face = [
#   [[pos, tex, normal], [pos, tex, normal], [pos, tex, normal]]
#]

for line in data.split("\n"):
    
     if line.startswith("#"):
         hash_count += 1
     elif line.startswith("vt "):
         u, v = line[3:].split(" ")
         u = float(u)
         v = float(v)
         tex_coords.append([u, v])
         vt_count += 1
     elif line.startswith("vn "):
         x, y, z = line[3:].split(" ")
         x = float(x)
         y = float(y)
         z = float(z)
#         print(x, y, z)
         normals.append([x, y, z])
         vn_count += 1
     elif line.startswith("v "):
         x, y, z = line[2:].split(" ")
         x = float(x)
         y = float(y)
         z = float(z)
         
#         print(x, y, z)
         positions.append([x, y, z])
         v_count += 1
     elif line.startswith("f "):
         f_count += 1
         face = []
         verts = line[2:].split(" ")
         for vert in verts:
             i_v, i_vt, i_vn = vert.split("/")
             
             i_v = int(i_v) - 1
#             assert i_v == 0, "i_v == 0"
             if (i_vt not in (None, "")):
                 i_vt = int(i_vt) - 1
             else:
                 i_vt = None
                 
             if (i_vn not in (None, "")):
                 i_vn = int(i_vn) - 1
             else:
                 i_vn = None
             
             print(i_vt, i_vn)
             assert ((i_vt or i_vn) >= 0)
                 
             if (i_v < 0):
                 raise Exception("Negative indexes are not supported: %d (v)" % i_v)
             if (i_vt is not None and i_vt < 0):
                 raise Exception("Negative indexes are not supported: %d (vt)"% i_vt)
             if (i_vn is not None and i_vn < 0):
                 raise Exception("Negative indexes are not supported: %d (vn)"% i_vn)
             
             v_indexes.add(i_v)
             vt_indexes.add(i_vt)
             vn_indexes.add(i_vn)
             
#             print("i_v", i_v)
#             print("positions: ", positions)
             v = positions[i_v]
             
             vt = tex_coords[i_vt] if i_vt is not None else None
             vn = normals[i_vn] if i_vn is not None else None
             
#             print(i_vt, i_vn, vt, vn)
             assert ((vt or vn) >= 0)
                 
             face.append([v, vt, vn])
         
         assert( len(face) == 3)  # currently, only triangles are supported
         unidx_faces.append(face)
         
         
             
#             __indexes.add(_)
         print(line)
     elif not line.strip():
         empty_count += 1
#         print("Empty line: ", line)
           # materials are hardcoded 
           # smoothing groups
           # object groups
     elif (line.startswith("mtllib ") or 
           line.startswith("usemtl ") or 
           line.startswith("s ") or 
           line.startswith("o ")):
         unsupported_count += 1
     else:
         unknown_count += 1
#         print("Special line: ", line)
     
print("#: %4d | v: %4d | vt: %3d | vn: %3d | f: %4d | e: %4d" % (
      hash_count, v_count, vt_count, vn_count, f_count, empty_count))
         
print("%3d v_indices" % len(v_indexes))
print("%3d vt_indices" % len(vt_indexes))
print("%3d vn_indices" % len(vn_indexes))
print("%3d __indices" % len(__indexes))

print("Unsupported lines found: %3d" % unsupported_count)
print("Unknown lines found: %3d" % unknown_count)
#    print(line)

assert (len(v_indexes) == v_count)
assert (len(vt_indexes.difference({None})) == vt_count)
assert (len(vn_indexes.difference({None})) == vn_count)

VBO_positions = []
VBO_tex_coords = []
VBO_normals = []

for face in unidx_faces:
    for vertex in face:
        pos, tex, normal = vertex
        VBO_positions.append(pos)
        VBO_tex_coords.append(tex)
        VBO_normals.append(normal)

assert (len(VBO_positions) == len(VBO_tex_coords) == len(VBO_normals))

FLAGS = 0
HAS_POS = 1 << 0
HAS_TEX_COORDS = 1 << 1
HAS_NORMALS = 1 << 2


if None not in VBO_positions:
    FLAGS |= HAS_POS
if None not in VBO_tex_coords:
    FLAGS |= HAS_TEX_COORDS
if None not in VBO_normals:
    FLAGS |= HAS_NORMALS
    

    
import struct

def write(filename):
    with open(filename, 'wb') as fout:
        n_triangles = len(VBO_positions) / 3
        fout.write( struct.pack("I",  FLAGS))  # unsigned int - triangle count
        fout.write( struct.pack("I", n_triangles))  # unsigned int - triangle count
        
        if FLAGS & HAS_POS:
            for position in VBO_positions:
                fout.write( struct.pack("fff",  *position))
                
        if FLAGS & HAS_TEX_COORDS:
            for tex_coords in VBO_tex_coords:
                fout.write( struct.pack("ff",  *tex_coords))
                
        if FLAGS & HAS_NORMALS:
            for normal in VBO_normals:
                fout.write( struct.pack("fff",  *normal))
            
#        
#            
#write('/home/egslava/my/arena-shooter/client/res/plane.model')
#write('/home/egslava/my/arena-shooter/client/res/cube.model')
write('/home/egslava/my/arena-shooter/client/res/warehouse.model')

#https://free3d.com/3d-model/abandoned-cottage-house-825251.html

# [✓]. Negative faces are not supported
# [✓]. Check if v == v_indices and vt = vt_indices and vn == vn_indices
# [ ]. Document ctrl+T in blender:
# assert( len(face) == 3)  # currently, only triangles are supported

#Here we go. Now we have unidx_faces and we just need to export it! :)
#         Yai!

