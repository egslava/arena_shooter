.. Arena documentation master file, created by
   sphinx-quickstart on Tue Jan 15 17:34:59 2019.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

Getting started
---------------

Installation
~~~~~~~~~~~~
.. code:: bash

    cd client/build
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

Can check why ``make uninstall`` is not available in CMake [1]_. Thanks to Emilio González  [2]_

.. toctree::
   :maxdepth: 2
   :caption: Contents:


.. Comment
.. Indices and tables
.. ==================
.. * :ref:`genindex`
.. * :ref:`modindex`
.. * :ref:`search`

.. [1] https://gitlab.kitware.com/cmake/community/wikis/FAQ#can-i-do-make-uninstall-with-cmake
.. [2] https://stackoverflow.com/a/44649542/1444191