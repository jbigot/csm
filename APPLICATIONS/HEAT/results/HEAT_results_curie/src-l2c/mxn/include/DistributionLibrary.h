#ifndef DISTRIBUTION_LIBRARY_IS_DEFINED
#define DISTRIBUTION_LIBRARY_IS_DEFINED

#include <vector>

namespace mxn {

  // To delete ?
  struct PieceToSend {
	 unsigned sourceNode; // Quel noeud envoi
	 unsigned destNode; // Quel est le noeud destinataire
	 unsigned long size; // nb octet ! la taille que l'on envoi
	 void* id; // Cette identifiant est ensuite redonnée a la bibliothèque lorsque l'on demande 
	 // réellement les données
  };

  //  typedef std::vector<PieceToSend> vPieceToSend_t;

  struct Topology_t {
	 unsigned total;
  };

  class DistributionLibrary {
  public:
	 // Topology
	 virtual void setSourceTopology(const Topology_t& topo) = 0; // paco_operation
	 virtual Topology_t getSourceTopology() = 0;          // ??
	 virtual void setDestTopology(const Topology_t& topo) = 0;   // paco_operation
	 virtual Topology_t getDestTopology() = 0;            // ??
	 virtual void setNodeRank(long Rank) = 0;                       // paco_operation
	 virtual long getNodeRank() = 0;                                // by user ?

	 // Com Id management
	 virtual bool setComId(long id) { return false;}
	 virtual bool freeComId(long id) { return false;}

	 // If the library is parallel
	 virtual void  setCommunicator(void * group) = 0;
	 //  virtual void* getCommunicator() = 0; PROPOSED EXTENSION
  };
  
  class DistributionLibraryClient : virtual public DistributionLibrary {
  public:
	 // Client Side
	 // Computes communications vector (or eventually matrix)
	 // Cette méthode est appellée par PaCO++ pour demander la matrice de communication
	 virtual PieceToSend* computePiecesToSend(unsigned& size) = 0;
	 // virtual PieceToSend* computePiecesToSend2(unsigned& size) = 0; SHOULD COMPUTE DIRECTLY THE FULL MATRIX
	 // Sets client-side data 
	 // Pointeur vers les données client
	 virtual void setDataPtr(void* dataPtr) = 0;

	 // Get data
	 // PaCO++ demande une quantité size pour le server_node
	 // length est la taille réelle que l'on lui donne
	 // end permet de fragmenter le message en plusieurs buffer : si on veut envoyer 
	 // qu'un seul buffer on repond true sinon on repond false
	 // pid est l'id contenu dans PieceToSend
	 // size est la taille que PaCO++ veut et on soustrait ce qu'on donne réellement
	 virtual void* getClientData(void *pid, unsigned server_node, unsigned long & size, unsigned long & length, bool & end) = 0;
	 // PaCO++ demande le descripteur qui sera attaché à l'envoi des données.
	 // PaCO++ caste le void * dans le type décrit dans le fichier de configuration de la lib 
	 // situé dans le répertoires Types.
	 virtual void* getDescr() = 0;

	 // Free
	 virtual void clientFree() = 0;
  };

  class DistributionLibraryServer : virtual public DistributionLibrary {
  public:
	 // Server Side
	 // Sets server-side data return true if data complete
	 // element_nb : nombre d'etls de la sequence != 1 si
	 // si le message est fragmenté
	 virtual bool insertData(void* rcptBuf, unsigned long element_nb) = 0; 
	 virtual void setDescr(void * descr) = 0;

	 // getData for the servant
	 virtual void* getServerData(unsigned long & length) = 0;

	 // Free
	 virtual void serverFree() = 0;
  };


}
#endif
