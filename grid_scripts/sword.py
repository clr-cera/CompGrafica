from model import *

def sword():
    # The sword need to be centralized in the handle, so that it stays in link's hand when scaled and rotated
    obj = Object()
    # blade
    blade = Shape.cylinder(5, 0.2, 30, [0.8, 0.8, 0.8], obj.last_offset).transform(
        translation=np.array([0, 2.5, 0]),
        rotation_deg=np.array([90, 0, 0])
    )
    obj.add_shape(blade)
    # guard
    guard = Shape.cylinder(0.5, 1.5, 30, [0.8, 0.8, 0.8], obj.last_offset).transform(
        translation=np.array([0, 1.5, 0]),
        rotation_deg=np.array([90, 0, 0])
    )
    obj.add_shape(guard)
    # handle
    handle = Shape.cylinder(1.5, 0.3, 30, [0.4, 0.2, 0], obj.last_offset).transform(
        rotation_deg=np.array([90, 0, 0])
    )
    obj.add_shape(handle)
    return obj

if __name__ == "__main__":
    obj = sword()
    obj.save("../objects/sword.obj")