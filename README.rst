.. README.rst

hackerrank
==========

A collection of HackerRank_ problems solved for interview prep or as exercises.

I hate doing these since the problems are contrived and encourage sloppy
programming habits but the reality is that too many companies use coding tests
for initial screenings so these are a necessary hardship. However, I will
concede that like in the classroom setting, contrived problems limited in scope
do provide some educational value.

This repo serves as a way to incentivize myself to persist with such
teeth-pulling via commit history gamification as well as facilitate local
debugging of large test cases. One cannot run their debugger of choice on the
HackerRank problem pages and some HackerRank inputs exceed the size limited
imposed on uploaded custom inputs.

TBD for building. Maybe I should try *not* doing these in C++.

TODO: Maybe add a submission simulation driver to enforce limits and make it
easier to run through different inputs and ouputs while reporting failures.
E.g. C/C++ programs have 2s runtime limit, 512 MB memory usage limit, and in
general submission code size must be less than 50 KB (very generous, probably
no need to check). It would be nice to get a warning when a runtime limit is
exceeded and if there is a difference in output, a line-by-line diff.

.. _HackerRank: https://www.hackerrank.com/
