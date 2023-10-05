#include <mat.h>


/*  Commonly used constants  */

const double pi = pic,
                    twopi = pic * 2.0,
                    piover4 = pic / 4.0,
                    fouroverpi = 4.0 / pic,
                    piover2 = pic / 2.0;

/*  Coefficients for ATAN evaluation  */

const double atanc[] = {
        0.0,
        0.4636476090008061165,
        0.7853981633974483094,
        0.98279372324732906714,
        1.1071487177940905022,
        1.1902899496825317322,
        1.2490457723982544262,
        1.2924966677897852673,
        1.3258176636680324644
};

/*  aint(x)       Return integer part of number.  Truncates towards 0    */

double aint(double x)
{
        long l;

        /*  Note that this routine cannot handle the full floating point
            number range.  This function should be in the machine-dependent
            floating point library!  */

        l = x;
        if ((int)(-0.5) != 0  &&  l < 0 ) {
           l++;
        }
        x = l;
        return x;
}

/*  sin(x)        Return sine, x in radians  */

double sin(double x)
{
        int sign;
        double y, r, z;

        x = (((sign= (x < 0.0)) != 0) ? -x: x);

        if (x > twopi) {
           x -= (aint(x / twopi) * twopi);
        }

        if (x > pi) {
           x -= pi;
           sign = !sign;
        }

        if (x > piover2) {
           x = pi - x;
        }

        if (x < piover4) {
           y = x * fouroverpi;
           z = y * y;
           r = y * (((((((-0.202253129293E-13 * z + 0.69481520350522E-11) * z -
              0.17572474176170806E-8) * z + 0.313361688917325348E-6) * z -
              0.365762041821464001E-4) * z + 0.249039457019271628E-2) * z -
              0.0807455121882807815) * z + 0.785398163397448310);
        } else {
           y = (piover2 - x) * fouroverpi;
           z = y * y;
           r = ((((((-0.38577620372E-12 * z + 0.11500497024263E-9) * z -
              0.2461136382637005E-7) * z + 0.359086044588581953E-5) * z -
              0.325991886926687550E-3) * z + 0.0158543442438154109) * z -
              0.308425137534042452) * z + 1.0;
        }
        return sign ? -r : r;
}

/*  cos(x)        Return cosine, x in radians, by identity  */

double cos(double x)
{
        x = (x < 0.0) ? -x : x;
        if (x > twopi) {                /* Do range reduction here to limit */
           x = x - (aint(x / twopi) * twopi); /* roundoff on add of PI/2    */
        }
        return sin(x + piover2);
}

/*  tan(x)        Return tangent, x in radians, by identity  */

double tan(double x)
{
        return sin(x) / cos(x);
}

/*  sqrt(x)       Return square root.  Initial guess, then Newton-
                  Raphson refinement  */

double sqrt(double x)
{
        double c, cl, y;
        int n;

        if (x == 0.0) {
           return 0.0;
        }

        if (x < 0.0) {
		/* ERROR */
        }

        y = (0.154116 + 1.893872 * x) / (1.0 + 1.047988 * x);

        c = (y - x / y) / 2.0;
        cl = 0.0;
        for (n = 50; c != cl && n--;) {
           y = y - c;
           cl = c;
           c = (y - x / y) / 2.0;
        }
        return y;
}

/*  atan(x)       Return arctangent in radians,
                  range -pi/2 to pi/2  */

double atan(double x)
{
        int sign, l, y;
        double a, b, z;

        x = (((sign = (x < 0.0)) != 0) ? -x : x);
        l = 0;

        if (x >= 4.0) {
           l = -1;
           x = 1.0 / x;
           y = 0;
           goto atl;
        } else {
           if (x < 0.25) {
              y = 0;
              goto atl;
           }
        }

        y = aint(x / 0.5);
        z = y * 0.5;
        x = (x - z) / (x * z + 1);

atl:
        z = x * x;
        b = ((((893025.0 * z + 49116375.0) * z + 425675250.0) * z +
            1277025750.0) * z + 1550674125.0) * z + 654729075.0;
        a = (((13852575.0 * z + 216602100.0) * z + 891080190.0) * z +
            1332431100.0) * z + 654729075.0;
        a = (a / b) * x + atanc[y];
        if (l) {
           a = piover2 - a;
        }
        return sign ? -a : a;
}

/*  atan2(y,x)    Return arctangent in radians of y/x,
                  range -pi to pi  */

double atan2(double y, double x)
{
        double temp;

        if (x == 0.0) {
           if (y == 0.0) { /*  Special case: atan2(0,0) = 0  */
              return 0.0;
           } else {
              if (y > 0) {
                 return piover2;
              } else {
                 return -piover2;
              }
           }
        }
        temp = atan(y / x);
        if (x < 0.0) {
           if (y >= 0.0) {
              temp += pic;
           } else {
              temp -= pic;
           }
        }
        return temp;
}

/*  asin(x)       Return arcsine in radians of x  */

double asin(double x)
{
        if (fabs(x) > 1.0) {
		/* ERROR */
        }
        return atan2(x, sqrt(1 - x * x));
}




double pow(double base, int exponent) 
{
    double result = 1.0;

    if (exponent < 0) {
        base = 1.0 / base;
        exponent = -exponent;
    }

    while (exponent > 0) {
        if (exponent % 2 == 1) {
            result *= base;
        }
        base *= base;
        exponent /= 2;
    }

    return result;
}
