=====================================
src/Sync/Sync.cpp :
=====================================

deux versions possibles:
1- simple héritage de go : récupération des pointeurs à chaque itération par appel virtuel sur Data
2- héritage go et synch : init à lancer dans DriverApp pour faire un seul appel virtuel

Version 2 activée pour weak/strong scaling
Mais ça n'a pas l'air de changer grand chose

=====================================
src/SeqParOmp/SeqParOmp.cpp :
=====================================

Scheduling statique openmp
Deux versions possibles :
1- politique d'ordonnancement close
2- politique d'ordonnancement spread

Les deux ont étés évalués

=====================================
src/SeqParOmp/SeqParOmpFus.cpp :
=====================================

Scheduling statique openmp mais avec fusion de composants de façon à coller à 2 threads
Deux versions possibles :
1- politique d'ordonnancement close
2- politique d'ordonnancement spread

Comme on peut s'y attendre ce n'est pas intéressant car trop peu de tâches

=====================================
src/DynOmp/DynOmp.cpp :
=====================================

Scheduling dynamique openmp 4.x
Deux versions possibles :
1- politique d'ordonnancement close
2- politique d'ordonnancement spread

=====================================
src/TimeAll/TimeAll.cpp :
=====================================

Scheduling dynamique openmp 4.x sans synchronisation à chaque itération de temps
Deux versions possibles :
1- politique d'ordonnancement close
2- politique d'ordonnancement spread


=====================================
src/Kernels :
=====================================

Tous les kernels classiques qui utilise le Controller (SkelGIS)
include/controller.hpp
On y trouve les kernel sans fusion et avec fusion.
Ceux avec fusion sont notés avec le nom de chaque kernel séparé par _
exemple : Kbloc1y1_bloc1y2.cpp

=====================================
src/KernelsOmp :
=====================================

Tous les kernels avec une boucle openMP for
Différentes politiques possibles : schedule(static,chunk) / schedule(static) / schedule(dynamic,chunk) / schedule(dynamic)
On y trouve les kernel sans fusion et avec fusion.
Les noms terminent par _omp
Ceux avec fusion sont notés avec le nom de chaque kernel séparé par _
exemple : Kbloc1y1_bloc1y2_omp.cpp

=====================================
include/libskelgis/comunications.hpp :
=====================================

Attention j'ai enlevé la sérialisation des données pour vérifier qu'un surcoût ne venait pas du memcpy
MPI_DOUBLE utilisé

=====================================
src/Assembly/Assembly.cpp :
=====================================

Ce composant est nécessaire à la version L2CLIGHT (utilisée pour le weak et strong scaling)
Il créée directement les composants et les communications.
Il est directement utilisé dans l2c.git/base/src/llcmcpp_loader.cpp

=====================================
src/Time/Time.cpp :
=====================================

Il y avait un énorme surcoût aux premiers appels MPI dans la première itération
Surement dus aux lib dynamiques
Une itération est effectuée en plus et non prise en compte dans le calcul de temps


=====================================
CMakeLists.txt :
=====================================

-O3
le -fopenmp est à ajouter si SeqParOmp utilisé
Bien faire attention sur Curie aux compilateurs utilisés :
- utiliser ccmake pour vérifier
- make VERBOSE=1 pour vérifier