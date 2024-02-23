.. _executioncontext:

=======================
Execution Context
=======================

.. index::
    single: execution context

The execution context is the union of the function stack frame and the compile-time bindings and constants.
The stack frame is the portion of stack where the local variables declared in its body are stored.
The environment object (``this``) is an implicit parameter that is automatically passed by the
function caller (see see :ref:`functions <functions>`).
During the execution, the body of a function can only transparently refer to its execution
context.
The root table can be explicitly accessed through the operator ``::`` (see :ref:`Variables <variables>`).

.. _named_bindings:

-----------------
Named bindings
-----------------

Named binding is an object that holds the result of evaluated initializer expressions.
Unlike variables it cannot change its value.

Examples:

::

    let a = 23+34
    let b = [1, 2, 3]
    function foo() { return "some_value" }

.. _variables:

-----------------
Variables
-----------------

There are two types of variables in Squirrel, local variables and tables/arrays slots.
Because global variables(variables stored in the root of a closure) are stored in a table, they are table slots.

A single identifier refers to a local variable or a slot in the environment object.::

    derefexp := id;

::

    _table["foo"]
    _array[10]

with tables we can also use the '.' syntax::

    derefexp := exp '.' id

::

    _table.foo

Squirrel first checks if an identifier is a local variable (function arguments are local
variables) if not looks up the environment object (this) and finally looks up
to the closure root.

For instance:::

    function testy(arg)
    {
        local a=10;
        print(a);
        return arg;
    }

in this case 'foo' will be equivalent to 'this.foo' or this["foo"].

Global variables are stored in a table called the root table. Usually in the global scope the
environment object is the root table, but to explicitly access the closure root of the function from
another scope, the slot name must be prefixed with ``'::'`` (``::foo``).

For instance:::

    function testy(arg)
    {
        local a=10;
        return arg+::foo;
    }

accesses the variable 'foo' in the closure root table.

Since Squirrel 3.1 each function has a weak reference to a specific root table, this can differ from the current VM root table.::

    function test() {
        foo = 10;
    }

is equivalent to write::

    function test() {
        if("foo" in this) {
            this.foo = 10;
        }else {
            ::foo = 10;
        }
    }
