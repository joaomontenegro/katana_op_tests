#include <FnGeolib/op/FnGeolibOp.h>


class PointCloudOp : public Foundry::Katana::GeolibOp
{
public:
    static void setup(Foundry::Katana::GeolibSetupInterface &interface);
    static void cook(Foundry::Katana::GeolibCookInterface &interface);
};


class PointCloudInstanceOp : public Foundry::Katana::GeolibOp
{
public:
    static void setup(Foundry::Katana::GeolibSetupInterface &interface);
    static void cook(Foundry::Katana::GeolibCookInterface &interface);
};
