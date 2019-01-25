.. Arena documentation master file, created by
   sphinx-quickstart on Tue Jan 15 17:34:59 2019.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

.. toctree::
   :maxdepth: 2
   :caption: Contents:

   .. getting-started-windows

===============
Getting Started
===============

.. _Project Directory:

**Project directory** is a directory where you cloned `the repository <repo_address_>`_. It should have **client** and **docs-src** folders inside.

Building
========

Please, fork `the repository <repo_address_>`_ and clone it to your PC. Open CMD/Bash/Terminal and go to the project's directory. Then type:

.. code:: bash

   cd client
   mkdir build
   cd build



On Windows
----------

Dependencies
~~~~~~~~~~~~

1. Visual Studio (C++).
2. `CMake <https://cmake.org/download/>`_.


Build & Run
~~~~~~~~~~~

Debug
^^^^^

.. code:: bash

   cmake ..
   cmake --build .  --target ALL_BUILD --config Debug
   Debug\Arena.exe



Release
^^^^^^^

.. code:: bash

   cmake ..
   cmake --build . --target ALL_BUILD --config Release
   Release\Arena.exe





On Ubuntu
---------

Dependencies
~~~~~~~~~~~~

1. cmake
1. libsdl2-dev
2. libglew-dev



Build & Run
~~~~~~~~~~~

Debug
^^^^^

.. code:: bash

   cmake  .. -DCMAKE_BUILD_TYPE=Debug
   cmake --build .
   ./Arena



Release
^^^^^^^

.. code:: bash

   cmake  .. -DCMAKE_BUILD_TYPE=Release
   cmake --build . --target all
   ./Arena






Installing
==========
Attention! The installers are INCOMPLETE YET!

Windows
-------
To be done


Ubuntu
------
Please, pay attention, the installer for Ubuntu is not completed yet. It installs the executrable files, but it doesn't copy all the resources!

Installation
~~~~~~~~~~~~
.. code:: bash

    cmake -G "Unix Makefiles" -D USE_MYMATH=ON ..
    make
    make install



Run
~~~
.. code:: bash

    Arena



Uninstallation
~~~~~~~~~~~~~~
.. code:: bash

    cd client/build
    cmake -G "Unix Makefiles" -D USE_MYMATH=ON ..
    make
    make install
    sudo xargs rm < install_manifest.txt

``make uninstall`` is intentionally not available in CMake [1]_. Thanks to Emilio González for the clarification [2]_.





=========================
Writing the documentation
=========================

Dependencies
============
1. `Python 3 <https://wiki.python.org/moin/BeginnersGuide/Download>`_. Python 3 ``THREE``.
2. You need to install `Sphynx <http://www.sphinx-doc.org/en/master/usage/installation.html>`_.


Build and run
=============
Go to the `Project Directory`_/docs-src and run *make html*. The output will appear in the `Project Directory`_/docs-src/build/html/index.html. It's very comfortable to add this file to bookmarks.


How to write docs?
==================
Firstly, please, read `the official tutorial <http://docs.sphinxdocs.com/en/latest/>`_.
Then, use `the cheatsheet <http://docs.sphinxdocs.com/en/latest/cheatsheet.html>`_.

**Commit your results** to `the repo <repo_address_>`_, after you have changed the docs! :)


.. Header1
.. =======
.. 
.. Header2
.. =======
.. 
.. Header3
.. -------
.. 
.. Header4
.. ~~~~~~~
.. 
.. Header5
.. ^^^^^^^


.. _repo_address: https://github.com/egslava/arena_shooter

.. Comment
.. Indices and tables
.. ==================
.. * :ref:`genindex`
.. * :ref:`modindex`
.. * :ref:`search`

.. [1] https://gitlab.kitware.com/cmake/community/wikis/FAQ#can-i-do-make-uninstall-with-cmake
.. [2] https://stackoverflow.com/a/44649542/1444191