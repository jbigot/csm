template_process.xml : le fichier avec la fusion pour le weak et strong scaling
template_process2.xml : le fichier sans fusion, pour les versions de base en hybride
template_process_omp.xml : le fichier sans fusion et omp 3.x (task without depend) avec l'utilisation du composant SeqParOmp
template_process_ompFus.xml : le fichier avec fusion et omp avec l'utilisation du composant SeqParOmpFus
template_process_ompfor.xml : le fichier sans fusion avec des ompfor dans les kernels
template_process_dynomp.xml : le fichier sans fusion et avec omp 4.x (task et depend) avec l'utilisation de DynOmp
template_process_dynompAll.xml : le fichier sans fusin et avec omp 4.x (task et depend) sans synchronisation à chaque itération (task graph global). Utilisation de TimeAll qui contient tout le code. PB : si trop de tâches pour le scheduler -> placer synchro par blocs d'itérations.
