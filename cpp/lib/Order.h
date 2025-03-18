#pragma once

#include "lib/Side.h"
#include "lib/types.h"

struct Order
{
    OrderIdT oid;
    Side side;
    PriceT price;
    SizeT size;
};
