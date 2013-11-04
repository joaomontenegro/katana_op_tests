#include <PointCloudOp.h>

#include <iostream>
#include <sstream>

#include <FnAttribute/FnAttribute.h>
#include <FnAttribute/FnGroupBuilder.h>
namespace FnAttribute = Foundry::Katana;


/** PointCloudOp **/

void PointCloudOp::setup(Foundry::Katana::GeolibSetupInterface &interface)
{
	interface.setThreading(Foundry::Katana::GeolibSetupInterface::ThreadModeConcurrent);
}

void PointCloudOp::cook(Foundry::Katana::GeolibCookInterface &interface)
{
	// Arguments
	FnAttribute::StringAttribute sourceArg = interface.getOpArg("source");
	FnAttribute::StringAttribute destArg = interface.getOpArg("dest");
	FnAttribute::StringAttribute instanceArg = interface.getOpArg("instance");
	FnAttribute::DoubleAttribute instanceScaleArg = interface.getOpArg("instanceScale");

	if ( !sourceArg.isValid() || !destArg.isValid() )
	{
		return;
	}

	double instanceScale = 0.01;
	if ( instanceScaleArg.isValid() )
	{
		instanceScale = instanceScaleArg.getValue();
	}

	// Check if we are in the destination location
	if ( interface.getOutputLocationPath() == destArg.getValue() )
	{
		// Get the source point cloud
		FnAttribute::FloatAttribute sourcePAttr = interface.getAttr("geometry.point.P", sourceArg.getValue());
		if ( !sourcePAttr.isValid() )
		{
			interface.setAttr("error", FnAttribute::StringAttribute("point cloud not found in source"));
			interface.setAttr("type", FnAttribute::StringAttribute("error"));
			return;
		}

		FnAttribute::FloatConstVector sourceP = sourcePAttr.getNearestSample(0);
		size_t nSourceP = sourceP.size();
		double p[3];
		double s[3] = {instanceScale, instanceScale, instanceScale};

		for (size_t i = 0; i + 2 < nSourceP; i += 3 )
		{
			// Transform for child arguments
			p[0] = (double)sourceP[i + 0];
			p[1] = (double)sourceP[i + 1];
			p[2] = (double)sourceP[i + 2];

			FnAttribute::DoubleAttribute translate(p, 3, 3);
			FnAttribute::DoubleAttribute scale(s, 3, 3);

			FnAttribute::GroupBuilder interactive;
			interactive.set("translate", translate);
			interactive.set("scale", scale);

			FnAttribute::GroupBuilder xform;
			xform.set("interactive", interactive.build());

			// Arguments for PointCloudInstanceOp
			FnAttribute::GroupBuilder childArgs;
			childArgs.set("xform", xform.build());
			childArgs.set("instance", instanceArg);

			// Child name
			std::stringstream childName;
			childName << "child_" << i;

			// Create child
			interface.createChild( childName.str(), "PointCloudInstanceOp", childArgs.build() );
		}

		// Set this location's type
		interface.setAttr("type", FnAttribute::StringAttribute("point cloud root"));
	}
}


/** PointCloudInstanceOp **/

void PointCloudInstanceOp::setup(Foundry::Katana::GeolibSetupInterface &interface)
{
	interface.setThreading(Foundry::Katana::GeolibSetupInterface::ThreadModeConcurrent);
}

void PointCloudInstanceOp::cook(Foundry::Katana::GeolibCookInterface &interface)
{
	// Arguments
	FnAttribute::GroupAttribute xformArg = interface.getOpArg("xform");
	FnAttribute::StringAttribute instanceArg = interface.getOpArg("instance");

	if ( !xformArg.isValid() || !instanceArg.isValid())
	{
		interface.setAttr("error",  FnAttribute::StringAttribute("xform or instance not specified"));
		interface.setAttr("type", FnAttribute::StringAttribute("error"));
		return;
	}

	// Get Instance's geometry
	FnAttribute::GroupAttribute geometryAttr = interface.getAttr("geometry", instanceArg.getValue());
	if ( !geometryAttr.isValid())
	{
		interface.setAttr("error",  FnAttribute::StringAttribute("source geometry not specified"));
		interface.setAttr("type", FnAttribute::StringAttribute("error"));
		return;
	}

	// Get Instance's type
	FnAttribute::StringAttribute typeAttr = interface.getAttr("type", instanceArg.getValue());
	if ( !typeAttr.isValid())
	{
		interface.setAttr("error",  FnAttribute::StringAttribute("source type not specified"));
		interface.setAttr("type", FnAttribute::StringAttribute("error"));
		return;
	}

	// Set xform from arg, geometry and type from instance
	interface.setAttr("xform", xformArg);
	interface.setAttr("geometry", geometryAttr);
	interface.setAttr("type", typeAttr);
}

