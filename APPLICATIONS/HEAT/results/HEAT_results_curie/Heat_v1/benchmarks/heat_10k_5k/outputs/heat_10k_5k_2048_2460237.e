+ SCRIPT_PID=90699
+ set +x
+ /bin/bash -x /tmp/jobstart.90685
++ id -u
+ '[' 24279 -ge 1000 ']'
+ MODULESHOME=/opt/Modules
+ export MODULESHOME
+ MODULEFILES=/opt/Modules/default/modulefiles
+ export MODULEFILES
+ '[' ccc:datadir/own:dfldatadir/own:licsrv/intel:c/intel/14.0.3.174:c++/intel/14.0.3.174:fortran/intel/14.0.3.174:mkl/14.0.3.174:idb/14.0.3.174:intel/14.0.3.174:bullxmpi/1.2.7.2:fftw3/3.3.4:python/2.7.3 = '' ']'
+ ENV=/etc/profile.d/modules.sh
+ export ENV
+ BASH_ENV=/etc/profile.d/modules.sh
+ export BASH_ENV
+ FPATH=/opt/Modules/init/fpath
+ export FPATH
+ [[ hxB =~ i ]]
+ export module
+ [[ -s /opt/Modules/init/bash_completion ]]
+ [[ 4 -ge 3 ]]
+ [[ hxB =~ i ]]
+ '[' ccc:datadir/own:dfldatadir/own:licsrv/intel:c/intel/14.0.3.174:c++/intel/14.0.3.174:fortran/intel/14.0.3.174:mkl/14.0.3.174:idb/14.0.3.174:intel/14.0.3.174:bullxmpi/1.2.7.2:fftw3/3.3.4:python/2.7.3 = '' ']'
+ set -x
+ cd /ccc/cont003/home/gen7317/richarje/use-local
+ source load-env-classic-l2c.sh
++ file=classic-llcmcpp
+++ pwd
++ export PATH=/ccc/cont003/home/gen7317/richarje/use-local/classic-llcmcpp/bin:/ccc/cont003/home/gen7317/richarje/use-local/classic-llcmcpp/bin:/usr/local/fftw3-3.3.1/bin:/ccc/cont003/home/gen7317/richarje/use-local/install/pkg-config/bin:/ccc/cont003/home/gen7317/richarje/use-local/install/boost/bin:/ccc/cont003/home/gen7317/richarje/use-local/install/omniorb/bin:/ccc/cont003/home/gen7317/richarje/use-local/install/tinyxml/bin:/ccc/cont003/home/gen7317/richarje/use-local/install/cmake/bin:/ccc/cont003/home/gen7317/richarje/use-local/install/2decomp/bin:/usr/local/fftw3-3.3.1/bin:/ccc/cont003/home/gen7317/richarje/use-local/install/pkg-config/bin:/ccc/cont003/home/gen7317/richarje/use-local/install/boost/bin:/ccc/cont003/home/gen7317/richarje/use-local/install/omniorb/bin:/ccc/cont003/home/gen7317/richarje/use-local/install/tinyxml/bin:/ccc/cont003/home/gen7317/richarje/use-local/install/cmake/bin:/ccc/cont003/home/gen7317/richarje/use-local/install/2decomp/bin:/usr/lib64/qt-3.3/bin:/usr/local/ccc_python/stable2/bin:/opt/mpi/bullxmpi/1.2.7.2/bin:/opt/intel/14.0.3.174/bin/intel64:/usr/local/bin:/bin:/usr/bin:/usr/local/ccc_users_env/bin
++ PATH=/ccc/cont003/home/gen7317/richarje/use-local/classic-llcmcpp/bin:/ccc/cont003/home/gen7317/richarje/use-local/classic-llcmcpp/bin:/usr/local/fftw3-3.3.1/bin:/ccc/cont003/home/gen7317/richarje/use-local/install/pkg-config/bin:/ccc/cont003/home/gen7317/richarje/use-local/install/boost/bin:/ccc/cont003/home/gen7317/richarje/use-local/install/omniorb/bin:/ccc/cont003/home/gen7317/richarje/use-local/install/tinyxml/bin:/ccc/cont003/home/gen7317/richarje/use-local/install/cmake/bin:/ccc/cont003/home/gen7317/richarje/use-local/install/2decomp/bin:/usr/local/fftw3-3.3.1/bin:/ccc/cont003/home/gen7317/richarje/use-local/install/pkg-config/bin:/ccc/cont003/home/gen7317/richarje/use-local/install/boost/bin:/ccc/cont003/home/gen7317/richarje/use-local/install/omniorb/bin:/ccc/cont003/home/gen7317/richarje/use-local/install/tinyxml/bin:/ccc/cont003/home/gen7317/richarje/use-local/install/cmake/bin:/ccc/cont003/home/gen7317/richarje/use-local/install/2decomp/bin:/usr/lib64/qt-3.3/bin:/usr/local/ccc_python/stable2/bin:/opt/mpi/bullxmpi/1.2.7.2/bin:/opt/intel/14.0.3.174/bin/intel64:/usr/local/bin:/bin:/usr/bin:/usr/local/ccc_users_env/bin
+++ pwd
++ export LIBRARY_PATH=/ccc/cont003/home/gen7317/richarje/use-local/classic-llcmcpp/lib:/ccc/cont003/home/gen7317/richarje/use-local/classic-llcmcpp/lib:/usr/local/fftw3-3.3.1/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/pkg-config/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/boost/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/omniorb/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/tinyxml/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/cmake/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/2decomp/lib:/usr/local/fftw3-3.3.1/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/pkg-config/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/boost/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/omniorb/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/tinyxml/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/cmake/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/2decomp/lib:
++ LIBRARY_PATH=/ccc/cont003/home/gen7317/richarje/use-local/classic-llcmcpp/lib:/ccc/cont003/home/gen7317/richarje/use-local/classic-llcmcpp/lib:/usr/local/fftw3-3.3.1/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/pkg-config/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/boost/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/omniorb/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/tinyxml/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/cmake/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/2decomp/lib:/usr/local/fftw3-3.3.1/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/pkg-config/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/boost/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/omniorb/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/tinyxml/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/cmake/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/2decomp/lib:
+++ pwd
++ export LD_LIBRARY_PATH=/ccc/cont003/home/gen7317/richarje/use-local/classic-llcmcpp/lib:/ccc/cont003/home/gen7317/richarje/use-local/classic-llcmcpp/lib:11320file/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/pkg-config/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/boost/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/omniorb/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/tinyxml/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/cmake/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/2decomp/lib:11320file/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/pkg-config/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/boost/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/omniorb/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/tinyxml/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/cmake/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/2decomp/lib:/usr/local/ccc_python/stable2/lib/vtk-5.8:/usr/local/ccc_python/stable2/lib:/opt/mpi/bullxmpi/1.2.7.2/lib:/opt/intel/14.0.3.174/compiler/lib/intel64:/lib64:/lib:/usr/lib64:/usr/lib:/usr/local/intel-14.0.3.174/14.0.3.174/mkl/lib/intel64:/opt/intel/14.0.3.174/debugger/lib/intel64:/usr/local/fftw3-3.3.4/lib:.:.
++ LD_LIBRARY_PATH=/ccc/cont003/home/gen7317/richarje/use-local/classic-llcmcpp/lib:/ccc/cont003/home/gen7317/richarje/use-local/classic-llcmcpp/lib:11320file/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/pkg-config/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/boost/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/omniorb/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/tinyxml/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/cmake/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/2decomp/lib:11320file/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/pkg-config/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/boost/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/omniorb/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/tinyxml/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/cmake/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/2decomp/lib:/usr/local/ccc_python/stable2/lib/vtk-5.8:/usr/local/ccc_python/stable2/lib:/opt/mpi/bullxmpi/1.2.7.2/lib:/opt/intel/14.0.3.174/compiler/lib/intel64:/lib64:/lib:/usr/lib64:/usr/lib:/usr/local/intel-14.0.3.174/14.0.3.174/mkl/lib/intel64:/opt/intel/14.0.3.174/debugger/lib/intel64:/usr/local/fftw3-3.3.4/lib:.:.
+++ pwd
++ export CPATH=/ccc/cont003/home/gen7317/richarje/use-local/classic-llcmcpp/include:/ccc/cont003/home/gen7317/richarje/use-local/classic-llcmcpp/include:/usr/local/fftw3-3.3.1/include:/ccc/cont003/home/gen7317/richarje/use-local/install/pkg-config/include:/ccc/cont003/home/gen7317/richarje/use-local/install/boost/include:/ccc/cont003/home/gen7317/richarje/use-local/install/omniorb/include:/ccc/cont003/home/gen7317/richarje/use-local/install/tinyxml/include:/ccc/cont003/home/gen7317/richarje/use-local/install/cmake/include:/ccc/cont003/home/gen7317/richarje/use-local/install/2decomp/include:/usr/local/fftw3-3.3.1/include:/ccc/cont003/home/gen7317/richarje/use-local/install/pkg-config/include:/ccc/cont003/home/gen7317/richarje/use-local/install/boost/include:/ccc/cont003/home/gen7317/richarje/use-local/install/omniorb/include:/ccc/cont003/home/gen7317/richarje/use-local/install/tinyxml/include:/ccc/cont003/home/gen7317/richarje/use-local/install/cmake/include:/ccc/cont003/home/gen7317/richarje/use-local/install/2decomp/include:
++ CPATH=/ccc/cont003/home/gen7317/richarje/use-local/classic-llcmcpp/include:/ccc/cont003/home/gen7317/richarje/use-local/classic-llcmcpp/include:/usr/local/fftw3-3.3.1/include:/ccc/cont003/home/gen7317/richarje/use-local/install/pkg-config/include:/ccc/cont003/home/gen7317/richarje/use-local/install/boost/include:/ccc/cont003/home/gen7317/richarje/use-local/install/omniorb/include:/ccc/cont003/home/gen7317/richarje/use-local/install/tinyxml/include:/ccc/cont003/home/gen7317/richarje/use-local/install/cmake/include:/ccc/cont003/home/gen7317/richarje/use-local/install/2decomp/include:/usr/local/fftw3-3.3.1/include:/ccc/cont003/home/gen7317/richarje/use-local/install/pkg-config/include:/ccc/cont003/home/gen7317/richarje/use-local/install/boost/include:/ccc/cont003/home/gen7317/richarje/use-local/install/omniorb/include:/ccc/cont003/home/gen7317/richarje/use-local/install/tinyxml/include:/ccc/cont003/home/gen7317/richarje/use-local/install/cmake/include:/ccc/cont003/home/gen7317/richarje/use-local/install/2decomp/include:
+++ pwd
++ export PKG_CONFIG_PATH=/ccc/cont003/home/gen7317/richarje/use-local/classic-llcmcpp/lib/pkgconfig:/ccc/cont003/home/gen7317/richarje/use-local/classic-llcmcpp/lib/pkgconfig:/usr/local/fftw3-3.3.1/lib/pkgconfig:/ccc/cont003/home/gen7317/richarje/use-local/install/pkg-config/lib/pkgconfig:/ccc/cont003/home/gen7317/richarje/use-local/install/boost/lib/pkgconfig:/ccc/cont003/home/gen7317/richarje/use-local/install/omniorb/lib/pkgconfig:/ccc/cont003/home/gen7317/richarje/use-local/install/tinyxml/lib/pkgconfig:/ccc/cont003/home/gen7317/richarje/use-local/install/cmake/lib/pkgconfig:/ccc/cont003/home/gen7317/richarje/use-local/install/2decomp/lib/pkgconfig:/usr/local/fftw3-3.3.1/lib/pkgconfig:/ccc/cont003/home/gen7317/richarje/use-local/install/pkg-config/lib/pkgconfig:/ccc/cont003/home/gen7317/richarje/use-local/install/boost/lib/pkgconfig:/ccc/cont003/home/gen7317/richarje/use-local/install/omniorb/lib/pkgconfig:/ccc/cont003/home/gen7317/richarje/use-local/install/tinyxml/lib/pkgconfig:/ccc/cont003/home/gen7317/richarje/use-local/install/cmake/lib/pkgconfig:/ccc/cont003/home/gen7317/richarje/use-local/install/2decomp/lib/pkgconfig:
++ PKG_CONFIG_PATH=/ccc/cont003/home/gen7317/richarje/use-local/classic-llcmcpp/lib/pkgconfig:/ccc/cont003/home/gen7317/richarje/use-local/classic-llcmcpp/lib/pkgconfig:/usr/local/fftw3-3.3.1/lib/pkgconfig:/ccc/cont003/home/gen7317/richarje/use-local/install/pkg-config/lib/pkgconfig:/ccc/cont003/home/gen7317/richarje/use-local/install/boost/lib/pkgconfig:/ccc/cont003/home/gen7317/richarje/use-local/install/omniorb/lib/pkgconfig:/ccc/cont003/home/gen7317/richarje/use-local/install/tinyxml/lib/pkgconfig:/ccc/cont003/home/gen7317/richarje/use-local/install/cmake/lib/pkgconfig:/ccc/cont003/home/gen7317/richarje/use-local/install/2decomp/lib/pkgconfig:/usr/local/fftw3-3.3.1/lib/pkgconfig:/ccc/cont003/home/gen7317/richarje/use-local/install/pkg-config/lib/pkgconfig:/ccc/cont003/home/gen7317/richarje/use-local/install/boost/lib/pkgconfig:/ccc/cont003/home/gen7317/richarje/use-local/install/omniorb/lib/pkgconfig:/ccc/cont003/home/gen7317/richarje/use-local/install/tinyxml/lib/pkgconfig:/ccc/cont003/home/gen7317/richarje/use-local/install/cmake/lib/pkgconfig:/ccc/cont003/home/gen7317/richarje/use-local/install/2decomp/lib/pkgconfig:
+++ pwd
++ export CMAKE_INCLUDE_PATH=/ccc/cont003/home/gen7317/richarje/use-local/classic-llcmcpp/include:/ccc/cont003/home/gen7317/richarje/use-local/classic-llcmcpp/include:/usr/local/fftw3-3.3.1/include:/ccc/cont003/home/gen7317/richarje/use-local/install/pkg-config/include:/ccc/cont003/home/gen7317/richarje/use-local/install/boost/include:/ccc/cont003/home/gen7317/richarje/use-local/install/omniorb/include:/ccc/cont003/home/gen7317/richarje/use-local/install/tinyxml/include:/ccc/cont003/home/gen7317/richarje/use-local/install/cmake/include:/ccc/cont003/home/gen7317/richarje/use-local/install/2decomp/include:/usr/local/fftw3-3.3.1/include:/ccc/cont003/home/gen7317/richarje/use-local/install/pkg-config/include:/ccc/cont003/home/gen7317/richarje/use-local/install/boost/include:/ccc/cont003/home/gen7317/richarje/use-local/install/omniorb/include:/ccc/cont003/home/gen7317/richarje/use-local/install/tinyxml/include:/ccc/cont003/home/gen7317/richarje/use-local/install/cmake/include:/ccc/cont003/home/gen7317/richarje/use-local/install/2decomp/include:
++ CMAKE_INCLUDE_PATH=/ccc/cont003/home/gen7317/richarje/use-local/classic-llcmcpp/include:/ccc/cont003/home/gen7317/richarje/use-local/classic-llcmcpp/include:/usr/local/fftw3-3.3.1/include:/ccc/cont003/home/gen7317/richarje/use-local/install/pkg-config/include:/ccc/cont003/home/gen7317/richarje/use-local/install/boost/include:/ccc/cont003/home/gen7317/richarje/use-local/install/omniorb/include:/ccc/cont003/home/gen7317/richarje/use-local/install/tinyxml/include:/ccc/cont003/home/gen7317/richarje/use-local/install/cmake/include:/ccc/cont003/home/gen7317/richarje/use-local/install/2decomp/include:/usr/local/fftw3-3.3.1/include:/ccc/cont003/home/gen7317/richarje/use-local/install/pkg-config/include:/ccc/cont003/home/gen7317/richarje/use-local/install/boost/include:/ccc/cont003/home/gen7317/richarje/use-local/install/omniorb/include:/ccc/cont003/home/gen7317/richarje/use-local/install/tinyxml/include:/ccc/cont003/home/gen7317/richarje/use-local/install/cmake/include:/ccc/cont003/home/gen7317/richarje/use-local/install/2decomp/include:
+++ pwd
++ export CMAKE_LIBRARY_PATH=/ccc/cont003/home/gen7317/richarje/use-local/classic-llcmcpp/lib:/ccc/cont003/home/gen7317/richarje/use-local/classic-llcmcpp/lib:/usr/local/fftw3-3.3.1/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/pkg-config/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/boost/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/omniorb/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/tinyxml/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/cmake/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/2decomp/lib:/usr/local/fftw3-3.3.1/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/pkg-config/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/boost/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/omniorb/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/tinyxml/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/cmake/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/2decomp/lib:
++ CMAKE_LIBRARY_PATH=/ccc/cont003/home/gen7317/richarje/use-local/classic-llcmcpp/lib:/ccc/cont003/home/gen7317/richarje/use-local/classic-llcmcpp/lib:/usr/local/fftw3-3.3.1/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/pkg-config/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/boost/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/omniorb/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/tinyxml/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/cmake/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/2decomp/lib:/usr/local/fftw3-3.3.1/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/pkg-config/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/boost/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/omniorb/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/tinyxml/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/cmake/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/2decomp/lib:
+ cd /ccc/cont003/home/gen7317/richarje/helene/HEAT/Heat_v1/benchmarks/heat_10k_5k
+ export LD_LIBRARY_PATH=/ccc/cont003/home/gen7317/richarje/helene/HEAT/Heat_v1/install/lib/:/ccc/cont003/home/gen7317/richarje/use-local/classic-llcmcpp/lib:/ccc/cont003/home/gen7317/richarje/use-local/classic-llcmcpp/lib:11320file/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/pkg-config/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/boost/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/omniorb/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/tinyxml/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/cmake/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/2decomp/lib:11320file/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/pkg-config/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/boost/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/omniorb/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/tinyxml/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/cmake/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/2decomp/lib:/usr/local/ccc_python/stable2/lib/vtk-5.8:/usr/local/ccc_python/stable2/lib:/opt/mpi/bullxmpi/1.2.7.2/lib:/opt/intel/14.0.3.174/compiler/lib/intel64:/lib64:/lib:/usr/lib64:/usr/lib:/usr/local/intel-14.0.3.174/14.0.3.174/mkl/lib/intel64:/opt/intel/14.0.3.174/debugger/lib/intel64:/usr/local/fftw3-3.3.4/lib:.:.
+ LD_LIBRARY_PATH=/ccc/cont003/home/gen7317/richarje/helene/HEAT/Heat_v1/install/lib/:/ccc/cont003/home/gen7317/richarje/use-local/classic-llcmcpp/lib:/ccc/cont003/home/gen7317/richarje/use-local/classic-llcmcpp/lib:11320file/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/pkg-config/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/boost/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/omniorb/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/tinyxml/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/cmake/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/2decomp/lib:11320file/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/pkg-config/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/boost/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/omniorb/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/tinyxml/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/cmake/lib:/ccc/cont003/home/gen7317/richarje/use-local/install/2decomp/lib:/usr/local/ccc_python/stable2/lib/vtk-5.8:/usr/local/ccc_python/stable2/lib:/opt/mpi/bullxmpi/1.2.7.2/lib:/opt/intel/14.0.3.174/compiler/lib/intel64:/lib64:/lib:/usr/lib64:/usr/lib:/usr/local/intel-14.0.3.174/14.0.3.174/mkl/lib/intel64:/opt/intel/14.0.3.174/debugger/lib/intel64:/usr/local/fftw3-3.3.4/lib:.:.
+ module load mpiprofile/bullxmpi/performance
++ tclsh /opt/Modules/bin/modulecmd.tcl sh load mpiprofile/bullxmpi/performance
load module mpiprofile/bullxmpi/performance (bullxMPI profile)
+ eval 'LOADEDMODULES=ccc:datadir/own:dfldatadir/own:licsrv/intel:c/intel/14.0.3.174:c++/intel/14.0.3.174:fortran/intel/14.0.3.174:mkl/14.0.3.174:idb/14.0.3.174:intel/14.0.3.174:bullxmpi/1.2.7.2:fftw3/3.3.4:python/2.7.3:mpiprofile/bullxmpi/performance;' export 'LOADEDMODULES;' '_LMFILES_=/opt/Modules/default/modulefiles/environment/ccc:/opt/Modules/default/modulefiles/environment/datadir/own:/opt/Modules/default/modulefiles/environment/dfldatadir/own:/opt/Modules/default/modulefiles/environment/licsrv/intel:/opt/Modules/default/modulefiles/compilers/c/intel/14.0.3.174:/opt/Modules/default/modulefiles/compilers/c++/intel/14.0.3.174:/opt/Modules/default/modulefiles/compilers/fortran/intel/14.0.3.174:/opt/Modules/default/modulefiles/libraries/mkl/14.0.3.174:/opt/Modules/default/modulefiles/tools/idb/14.0.3.174:/opt/Modules/default/modulefiles/compilers/intel/14.0.3.174:/opt/Modules/default/modulefiles/parallel/bullxmpi/1.2.7.2:/opt/Modules/default/modulefiles/libraries/fftw3/3.3.4:/opt/Modules/default/modulefiles/tools/python/2.7.3:/opt/Modules/default/modulefiles/parallel/mpiprofile/bullxmpi/performance;' export '_LMFILES_;' 'OMPI_MCA_btl_openib_receive_queues=X,128,256,128,32:X,2048,256,128,32:X,12288,256,128,32:X,65536,256,128,32;' export 'OMPI_MCA_btl_openib_receive_queues;' 'OMPI_MCA_btl_openib_use_eager_rdma=1;' export 'OMPI_MCA_btl_openib_use_eager_rdma;' '_LMSTICKY__modshare=mpiprofile/bullxmpi/performance:1:python/2.7.3:1:intel/14.0.3.174:1:ccc:1:bullxmpi/1.2.7.2:1:dfldatadir/own:1:fftw3/3.3.4:1;' export '_LMSTICKY__modshare;' '_LMPREREQ__modshare=idb/14.0.3.174\&licsrv/intel:1:fortran/intel/14.0.3.174\&licsrv/intel:1:dfldatadir/own\&datadir/own:1:c++/intel/14.0.3.174\&licsrv/intel:1:mpiprofile/bullxmpi/performance\&bullxmpi:1:c/intel/14.0.3.174\&licsrv/intel:1;' export '_LMPREREQ__modshare;' '_LMCONFLICT_=dfldatadir/own\&dfldatadir:c/intel/14.0.3.174\&c/intel:c++/intel/14.0.3.174\&c++/intel:fortran/intel/14.0.3.174\&fortran/intel:mkl/14.0.3.174\&mkl:idb/14.0.3.174\&idb:intel/14.0.3.174\&intel:bullxmpi/1.2.7.2\&bullxmpi\&bullmpi\&intelmpi\&mpc\&mvapich2\&openmpi:fftw3/3.3.4\&fftw3:python/2.7.3\&python:mpiprofile/bullxmpi/performance\&mpiprofile/bullxmpi/low_memory_footprint\&openmpi\&bullmpi\&intelmpi\&mvapich2;' export '_LMCONFLICT_;' 'OMPI_MCA_mpi_leave_pinned=1;' export 'OMPI_MCA_mpi_leave_pinned;' '_LMCONFLICT__modshare=bullxmpi/1.2.7.2\&bullxmpi\&bullmpi\&intelmpi\&mpc\&mvapich2\&openmpi:1:python/2.7.3\&python:1:idb/14.0.3.174\&idb:1:fortran/intel/14.0.3.174\&fortran/intel:1:mpiprofile/bullxmpi/performance\&mpiprofile/bullxmpi/low_memory_footprint\&openmpi\&bullmpi\&intelmpi\&mvapich2:1:fftw3/3.3.4\&fftw3:1:intel/14.0.3.174\&intel:1:mkl/14.0.3.174\&mkl:1:c++/intel/14.0.3.174\&c++/intel:1:c/intel/14.0.3.174\&c/intel:1:dfldatadir/own\&dfldatadir:1;' export '_LMCONFLICT__modshare;' 'LOADEDMODULES_modshare=ccc:1:intel/14.0.3.174:1:datadir/own:1:idb/14.0.3.174:1:licsrv/intel:1:python/2.7.3:1:c++/intel/14.0.3.174:1:mkl/14.0.3.174:1:dfldatadir/own:1:bullxmpi/1.2.7.2:1:c/intel/14.0.3.174:1:mpiprofile/bullxmpi/performance:1:fortran/intel/14.0.3.174:1:fftw3/3.3.4:1;' export 'LOADEDMODULES_modshare;' '_LMFILES__modshare=/opt/Modules/default/modulefiles/libraries/fftw3/3.3.4:1:/opt/Modules/default/modulefiles/environment/ccc:1:/opt/Modules/default/modulefiles/libraries/mkl/14.0.3.174:1:/opt/Modules/default/modulefiles/environment/dfldatadir/own:1:/opt/Modules/default/modulefiles/compilers/c++/intel/14.0.3.174:1:/opt/Modules/default/modulefiles/parallel/bullxmpi/1.2.7.2:1:/opt/Modules/default/modulefiles/environment/datadir/own:1:/opt/Modules/default/modulefiles/compilers/c/intel/14.0.3.174:1:/opt/Modules/default/modulefiles/compilers/fortran/intel/14.0.3.174:1:/opt/Modules/default/modulefiles/tools/idb/14.0.3.174:1:/opt/Modules/default/modulefiles/compilers/intel/14.0.3.174:1:/opt/Modules/default/modulefiles/parallel/mpiprofile/bullxmpi/performance:1:/opt/Modules/default/modulefiles/tools/python/2.7.3:1:/opt/Modules/default/modulefiles/environment/licsrv/intel:1;' export '_LMFILES__modshare;' '_LMPREREQ_=dfldatadir/own\&datadir/own:c/intel/14.0.3.174\&licsrv/intel:c++/intel/14.0.3.174\&licsrv/intel:fortran/intel/14.0.3.174\&licsrv/intel:idb/14.0.3.174\&licsrv/intel:mpiprofile/bullxmpi/performance\&bullxmpi;' export '_LMPREREQ_;' '_LMSTICKY_=ccc:dfldatadir/own:intel/14.0.3.174:bullxmpi/1.2.7.2:fftw3/3.3.4:python/2.7.3:mpiprofile/bullxmpi/performance;' export '_LMSTICKY_;'
++ LOADEDMODULES=ccc:datadir/own:dfldatadir/own:licsrv/intel:c/intel/14.0.3.174:c++/intel/14.0.3.174:fortran/intel/14.0.3.174:mkl/14.0.3.174:idb/14.0.3.174:intel/14.0.3.174:bullxmpi/1.2.7.2:fftw3/3.3.4:python/2.7.3:mpiprofile/bullxmpi/performance
++ export LOADEDMODULES
++ _LMFILES_=/opt/Modules/default/modulefiles/environment/ccc:/opt/Modules/default/modulefiles/environment/datadir/own:/opt/Modules/default/modulefiles/environment/dfldatadir/own:/opt/Modules/default/modulefiles/environment/licsrv/intel:/opt/Modules/default/modulefiles/compilers/c/intel/14.0.3.174:/opt/Modules/default/modulefiles/compilers/c++/intel/14.0.3.174:/opt/Modules/default/modulefiles/compilers/fortran/intel/14.0.3.174:/opt/Modules/default/modulefiles/libraries/mkl/14.0.3.174:/opt/Modules/default/modulefiles/tools/idb/14.0.3.174:/opt/Modules/default/modulefiles/compilers/intel/14.0.3.174:/opt/Modules/default/modulefiles/parallel/bullxmpi/1.2.7.2:/opt/Modules/default/modulefiles/libraries/fftw3/3.3.4:/opt/Modules/default/modulefiles/tools/python/2.7.3:/opt/Modules/default/modulefiles/parallel/mpiprofile/bullxmpi/performance
++ export _LMFILES_
++ OMPI_MCA_btl_openib_receive_queues=X,128,256,128,32:X,2048,256,128,32:X,12288,256,128,32:X,65536,256,128,32
++ export OMPI_MCA_btl_openib_receive_queues
++ OMPI_MCA_btl_openib_use_eager_rdma=1
++ export OMPI_MCA_btl_openib_use_eager_rdma
++ _LMSTICKY__modshare=mpiprofile/bullxmpi/performance:1:python/2.7.3:1:intel/14.0.3.174:1:ccc:1:bullxmpi/1.2.7.2:1:dfldatadir/own:1:fftw3/3.3.4:1
++ export _LMSTICKY__modshare
++ _LMPREREQ__modshare='idb/14.0.3.174&licsrv/intel:1:fortran/intel/14.0.3.174&licsrv/intel:1:dfldatadir/own&datadir/own:1:c++/intel/14.0.3.174&licsrv/intel:1:mpiprofile/bullxmpi/performance&bullxmpi:1:c/intel/14.0.3.174&licsrv/intel:1'
++ export _LMPREREQ__modshare
++ _LMCONFLICT_='dfldatadir/own&dfldatadir:c/intel/14.0.3.174&c/intel:c++/intel/14.0.3.174&c++/intel:fortran/intel/14.0.3.174&fortran/intel:mkl/14.0.3.174&mkl:idb/14.0.3.174&idb:intel/14.0.3.174&intel:bullxmpi/1.2.7.2&bullxmpi&bullmpi&intelmpi&mpc&mvapich2&openmpi:fftw3/3.3.4&fftw3:python/2.7.3&python:mpiprofile/bullxmpi/performance&mpiprofile/bullxmpi/low_memory_footprint&openmpi&bullmpi&intelmpi&mvapich2'
++ export _LMCONFLICT_
++ OMPI_MCA_mpi_leave_pinned=1
++ export OMPI_MCA_mpi_leave_pinned
++ _LMCONFLICT__modshare='bullxmpi/1.2.7.2&bullxmpi&bullmpi&intelmpi&mpc&mvapich2&openmpi:1:python/2.7.3&python:1:idb/14.0.3.174&idb:1:fortran/intel/14.0.3.174&fortran/intel:1:mpiprofile/bullxmpi/performance&mpiprofile/bullxmpi/low_memory_footprint&openmpi&bullmpi&intelmpi&mvapich2:1:fftw3/3.3.4&fftw3:1:intel/14.0.3.174&intel:1:mkl/14.0.3.174&mkl:1:c++/intel/14.0.3.174&c++/intel:1:c/intel/14.0.3.174&c/intel:1:dfldatadir/own&dfldatadir:1'
++ export _LMCONFLICT__modshare
++ LOADEDMODULES_modshare=ccc:1:intel/14.0.3.174:1:datadir/own:1:idb/14.0.3.174:1:licsrv/intel:1:python/2.7.3:1:c++/intel/14.0.3.174:1:mkl/14.0.3.174:1:dfldatadir/own:1:bullxmpi/1.2.7.2:1:c/intel/14.0.3.174:1:mpiprofile/bullxmpi/performance:1:fortran/intel/14.0.3.174:1:fftw3/3.3.4:1
++ export LOADEDMODULES_modshare
++ _LMFILES__modshare=/opt/Modules/default/modulefiles/libraries/fftw3/3.3.4:1:/opt/Modules/default/modulefiles/environment/ccc:1:/opt/Modules/default/modulefiles/libraries/mkl/14.0.3.174:1:/opt/Modules/default/modulefiles/environment/dfldatadir/own:1:/opt/Modules/default/modulefiles/compilers/c++/intel/14.0.3.174:1:/opt/Modules/default/modulefiles/parallel/bullxmpi/1.2.7.2:1:/opt/Modules/default/modulefiles/environment/datadir/own:1:/opt/Modules/default/modulefiles/compilers/c/intel/14.0.3.174:1:/opt/Modules/default/modulefiles/compilers/fortran/intel/14.0.3.174:1:/opt/Modules/default/modulefiles/tools/idb/14.0.3.174:1:/opt/Modules/default/modulefiles/compilers/intel/14.0.3.174:1:/opt/Modules/default/modulefiles/parallel/mpiprofile/bullxmpi/performance:1:/opt/Modules/default/modulefiles/tools/python/2.7.3:1:/opt/Modules/default/modulefiles/environment/licsrv/intel:1
++ export _LMFILES__modshare
++ _LMPREREQ_='dfldatadir/own&datadir/own:c/intel/14.0.3.174&licsrv/intel:c++/intel/14.0.3.174&licsrv/intel:fortran/intel/14.0.3.174&licsrv/intel:idb/14.0.3.174&licsrv/intel:mpiprofile/bullxmpi/performance&bullxmpi'
++ export _LMPREREQ_
++ _LMSTICKY_=ccc:dfldatadir/own:intel/14.0.3.174:bullxmpi/1.2.7.2:fftw3/3.3.4:python/2.7.3:mpiprofile/bullxmpi/performance
++ export _LMSTICKY_
+ l2c_loader --mpi -c 2048 lad/heat_10k_5k_2048.lad
+ exit 0
