#include <stdio.h>

#include <maya/MString.h>
#include <maya/MArgList.h>

#include <maya/MFnPlugin.h>
#include <maya/MGlobal.h>
#include <maya/MItSelectionList.h>
#include <maya/MPxCommand.h>
#include <maya/MFnMesh.h>
#include <maya/MDagPath.h>
#include <maya/MVector.h>
#include <maya/MSelectionList.h>
#include <maya/MMatrix.h>

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
    MFnPlugin plugin( obj, "Viacheslav", "1.0", "Any" );
    plugin.registerCommand( "FindCollision", findCollision::creator );
    return MS::kSuccess;
}

MStatus uninitializePlugin( MObject obj ) {
    MFnPlugin plugin( obj );
    plugin.deregisterCommand( "FindCollision" );
    return MS::kSuccess;
}

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

    float ray_directions[3][3] = {
        {1.0, 0.0, 0.0},
        {0.0, 1.0, 0.0},
        {1.0, 0.0, 1.0}
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
            false,
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
        if (intersects) {
            cout << hit_point << endl;
            // TODO: return the closest intersection
            // from all 3 directions instead of the first.
            return status;
        };
    };

    return status;
}
