#include <TestParentOp.h>

#include <iostream>

#include <FnAttribute/FnAttribute.h>
#include <FnAttribute/FnGroupBuilder.h>
namespace FnAttribute = Foundry::Katana;


void TestParentOp::setup(Foundry::Katana::GeolibSetupInterface &interface)
{
	interface.setThreading(Foundry::Katana::GeolibSetupInterface::ThreadModeConcurrent);
}

void TestParentOp::cook(Foundry::Katana::GeolibCookInterface &interface)
{
	std::cout << interface.getOutputLocationPath() << ": TestParentOp: " << std::endl;

	// Create Attribute
	FnAttribute::StringAttribute argParent = interface.getOpArg("argParent");
	FnAttribute::StringAttribute argChild = interface.getOpArg("argChild");

	if ( !argParent.isValid() || !argChild.isValid() )
	{
		return;
	}

	std::string argParentValue = argParent.getValue();
	std::string argChildValue = argChild.getValue();

	FnAttribute::GroupBuilder childArgs;
	childArgs.set("argChild", Foundry::Katana::StringAttribute(argChildValue));

	interface.setAttr("parentAttr", Foundry::Katana::StringAttribute(argParentValue));

	if ( interface.getOutputLocationPath() == "/root/world/geo/a/b" )
	{
		interface.createChild("child", "TestChildOp", childArgs.build());
		interface.stopChildTraversal();
	}
	else
	{
		std::string location = interface.getOutputName();
		std::string childName = "null";

		if ( location == "root" )
		{
			childName = "world";
		}
		else if ( location == "world" )
		{
			childName = "geo";
		}
		else if ( location == "geo" )
		{
			childName = "a";
		}
		else if ( location == "a" )
		{
			childName = "b";
		}

		childArgs.set( "argParent", Foundry::Katana::StringAttribute( argParentValue ) );
		interface.createChild( childName, "TestParentOp", childArgs.build() );
	}

	if ( interface.getOutputName() == "a" )
	{
		FnAttribute::GroupBuilder execArgs;
		execArgs.set("argChild", Foundry::Katana::StringAttribute("exec"));
		interface.execOp("TestChildOp", execArgs.build());
	}
}
