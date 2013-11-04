#include <FnGeolib/op/FnGeolibOp.h>
namespace FnGeolibOp = FNGEOLIBOP_NAMESPACE;


class PonyfierOp : public FnGeolibOp::GeolibOp
{
public:
    static void setup(FnGeolibOp::GeolibSetupInterface &interface);
    static void cook(FnGeolibOp::GeolibCookInterface &interface);
};
