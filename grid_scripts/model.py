from abc import abstractmethod

import numpy as np
from math import *



    
class Shape:
    def __init__(self, vertices : np.array, colors : np.array, faces : np.array, offset : int):
        self.offset : int = offset
        # [n, 3] array
        self.vertices : np.array  = vertices
        self.colors : np.array  = colors
        self.faces : np.array = faces + offset
    def __str__(self):
        return f"""
        Shape:  
            offset: {self.offset}
            vertices: {self.vertices}
            colors: {self.colors}
            faces: {self.faces} 
        """
    def vertex_string(self) -> str:
        vertices_str = ""
        for vertex, color in zip(self.vertices, self.colors):
            vertices_str += f"v {vertex[0]} {vertex[1]} {vertex[2]} {color[0]} {color[1]} {color[2]}\n"
        return vertices_str
    def face_string(self) -> str:
        faces_str = ""
        for face in self.faces:
            faces_str += f"f {face[0]} {face[1]} {face[2]}\n"
        return faces_str

    def circle(radius, num_segments, empty_dim, color, offset):
        t = np.linspace(0, 2 * pi, num_segments, endpoint=False)
        dim1 = radius * np.cos(t)
        dim2 = radius * np.sin(t)
        vertices = np.zeros((num_segments, 3))
        match empty_dim:
            case "x":
                vertices[:, 1] = dim1
                vertices[:, 2] = dim2
            case "y":
                vertices[:, 0] = dim1
                vertices[:, 2] = dim2
            case "z":
                vertices[:, 0] = dim1
                vertices[:, 1] = dim2  
        colors = np.tile(color, (num_segments, 1))
        faces = []
        for i in range(1, num_segments-1):
            faces.append([0, i, i + 1])
        return Shape(vertices, colors, np.array(faces), offset)

    def cylinder(height, radius, num_segments, color, offset):
        top_circle = Shape.circle(radius, num_segments, "z", color, offset)
        bottom_circle = Shape.circle(radius, num_segments, "z", color, offset + num_segments)
        bottom_circle.vertices[:, 2] -= height / 2
        top_circle.vertices[:, 2] += height / 2
        def helper(i):
            if i < 2* num_segments:
                return i
            return i - num_segments
        faces = []
        for i in range(num_segments):
            faces.append([i, (i + 1) % num_segments, i + num_segments])
            faces.append([i+num_segments, helper(i+1+num_segments), (i+1)%num_segments])
        return Shape(np.vstack((top_circle.vertices, bottom_circle.vertices)), np.vstack((top_circle.colors, bottom_circle.colors)), np.array(faces), offset)

    def sphere(radius, num_segments, color, offset):
        vertices = []
        colors = []
        faces = []
        for i in range(num_segments):
            lat = pi * (i / num_segments - 0.5)
            for j in range(num_segments):
                lon = 2 * pi * (j / num_segments)
                x = radius * cos(lat) * cos(lon)
                y = radius * cos(lat) * sin(lon)
                z = radius * sin(lat)
                vertices.append([x, y, z])
                colors.append(color)
        vertices = np.array(vertices)
        colors = np.array(colors)
        for i in range(num_segments):
            for j in range(num_segments):
                next_i = (i + 1) % num_segments
                next_j = (j + 1) % num_segments
                faces.append([i * num_segments + j, next_i * num_segments + j, i * num_segments + next_j])
                faces.append([next_i * num_segments + j, next_i * num_segments + next_j, i * num_segments + next_j])
        return Shape(vertices, colors, np.array(faces), offset)

    def cone(height, radius, num_segments, color, offset):
        base_circle = Shape.circle(radius, num_segments, "z", color, offset)
        tip_vertex = np.array([[0, 0, height / 2]])
        vertices = np.vstack((base_circle.vertices, tip_vertex))
        colors = np.vstack((base_circle.colors, color))
        faces = []
        for i in range(num_segments):
            faces.append([i, (i + 1) % num_segments, num_segments])  # Connect base to tip
        return Shape(vertices, colors, np.array(faces), offset)

    def square(side, color, offset):
        s = side / 2
        vertices = np.array([
            [s, 0, s],
            [s, 0, -s],
            [-s, 0, -s],
            [-s, 0, s]
        ])
        faces = np.array([
            [0, 1, 2],
            [2, 3, 0]
        ])
        colors = np.tile(color, (4, 1))
        return Shape(vertices, colors, faces, offset)

    def transform(self, translation: np.array = np.array([0, 0, 0]), scale: np.array = np.array([1, 1, 1]), rotation_deg: np.array = np.array([0, 0, 0])) -> 'Shape':
        """
        Applies transformations to all vertices in the correct order: Scale -> Rotate -> Translate.
        
        Args:
            translation: [x, y, z] translation vector
            scale: [sx, sy, sz] scale factors for each axis
            rotation_deg: [rx, ry, rz] rotation angles in degrees around each axis
        """
        # Convert degrees to radians
        rx, ry, rz = np.radians(rotation_deg)
        
        # Rotation matrices for each axis
        rot_x = np.array([
            [1, 0, 0],
            [0, cos(rx), -sin(rx)],
            [0, sin(rx), cos(rx)]
        ])
        
        rot_y = np.array([
            [cos(ry), 0, sin(ry)],
            [0, 1, 0],
            [-sin(ry), 0, cos(ry)]
        ])
        
        rot_z = np.array([
            [cos(rz), -sin(rz), 0],
            [sin(rz), cos(rz), 0],
            [0, 0, 1]
        ])
        
        # Combined rotation matrix: Rz * Ry * Rx (applied right-to-left)
        rotation_matrix = rot_z @ rot_y @ rot_x
        
        # Scale matrix
        scale_matrix = np.diag(scale)
        
        # Combined transform: Rotate * Scale (translation added after)
        transform_matrix = rotation_matrix @ scale_matrix
        
        # Apply to all vertices: Scale -> Rotate -> Translate
        self.vertices = (self.vertices @ transform_matrix.T) + translation
        
        return self

class Object:
    def __init__(self):
        self.shapes : list[Shape] = []
        self.last_offset = 1
    def to_wavefront_obj(self) -> str:
        vertices_str = ""
        faces_str = ""
        for shape in self.shapes:
            vertices_str += shape.vertex_string()
            faces_str += shape.face_string()
        return vertices_str + faces_str
    def save(self, filename : str):
        with open(filename, "w") as f:
            f.write(self.to_wavefront_obj())
    def add_shape(self, shape : Shape):
        self.shapes.append(shape)
        self.last_offset += len(shape.vertices)




if __name__ == "__main__":
    # Create a cylinder
    obj = Object()
    s = Shape.cone(1, 0.5, 20, [255, 0, 0], obj.last_offset).transform(
        rotation_deg=np.array([0, 90, 0])
    )
    obj.add_shape(s)
    
    # Apply transformations:
    # - Scale by 2x in all dimensions
    # - Rotate 45° around Z axis
    # - Translate by (1, 2, 3)
    

    obj.save("/home/felipe/Documents/Usp/2025_2/cad/CompGrafica/objects/sample.obj")