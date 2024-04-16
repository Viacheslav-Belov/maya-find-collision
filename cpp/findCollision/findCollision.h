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

MFloatPoint find_closest_point(MFloatPointArray points, MFloatPoint origin_point);
MObject create_locator(float position_x, float position_y, float position_z);
MStatus initializePlugin( MObject obj );
MStatus uninitializePlugin( MObject obj );
