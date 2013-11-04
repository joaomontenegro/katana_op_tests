#include <TestChildOp.h>

#include <iostream>

#include <FnAttribute/FnAttribute.h>
#include <FnAttribute/FnGroupBuilder.h>
namespace FnAttribute = Foundry::Katana;


void TestChildOp::setup(Foundry::Katana::GeolibSetupInterface &interface)
{
	interface.setThreading(Foundry::Katana::GeolibSetupInterface::ThreadModeConcurrent);
}

void TestChildOp::cook(Foundry::Katana::GeolibCookInterface &interface)
{
	std::cout << interface.getOutputLocationPath() << ": TestChildOp: ";

	FnAttribute::StringAttribute argChild = interface.getOpArg("argChild");

	if ( !argChild.isValid() )
	{
		return;
	}

	std::string value = argChild.getValue();
	interface.setAttr("childAttr", Foundry::Katana::StringAttribute(value));
	interface.setAttr("type", Foundry::Katana::StringAttribute("locator"));

	FnAttribute::StringAttribute worldAttr = interface.getAttr("worldAttr", "/root/world", 0);
	if(worldAttr.isValid())
	{
		interface.setAttr("worldAttr", Foundry::Katana::StringAttribute(worldAttr.getValue("")));
	}

}
