from model import *

def grass():
    obj = Object()
    obj.add_shape(Shape.square(1.0, [45/255, 135/255, 69/255], obj.last_offset))
    obj.save("../objects/floor.obj")
    obj = Object()
    obj.add_shape(Shape.cone(0.1, 0.05, 30, [12/255, 46/255, 21/255], obj.last_offset))
    obj.save("../objects/grass.obj")

def link():
    obj = Object()
    # body
    body = Shape.cylinder(1.5, 0.5, 30, [0.25, 0.46, 0.28], obj.last_offset).transform(
        rotation_deg=np.array([90, 0, 0])
    )
    obj.add_shape(body)
    # head
    head = Shape.sphere(0.5, 50, [0.94, 0.59, 0], obj.last_offset).transform(
        translation=np.array([0, 1.25, 0]),
        # scale=np.array([1.5, 1.5, 1.5])
    )
    obj.add_shape(head)
    hat = Shape.cone(4, 0.8, 30, [0.25, 0.46, 0.28], obj.last_offset).transform(
        translation=np.array([0, 1.5, 0]),
        rotation_deg=np.array([315, 0, 0]),
        scale=np.array([0.8, 0.8, 0.8])   
    )
    obj.add_shape(hat)
    # left arm
    left_arm = Shape.cylinder(1.2, 0.25, 30, [0.94, 0.59, 0], obj.last_offset).transform(
        translation=np.array([-0.8, 0.3, 0]),
        rotation_deg=np.array([0, 90, 45])
    )
    obj.add_shape(left_arm)
    # right arm
    right_arm = Shape.cylinder(1.2, 0.25, 30, [0.94, 0.59, 0], obj.last_offset).transform(
        translation=np.array([0.8, 0.3, 0]),
        rotation_deg=np.array([0, 90, -45])
    )
    obj.add_shape(right_arm)
    # left leg
    left_leg = Shape.cylinder(1.5, 0.3, 30, [0.94, 0.59, 0], obj.last_offset).transform(
        translation=np.array([-0.2, -1.5, 0.0]),
        rotation_deg=np.array([0, 90, 90])
    )
    obj.add_shape(left_leg)
    # right leg
    right_leg = Shape.cylinder(1.5, 0.3, 30, [0.94, 0.59, 0], obj.last_offset).transform(
        translation=np.array([0.2, -1.5, 0.0]),
        rotation_deg=np.array([0, 90, 90])
    )
    obj.add_shape(right_leg)
    return obj


if __name__ == "__main__":
    grass()
