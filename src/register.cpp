#include <FnPluginSystem/FnPlugin.h>

#include <TestChildOp.h>
#include <TestParentOp.h>
#include <PointCloudOp.h>
#include <PonyfierOp.h>

DEFINE_GEOLIBOP_PLUGIN(TestChildOp)
DEFINE_GEOLIBOP_PLUGIN(TestParentOp)
DEFINE_GEOLIBOP_PLUGIN(PointCloudOp)
DEFINE_GEOLIBOP_PLUGIN(PointCloudInstanceOp)
DEFINE_GEOLIBOP_PLUGIN(PonyfierOp)

void registerPlugins()
{
    REGISTER_PLUGIN(TestChildOp, "TestChildOp", 0, 1);
    REGISTER_PLUGIN(TestParentOp, "TestParentOp", 0, 1);
    REGISTER_PLUGIN(PointCloudOp, "PointCloudOp", 0, 1);
    REGISTER_PLUGIN(PointCloudInstanceOp, "PointCloudInstanceOp", 0, 1);
    REGISTER_PLUGIN(PonyfierOp, "PonyfierOp", 0, 1);
}
