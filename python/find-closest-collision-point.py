from __future__ import annotations

import maya.api.OpenMaya as om
import pymel.core as pm
from typing import Optional


def find_closest_collision_point() -> om.MFloatPoint | None:
    """
    1. Select mesh.
    2. Select locator to check for collisions with mesh.
    3. Execute this function.
    
    Returns:
        (X, Y, Z) coordinates of the collision point, if found.
        None otherwise.
    """
    selection_list = om.MGlobal.getActiveSelectionList()

    mesh = om.MFnMesh(selection_list.getDagPath(0))
    locator_dag = selection_list.getDagPath(1)
    
    m_matrix = om.MTransformationMatrix(locator_dag.inclusiveMatrix())
    ray_source = m_matrix.translation(om.MSpace.kWorld)
    ray_source_float_point = om.MFloatPoint(*ray_source)

    ray_directions = (
        (1.0, 0.0, 0.0),
        (0.0, 1.0, 0.0),
        (0.0, 0.0, 1.0),
    )
    intersections = []
    for ray_direction in ray_directions:
        intersection = mesh.closestIntersection(
            ray_source_float_point,  # raySource
            om.MFloatVector(*ray_direction),  # rayDirection
            om.MSpace.kWorld,  # space
            10000,  # maxParam
            True,  # testBothDirections
            idsSorted=False,
            accelParams=mesh.autoUniformGridParams(),
            tolerance=0.0001,
        )
        if intersection is not None:
            hit_point, hit_ray_param, hit_face, hit_triangle, hit_bary_1, hit_bary_2 = intersection
            intersections.append(hit_point)

    if intersections:
        return min(intersections, key=lambda hp: hp.distanceTo(ray_source_float_point))


def create_locator(m_float_point: om.MFloatPoint) -> None:
    """
    Creates a new locator at the "m_float_point" coordinates.
    """
    loc = pm.spaceLocator()
    loc.setTranslation(
        (m_float_point.x, m_float_point.y, m_float_point.z),
        ws=True,
    )


closest_collision_point = find_closest_collision_point()
if closest_collision_point:
    create_locator(closest_collision_point)
