.. _stdlib_stdmathlib:

================
The Math library
================

the math lib provides basic mathematic routines. The library mimics the
C runtime library implementation.

------------
Squirrel API
------------

+++++++++++++++
Global Symbols
+++++++++++++++

.. sq:function:: abs(x)

    returns the absolute value of `x` as an integer

.. sq:function:: acos(x)

    returns the arccosine of `x`

.. sq:function:: asin(x)

    returns the arcsine of `x`

.. sq:function:: atan(x)

    returns the arctangent of `x`

.. sq:function:: atan2(x,y)

    returns the arctangent of  `x/y`

.. sq:function:: ceil(x)

    returns a float value representing the smallest integer that is greater than or equal to `x`

.. sq:function:: cos(x)

    returns the cosine of `x`

.. sq:function:: exp(x)

    returns the exponential value of the float parameter `x`

.. sq:function:: fabs(x)

    returns the absolute value of `x` as a float

.. sq:function:: floor(x)

    returns a float value representing the largest integer that is less than or equal to `x`

.. sq:function:: log(x)

    returns the natural logarithm of `x`

.. sq:function:: log10(x)

    returns the logarithm base-10 of `x`

.. sq:function:: pow(x,y)

    returns `x` raised to the power of `y`

.. sq:function:: rand()

    returns a pseudorandom integer in the range 0 to `RAND_MAX`

.. sq:function:: sin(x)

    rreturns the sine of `x`

.. sq:function:: sqrt(x)

    returns the square root of `x`

.. sq:function:: srand(seed)

    sets the starting point for generating a series of pseudorandom integers

.. sq:function:: tan(x)

    returns the tangent of `x`

.. sq:function:: min(x, y, [z], [w], ...)

    returns minimal value of all arguments

.. sq:function:: max(x, y, [z], [w], ...)

    returns maximal value of all arguments

.. sq:function:: clamp(x, min_val, max_val)

    returns value limited by provided min-max range

.. sq:data:: RAND_MAX

    the maximum value that can be returned by the `rand()` function

.. sq:data:: PI

    The numeric constant pi (3.141592) is the ratio of the circumference of a circle to its diameter

------------
C API
------------

.. _sqstd_register_mathlib:

.. c:function:: SQRESULT sqstd_register_mathlib(HSQUIRRELVM v)

    :param HSQUIRRELVM v: the target VM
    :returns: an SQRESULT
    :remarks: The function aspects a table on top of the stack where to register the global library functions.

    initializes and register the math library in the given VM.

