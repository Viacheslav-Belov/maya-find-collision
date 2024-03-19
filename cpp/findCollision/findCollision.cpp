#include <stdio.h>

#include <maya/MString.h>
#include <maya/MArgList.h>

#include <maya/MObject.h>
#include <maya/MFnTransform.h>
#include <maya/MFnPlugin.h>
#include <maya/MGlobal.h>
#include <maya/MItSelectionList.h>
#include <maya/MPxCommand.h>
#include <maya/MFnMesh.h>
#include <maya/MDagPath.h>
#include <maya/MVector.h>
#include <maya/MSelectionList.h>
#include <maya/MMatrix.h>
#include <maya/MFloatPoint.h>
#include <maya/MFloatPointArray.h>

#include <maya/MDagModifier.h>

#include <maya/miostream.h>


class findCollision : public MPxCommand
{
    public:
        MStatus doIt( const MArgList& args );
        static void* creator();
};

void* findCollision::creator() {
    return new findCollision;
}

MStatus initializePlugin( MObject obj ) {
    MFnPlugin plugin( obj, "ViacheslavBelov", "1.0", "Any" );
    plugin.registerCommand( "FindCollision", findCollision::creator );
    return MS::kSuccess;
}

MStatus uninitializePlugin( MObject obj ) {
    MFnPlugin plugin( obj );
    plugin.deregisterCommand( "FindCollision" );
    return MS::kSuccess;
}


MFloatPoint find_closest_point(MFloatPointArray points, MFloatPoint origin_point) {
    MFloatPoint c_point = points[0];
    float min_length = 0;

    int points_length = points.length();
    for (int i = 1; i < points_length; i++)
        if (points[i].distanceTo(origin_point) < min_length)
            c_point = points[i];

    return c_point;
};


MObject create_locator(float position_x, float position_y, float position_z) {

    MStatus status = MS::kSuccess;
    MObject locatorObj = MObject::kNullObj;
    MObject parent = MObject::kNullObj;
    
    MString name("intersection_point");
    MDagModifier modifier;
    locatorObj = modifier.createNode("locator", parent, &status);
    status = modifier.doIt();
    if (status != MS::kSuccess)
    {
        MString theError("Failed to create locator node ");
        theError += name;
        cout << theError << endl;
        return locatorObj;
    }

    MFnDagNode fnLocator(locatorObj);
    fnLocator.setName(name);
    MFnTransform transform(locatorObj);
    transform.setTranslation(MVector(position_x, position_y, position_z), MSpace::kTransform);

    return locatorObj;
};


MStatus findCollision::doIt( const MArgList& args ) {
    MStatus status = MS::kSuccess;
    
    MSelectionList selection_list;
    MGlobal::getActiveSelectionList(selection_list);
    
    MDagPath mesh_dag;
    selection_list.getDagPath(0, mesh_dag);
    MDagPath locator_dag;
    selection_list.getDagPath(1, locator_dag);
    MFnMesh mesh(mesh_dag, &status);

    MMatrix locator_matrix;
    locator_matrix = locator_dag.inclusiveMatrix();
    MTransformationMatrix m_matrix(locator_matrix);

    MVector ray_source;
    ray_source = m_matrix.translation(MSpace::kWorld);
    MFloatPoint ray_source_float_point(ray_source);
    
    MFloatPoint hit_point;
    MMeshIsectAccelParams accel_params = mesh.autoUniformGridParams();
    float hit_ray_param, hit_bary_1, hit_bary_2;
    int hit_face, hit_triangle;

    MFloatPoint closest_point;
    MFloatPointArray hit_points;
    hit_points.setLength(3);

    float ray_directions[3][3] = {
        {1.0, 0.0, 0.0},
        {0.0, 1.0, 0.0},
        {0.0, 0.0, 1.0}
    };
    bool intersects;
    for (int i=0; i < 3; i++) {
        intersects = mesh.closestIntersection(
            ray_source,
            ray_directions[i],
            NULL,
            NULL,
            false,
            MSpace::kWorld,
            9999,
            true,
            &accel_params,
            hit_point,
            &hit_ray_param,
            &hit_face,
            &hit_triangle,
            &hit_bary_1,
            &hit_bary_2,
            (float)1e-6,
            &status
        );
        hit_points[i] = hit_point;
    };

    if (intersects) {
        closest_point = find_closest_point(hit_points, ray_source);
        cout << closest_point << endl;
        create_locator(closest_point.x, closest_point.y, closest_point.z);
    } else {
        cout << "No collisions found" << endl;
    };
    
    return status;

};
