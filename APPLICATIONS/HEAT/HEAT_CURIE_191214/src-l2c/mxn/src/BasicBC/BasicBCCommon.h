#ifndef _BASICBC_H
#define _BASICBC_H

#include <list>
#include <map>

#include "DistributionLibrary.h"
#include "BasicBC_Redist.h"
#include "BasicBC.h"
#include "basicbcconfig.h"

namespace mxn
{

namespace impl
{

  ////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////
  
  typedef enum { BASICBC_BLOC=1, // no parameter: all bloc same size but last one: bc(n+p-1/p)
					  BASICBC_CYCLIC,  // no parameter: bc(1)
					  BASICBC_BLOCCYCLIC // parameter
  } BasicBC_distri_type_t;

  ////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////
  
  typedef struct BasicBC_param_s {
	 BasicBC_distri_type_t type;
	 unsigned long unitsize;  // size of one element of our "vector"; in octet
	 unsigned long blocsize; // for bloc-cyclic; in element
  } BasicBC_param_t;


  ////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////

  // Client & Server side
  class BasicBCCommon : virtual public BasicBC, virtual public BasicBCConfig {
  protected:	 
	 unsigned _config; // vector of bit describing which elements have been configured (cf .cc)
	 bool _dirty; // true means that the config has changed => need to recompute cached value
	 
	 Topology_t _sTopo; // topology of the source (sender)  -- SET
	 Topology_t _dTopo; // topology of the destination (receiver) -- SET

	 BasicBC_param_t _param;    // the parameter of the distribution -- SET
	 unsigned _nodeRank;        // id of the node (either source or destination) -- SET

	 unsigned _glen;     // total vector size: in element -- SET
	 unsigned _llen;     // local vector size: in element -- COMPUTED

	 unsigned _sbsz;     // source blocsize in element -- COMPUTED
	 unsigned _dbsz;     // source blocsize in element -- COMPUTED

	 unsigned long _gfirst;  //  COMPUTED by sub class
	 unsigned long _glast;   //  COMPUTED by sub class

  public:
	 BasicBCCommon();
	 virtual ~BasicBCCommon();

	 // Distribution Library
	 virtual void setSourceTopology(const Topology_t& topo); 
	 virtual Topology_t getSourceTopology(); 
	 virtual void setDestTopology(const Topology_t& topo); 
	 virtual Topology_t getDestTopology(); 
	 virtual void setNodeRank(long Rank);
	 virtual long getNodeRank(); 
	 virtual void setCommunicator(void * group) {}

	 // Data description API
	 virtual void setEltSize(unsigned long size);
	 virtual void setBlocSize(unsigned long bsz); // bloc-> bc(0), cyclic -> bc(1)
	 virtual void setTotalNbElt(unsigned long elt_nb);
	 virtual unsigned long getBlocSize();
	 virtual unsigned long getLocalNbElt();
	 virtual unsigned long getTotalNbElt();
  };

  
  ////////////////////////////////////////
  // Client Side
  class BasicBC_Client : virtual public BasicBCCommon, virtual public BasicClientConfig {
  protected:

	 unsigned long _lstart;   // id of 1st local element of src    -- COMPUTED

	 // 	 unsigned long _gstep;   //  cf sbsz or dbsz

	 // Info passed between computePiecesToSend & getClientData
	 // is in fact a map of info_t with destNode as key :(
	 // Private type
	 typedef struct info_s {
		unsigned gstart;    // in element
		unsigned lstart;    // in octet
		unsigned msg_size;  // in octet
		unsigned sent_size; // in octet 
	 } info_t;
	 typedef std::list<info_t*> info_list_t;
	 typedef std::vector<info_list_t*> info_vector_t;

	 long _cur_id; // current communication id, -1 means none
	 info_vector_t* _infolists; // current array of dTotal entries

	 typedef std::map<long, info_vector_t*> info_list_map_t; // also store com for id == -1
	 info_list_map_t _info_list_map;
	 
	 bool _descr_to_be_cleaned; // true when descr should be reset - cf getClientData & getDescr (during an invocation)
	 char* _clientBuffer; // pointer to the user data (set)

	 BasicBCDescr _clientDescr; // contains the descr exchange between sender & destination -- SET indirectly

  public:
	 BasicBC_Client();
	 virtual ~BasicBC_Client();

	 // Client Side
	 virtual PieceToSend* computePiecesToSend(unsigned& size);
	 virtual void  setDataPtr(void* dataPtr);
	 virtual void* getClientData(void *pid, unsigned server_node, unsigned long & size, unsigned long& length, bool & end);
	 virtual void* getDescr();
	 virtual void  clientFree(); // true if com schedule to be freed
	 
	 virtual unsigned long getFirst();
	 virtual unsigned long getLast();
	 virtual unsigned long getStep();

#ifdef MEMORY
	 virtual bool setComId(long id);
	 virtual bool freeComId(long id);
#endif

private:  
#ifdef MEMORY
	 void internalSetComId(long id);
	 void internalFreeComId(info_list_map_t::iterator& it);
	 
	 // Helper function
	 info_vector_t*  allocate_new_info_list();
	 void reset_info_list(info_vector_t&);
	 void clear_info_list(info_vector_t&);
#endif
  };

  ////////////////////////////////////////
  // Server Side
  class BasicBC_Server : virtual public BasicBCCommon, virtual public DistributionLibraryServer
  {
	 protected:
  
	 unsigned long _serverToReceived; // #of element yet to be received (during an invocation)
	 unsigned _descr_ids_counter; // id of the current block (msg = sequence<block>) (during an invocation)
	 char* _serverBuffer; // Buffer where to receive the data

	 BasicBCDescr* _serverDescr; // contains the descr exchange between sender & destination -- computed
	 
  public:
	 BasicBC_Server();
	 virtual ~BasicBC_Server();

	 virtual bool  insertData(void* rcptBuf, unsigned long element_nb);
	 virtual void  setDescr(void * descr);

	 virtual void* getServerData(unsigned long & length);
	 virtual void  serverFree();

	 virtual unsigned long getFirst();
	 virtual unsigned long getLast();
	 virtual unsigned long getStep();

  };

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

}

} // namespace

#endif
