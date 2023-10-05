
#ifndef MAT_H
#define MAT_H

#define cot(x) (1.0 / tan(x))

#define TRUE  1
#define FALSE 0



/*  Internal trig functions (used only if INTRIG is  defined).   These
    standard  functions  may be enabled to obtain timings that reflect
    the machine's floating point performance rather than the speed  of
    its trig function evaluation.  */


#define fabs(x)  ((x < 0.0) ? -x : x)

#define pic 3.1415926535897932

/*  Commonly used constants  */

extern const double pi;
extern const double twopi;
extern const double piover4;
extern const double fouroverpi;
extern const double piover2;

/*  Coefficients for ATAN evaluation  */

extern const double atanc[];

/*  aint(x)       Return integer part of number.  Truncates towards 0    */

double aint(double x);

/*  sin(x)        Return sine, x in radians  */

double sin(double x);

/*  cos(x)        Return cosine, x in radians, by identity  */

double cos(double x);

/*  tan(x)        Return tangent, x in radians, by identity  */

double tan(double x);

/*  sqrt(x)       Return square root.  Initial guess, then Newton-
                  Raphson refinement  */

double sqrt(double x);

/*  atan(x)       Return arctangent in radians,
                  range -pi/2 to pi/2  */

double atan(double x);

/*  atan2(y,x)    Return arctangent in radians of y/x,
                  range -pi to pi  */

double atan2(double y, double x);

/*  asin(x)       Return arcsine in radians of x  */

double asin(double x);

double pow(double base, int exponent);

#endif  /* MATH_H */
