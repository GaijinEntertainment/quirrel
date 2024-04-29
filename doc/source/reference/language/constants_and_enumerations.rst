.. _constants_and_enumerations:


========================
Constants & Enumerations
========================

.. index::
    single: Constants & Enumerations



Quirrel allows to bind constant values to an identifier that will be evaluated compile-time.
This is achieved though constants and Enumerations.

---------------
Constants
---------------

.. index::
    single: Constants

Constants bind a specific value to an identifier. Constants are similar to
global values, except that they are evaluated compile time and their value cannot be changed.

constants values can only be integers, floats or string literals. No expression are allowed.
are declared with the following syntax.::

    const foobar = 100
    const floatbar = 1.2
    const stringbar = "I'm a constant string"

Constants are locally scoped, from the moment they are declared.
To make constant global, use the ``global`` keyword.::

    global const baz = 123

::

    let x = foobar * 2

..  Warning::
  Constant values are initialized at compile-time when code is being generated, not executed.
  And their values can be reassigned with new ones (also during compilation) while the same constant objects is being used in generated code.

Example::

  global const foo = 0
  if ("foo" not in getconsttable()) 
    global const foo = 1
  print(foo) //prints 1

---------------
Enumerations
---------------

.. index::
    single: Enumerations

As Constants, Enumerations bind a specific value to a name. Enumerations are also evaluated at compile time
and their value cannot be changed.

An enum declaration introduces a new enumeration into the program.
Enumeration values can only be integers, floats or string literals. No expression are allowed.::

    enum Stuff {
      first, //this will be 0
      second, //this will be 1
      third //this will be 2
    }

or::

    enum Stuff {
      first = 10
      second = "string"
      third = 1.2
    }

An enum value is accessed in a manner that's similar to accessing a static class member.
The name of the member must be qualified with the name of the enumeration, for example ``Stuff.second``

::

    let x = Stuff.first * 2

Like with constants, to declare globally-scoped enum, use the ``global`` keyword.::

    global enum Stuff { first, second }

--------------------
Implementation notes
--------------------

Enumerations and Constants are a compile-time feature. Only integers, string and floats can be declared as const/enum;
No expressions are allowed (because they would have to be evaluated compile time).

If a const or enum is global, when declared it is added compile time to the ``consttable``. This table is stored in the VM shared state
and is shared by the VM and all its threads.
The ``consttable`` is a regular quirrel table; In the same way as the ``roottable``
it can be modified runtime.
You can access the ``consttable`` through the built-in function ``getconsttable()``
and also change it through the built-in function ``setconsttable()``

here some example: ::

    //creates a constant
    getconsttable()["something"] <- 10"
    //creates an enumeration
    getconsttable()["somethingelse"] <- { a = "10", c = "20", d = "200"};
    //deletes the constant
    delete getconsttable()["something"]
    //deletes the enumeration
    delete getconsttable()["somethingelse"]

This system allows to procedurally declare constants and enumerations, it is also possible to assign any quirrel type
to a constant/enumeration(function,classes etc...). However this will make serialization of a code chunk impossible.
