#ifndef HCKT_UTIL_H
#define HCKT_UTIL_H

#include <cassert>

namespace hckt
{
    /*
     * 2d get position
     * convert three part positions to memory location
     * as input: (left = 0 | right = 1) + (top = 0 | bottom = 2)
     *
     * result will be:
     * y1:x1:y2:x2:y3:x3
     *
     */
    unsigned get_position_2d(const unsigned d1, const unsigned d2, const unsigned d3)
    {
        assert(d1 < 4);
        assert(d2 < 4);
        assert(d3 < 4);

        return ((1 << 0) * ((d1 & 2) >> 1))
             + ((1 << 1) *  (d1 & 1))
             + ((1 << 2) * ((d2 & 2) >> 1))
             + ((1 << 3) *  (d2 & 1))
             + ((1 << 4) * ((d3 & 2) >> 1))
             + ((1 << 5) *  (d3 & 1));
    }

    unsigned get_x_2d(const unsigned d1, const unsigned d2, const unsigned d3)
    {
        assert(d1 < 4);
        assert(d2 < 4);
        assert(d3 < 4);

        return ((d1 & 1) << 0)
             + ((d2 & 1) << 1)
             + ((d3 & 1) << 2);
    }

    unsigned get_y_2d(const unsigned d1, const unsigned d2, const unsigned d3)
    {
        assert(d1 < 4);
        assert(d2 < 4);
        assert(d3 < 4);

        return ((d1 & 2) >> 1 << 0)
             + ((d2 & 2) >> 1 << 1)
             + ((d3 & 2) >> 1 << 2);
    }

    /*
     * 3d get position
     * convert two part positions to memory location
     * as input: (left = 0 | right = 1) + (top = 0 | bottom = 2) + (in = 0 | out = 4)
     *
     * result will be:
     * z1:y1:x1:z2:y2:x2
     *
     */
    unsigned get_position_3d(const unsigned d1, const unsigned d2)
    {
        assert(d1 < 8);
        assert(d2 < 8);

        return ((1 << 0) * ((d1 & 4) >> 2))
             + ((1 << 1) * ((d1 & 2) >> 1))
             + ((1 << 2) *  (d1 & 1))
             + ((1 << 3) * ((d2 & 4) >> 2))
             + ((1 << 4) * ((d2 & 2) >> 1))
             + ((1 << 5) *  (d2 & 1));
    }

    unsigned get_x_3d(const unsigned d1, const unsigned d2)
    {
        assert(d1 < 8);
        assert(d2 < 8);

        return ((d1 & 1) << 0)
             + ((d2 & 1) << 1);
    }

    unsigned get_y_3d(const unsigned d1, const unsigned d2)
    {
        assert(d1 < 8);
        assert(d2 < 8);

        return ((d1 & 2) >> 1 << 0)
             + ((d2 & 2) >> 1 << 1);
    }

    unsigned get_z_3d(const unsigned d1, const unsigned d2)
    {
        assert(d1 < 8);
        assert(d2 < 8);

        return ((d1 & 4) >> 2 << 0)
             + ((d2 & 4) >> 2 << 1);
    }
};

#endif

