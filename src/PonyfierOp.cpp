#include <PonyfierOp.h>

#include <FnGeolib/op/FnGeolibCookInterface.h>
namespace FnGeolibOp = FNGEOLIBOP_NAMESPACE;

#include <FnAttribute/FnAttribute.h>
#include <FnAttribute/FnGroupBuilder.h>
namespace FnAttribute = FNATTRIBUTE_NAMESPACE;

#include <FnGeolibServices/FnGeolibCookInterfaceUtilsService.h>
namespace FnGeolibServices = FNGEOLIBSERVICES_NAMESPACE;


/** PonyfierOp **/

void PonyfierOp::setup(FnGeolibOp::GeolibSetupInterface &interface)
{
    interface.setThreading(FnGeolibOp::GeolibSetupInterface::ThreadModeConcurrent);
}

void PonyfierOp::cook(FnGeolibOp::GeolibCookInterface &interface)
{
    // Arguments
    FnAttribute::StringAttribute celArg = interface.getOpArg("cel");
    if ( !celArg.isValid() )
    {
        return;
    }

    // Match CEL
    FnGeolibServices::FnGeolibCookInterfaceUtils::MatchesCELInfo info;
    FnGeolibServices::FnGeolibCookInterfaceUtils::matchesCEL(
                        info, interface, celArg);
    if (!info.canMatchChildren)
    {
        interface.stopChildTraversal();
    }

    if (!info.matches)
    {
        return;
    }

    // Get the original bounds and xform
    FnAttribute::DoubleAttribute initialBoundsAttr = interface.getAttr("bound");
    FnAttribute::GroupAttribute initialXformAttr = interface.getAttr("xform");
    FnAttribute::DoubleAttribute initialScaleAttr = interface.getAttr("xform.interactive.scale");
    FnAttribute::DoubleAttribute initialTranslateAttr = interface.getAttr("xform.interactive.translate");

    // Delete all the attributes and load the pony
    interface.deleteAttrs();

    FnAttribute::GroupBuilder alembicArgs;
    alembicArgs.set("fileName", FnAttribute::StringAttribute("./bin/python/UI4/Resources/Geometry/pony/pony.attrs"));
    interface.execOp("ApplyAttrFile", alembicArgs.build());

    // Recover original xform
    if ( initialXformAttr.isValid() )
    {
        interface.setAttr("xform", initialXformAttr);
    }

    // Calulate the pony's dimensions
    FnAttribute::DoubleAttribute ponyBoundsAttr = interface.getOutputAttr("bound");
    if (initialBoundsAttr.isValid() && ponyBoundsAttr.isValid())
    {
        const double* ponyBounds = ponyBoundsAttr.getNearestSample(0).data();
        const double* initialBounds = initialBoundsAttr.getNearestSample(0).data();

        // Scale the pony
        double scale[3] = { (initialBounds[1] - initialBounds[0]) / (ponyBounds[1] - ponyBounds[0]),
                            (initialBounds[3] - initialBounds[2]) / (ponyBounds[3] - ponyBounds[2]),
                            (initialBounds[5] - initialBounds[4]) / (ponyBounds[5] - ponyBounds[4]) };

        if ( initialScaleAttr.isValid() )
        {
            const double* initialScale = initialScaleAttr.getNearestSample(0).data();
            scale[0] *= initialScale[0];
            scale[1] *= initialScale[1];
            scale[2] *= initialScale[2];
        }

        interface.setAttr("xform.interactive.scale", FnAttribute::DoubleAttribute(scale, 3, 3));

        // Translate the pony
        double translate[3] = { (initialBounds[0] + initialBounds[1]) / 2.0,
                                initialBounds[2],
                                (initialBounds[4] + initialBounds[5]) / 2.0 };

        if ( initialTranslateAttr.isValid() )
        {
            const double* initialTranslate = initialTranslateAttr.getNearestSample(0).data();
            translate[0] += initialTranslate[0];
            translate[1] += initialTranslate[1];
            translate[2] += initialTranslate[2];
        }

        interface.setAttr("xform.interactive.translate", FnAttribute::DoubleAttribute(translate, 3, 3));

    }

}

