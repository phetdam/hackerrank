.. README.rst

hackerrank
==========

A collection of HackerRank_ problems solved for interview prep or as exercises.

I dislike doing these since the problems are contrived and encourage sloppy
programming habits but the reality is that too many companies use coding tests
for initial screenings so these are a necessary hardship. However, I will
concede that like in the classroom setting, contrived problems limited in scope
do provide some educational value.

This repo serves as a way to incentivize myself to persist with such
teeth-pulling via commit history gamification as well as facilitate local
debugging of large test cases. One cannot run their debugger of choice on the
HackerRank problem pages and some HackerRank inputs exceed the size limits
imposed on uploaded custom inputs.

TBD: Notes on the homegrown test comparison functions in ``compare.hh``.

.. _HackerRank: https://www.hackerrank.com/

Building from source
--------------------

CMake_ >=3.16 is required to build from source on all platforms.

.. _CMake: https://cmake.org/cmake/help/latest/

\*nix
~~~~~

Building is easy with the provided ``build.sh`` build script. For usage, type

.. code:: bash

   ./build.sh --help

To build release binaries for this project, simply use the command

.. code:: bash

   ./build.sh -c Release

Simply typing ``./build.sh`` will build unoptimized binaries with debug symbols.

Windows
~~~~~~~

Building is easy with the provided ``build.bat`` build script. For usage, type

.. code:: shell

   build --help

To build 64-bit release binaries for this project, simply invoke

.. code:: shell

   build -c Release

Simply typing ``build`` will build 64-bit unoptimized binaries and the PDB
files. You can specify the target architecture using the ``-a`` flag, so to
build 32-bit unoptimized binaries, use

.. code:: shell

   build -a x86

The Visual Studio toolset used will be whichever is the default.

Running tests
-------------

Tests are registered by conditionally compiling the HackerRank submissions with
the ``PDHKR_TEST``, ``PDHKR_TEST_INPUT``, and ``PDHKR_TEST_OUTPUT`` macros
defined. Each test uses the name of the original program target as its name
prefix, suffixed with an underscore and the stem of the respective input file.
These test cases do not include all the ones actually run for each HackerRank
problem but suffice for correctness testing.

\*nix
~~~~~

Running the tests is easy with the provided ``check.sh`` test harness script.
For usage, type

.. code:: bash

   ./check.sh --help

To run the tests, which are done in parallel by default, one simply invokes

.. code:: bash

   ./check.sh

For serial test running or for lower test parallelism one can use the CTest_
``-j`` option, e.g.

.. code:: bash

   ./check.sh -Ct -j4

By default, ``check.sh`` uses a value of ``$(nproc)`` for the ``-j`` flag.

.. _CTest: https://cmake.org/cmake/help/latest/manual/ctest.1.html

Windows
~~~~~~~

To run the tests in parallel, e.g. for 32-bit Debug artifacts, one invokes
CTest as follows:

.. code:: shell

   ctest --test-dir build_windows_x86 -C Debug -j%NUMBER_OF_PROCESSORS%

If release artifacts were built, ``-C Release`` can be used to run the release
mode tests.
