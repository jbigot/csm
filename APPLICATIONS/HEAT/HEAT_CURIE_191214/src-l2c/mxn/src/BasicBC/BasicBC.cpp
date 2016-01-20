#include <climits>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

#include <llcmcpp/llcmcpp.h>

#include "BasicBCCommon.h"

#define SRC_TOPOLOGY 1
#define DST_TOPOLOGY 2
#define ELEMENT_SIZE 4
#define TOTAL_SIZE   8
#define DISTRIBUTION_TYPE 16
#define NODE_RANK 32

#undef DEBUG_INTERNAL
#undef DEBUG_INTERNAL2

namespace mxn
{

namespace impl
{

  //////////////////////////////////////////////////////////////////////
  // Compute the owner of a bloc accoring a cyclic distribution
  static inline unsigned OwnerBloc(const unsigned bid, const unsigned nbprocs) {
	 return bid % nbprocs;
  }

  //////////////////////////////////////////////////////////////////////
  // Compute the id of a proc owning an element 
  inline static unsigned getProcRangeInf(unsigned low, unsigned bsz) {
	 return low / bsz;      // first remote node
  }

  // Compue the id of the proc owning the previous element
  inline static unsigned getProcRangeSup(unsigned high, unsigned bsz) {
	 return (high-1) / bsz; //  last remote node
  }

  //////////////////////////////////////////////////////////////////////

  // Compute the number of bloc a given processor owns accoring to a cyclic distribution
  static inline unsigned NumberOfBlocProc(const unsigned glen, const unsigned nbprocs,
														const unsigned bsz, const unsigned rank) {
	 unsigned nbbloc = (glen + bsz - 1 ) / bsz;
	 return (nbbloc / nbprocs) + (rank < (nbbloc % nbprocs ) ) ;
  }

  //////////////////////////////////////////////////////////////////////

  // Compute the number of element of a given bloc (local bloc pos)
  // on a proc of given rank according to a cyclic distribution
  static inline unsigned BlocNumberOfElementProc(const unsigned glen, const unsigned rank,
																 const unsigned nbprocs, const unsigned bsz,
																 const unsigned pos) {
	 
	 unsigned long low = ( pos * nbprocs + rank ) * bsz;
	 unsigned long end = low + bsz;
	 unsigned long high = (glen < end ? glen : end);
	 
	 return ( high - low );
  }

  //////////////////////////////////////////////////////////////////////
  // compute the global low bounds of local bloc number pos
  static inline unsigned computeBlocBoundInf(const unsigned bsz, const unsigned rank,
															const unsigned nbprocs, const unsigned pos) {
	 return  ( pos * nbprocs + rank) * bsz;
  }

  //////////////////////////////////////////////////////////////////////
  // Compute the global bounds (low & high) of a given bloc (local bloc pos)
  // on a proc of given rank according to a cyclic distribution
  static inline void computeBlocBounds(unsigned long *low, unsigned long *high,
													const unsigned glen, const unsigned rank,
													const unsigned nbprocs, const unsigned bsz,
													const unsigned pos) {
	 unsigned long start = (pos * nbprocs + rank) * bsz;
	 unsigned long end = start + bsz;
	 *low =  start;
	 *high = ((glen <= end)?glen:end); // min
  }

  //////////////////////////////////////////////////////////////////////
  // Compute the number of element own by the processor of rank rank
  static inline unsigned TotalNumberOfElementProc(const unsigned glen, const unsigned rank,
																  const unsigned nbprocs, const unsigned bsz) {

	 unsigned nbblocfull = NumberOfBlocProc(glen, nbprocs, bsz, rank) - 1;
	 // Is the last full ?
	 unsigned long low = ( nbblocfull * nbprocs + rank ) * bsz;
	 unsigned long end = low + bsz;
	 unsigned long high = (glen < end ? glen : end);
	 
	 return nbblocfull * bsz + ( high - low );
  }

  //////////////////////////////////////////////////////////////////////
  // Compute the size in octet of a bloc according to the given parameter
  // 
  static inline unsigned blocSize(const unsigned glen, const unsigned nbprocs,
											 const BasicBC_param_t& param) {
	 switch(param.type)
		{
		case BASICBC_BLOC:
		  {
			 unsigned nbbloc = (glen + param.unitsize - 1) / param.unitsize;
			 return ((nbbloc + nbprocs - 1 ) / nbprocs) * param.unitsize;
		  }
		case BASICBC_CYCLIC: return param.unitsize; break;
		case BASICBC_BLOCCYCLIC: return param.blocsize*param.unitsize; break;
		}
	 return 0;
  }

  //////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////

  BasicBCCommon::BasicBCCommon() : _dirty(false), _sbsz(0), _dbsz(0), _gfirst(ULONG_MAX), _glast(0)
  {
	 // Common
	 _config = 0;

	 // By default 1->1 :)
	 _sTopo.total=1;
	 _dTopo.total=1;
	 _param.type = BASICBC_BLOC;
	 _nodeRank=0;
	 _llen=0;
  }

  BasicBCCommon::~BasicBCCommon() {}
  
  BasicBC_Client::BasicBC_Client() : _cur_id(-1), _infolists(NULL), _clientBuffer(NULL)
  {
	 // Client side
	 _clientBuffer=NULL;
	 _descr_to_be_cleaned=true;

#ifdef MEMORY
	 this->internalSetComId(-1); // set anonymous & init iterator, infolists & _cur_id
#endif
  }

  BasicBC_Client::~BasicBC_Client() 
  {
#ifdef MEMORY
	 for(info_list_map_t::iterator it=_info_list_map.begin(); it!=_info_list_map.end(); ++it) {
#ifdef DEBUG_INTERNAL
	 std::cerr << "** Destructor: feeing info list map entry with id " << it->first << std::endl;
#endif
	 this->internalFreeComId(it);
	 }
	 _info_list_map.clear();
#endif
  }

  BasicBC_Server::BasicBC_Server() : _serverBuffer(NULL)
  {
	 // Server side
	 _serverDescr=NULL;
	 _serverBuffer=NULL;
  }

  BasicBC_Server::~BasicBC_Server() {}


  /////////////////////////////


  void
  BasicBCCommon::setSourceTopology(const Topology_t& topo)
  {
#ifdef DEBUG_INTERNAL
	 std::cerr << "-- setSourceTopology: " << topo.total << std::endl;
#endif
	 _config |= SRC_TOPOLOGY;
	 _sTopo = topo;
  }

  Topology_t
  BasicBCCommon::getSourceTopology()
  {
	 std::cerr << "****** Why is the method " << __FUNCTION__ << "called?\n";
	 abort();
	 return _sTopo;
  }


  void
  BasicBCCommon::setDestTopology(const Topology_t& topo)
  {
#ifdef DEBUG_INTERNAL
	 std::cerr << "-- setDestTopology: " << topo.total << std::endl;
#endif
	 _config |= DST_TOPOLOGY;
	 _dTopo = topo;
	 _dirty = true;
  }

  Topology_t
  BasicBCCommon::getDestTopology()
  {
	 std::cerr << "****** Why is the method " << __FUNCTION__ << "called?\n";
	 abort();
	 return _dTopo;
  }


#ifdef MEMORY
  void
  BasicBC_Client::setDestTopology(Topology_t topo)
  {
	 this->BasicBCCommon::setDestTopology(topo);

#ifdef DEBUG_INTERNAL
	 std::cerr << "-- setDestTopology(2): " << topo.total << std::endl;
#endif

	 if ( _dTopo.total == topo.total ) return ; // Nothing change !!!!

	 _dirty = true;

	 // Delete old entries wrt to old state
	 info_list_map_t::iterator it = _info_list_map.find(_cur_id);
	 if (it!=_info_list_map.end()) {
		this->internalFreeComId(it);
		_info_list_map.erase(it);
		_cur_id=-2; // noone
		_infolists=NULL;
	 }

	 // Set new entries
	 this->internalSetComId(_cur_id);

	 // BUG: MUST ALSO DELETE ALL ENTRIES IN _info_list_map 
  }
#endif


  void
  BasicBCCommon::setNodeRank(long Rank)
  {
#ifdef DEBUG_INTERNAL
	 std::cerr << "-- setNodeRank: " << Rank << std::endl;
#endif
	 _nodeRank = Rank;
	 _config |= NODE_RANK;
	 _dirty = true;
  }

  long
  BasicBCCommon::getNodeRank()
  {
	 return _nodeRank;
  }

  void
  BasicBCCommon::setBlocSize(unsigned long size)
  {
#ifdef DEBUG_INTERNAL
	 std::cerr << "-- setBlocSize: " << size << std::endl;
#endif
	 _config |= DISTRIBUTION_TYPE;

	 if (size==_param.blocsize) return; // nothing changed

	 _param.blocsize = size;

	 switch(size) {
	 case 0:
#ifdef DEBUG_INTERNAL
		std::cout << "Selecting a BLOC distribution\n";
#endif
		_param.type = BASICBC_BLOC;
		break;
	 case 1:
#ifdef DEBUG_INTERNAL
		std::cout << "Selecting a CYCLIC distribution\n";
#endif
		_param.type = BASICBC_CYCLIC;
		break;
	 default:
#ifdef DEBUG_INTERNAL
		std::cout << "Selecting a BLOCCYCLIC with bsz= " << size << " \n";
#endif
		_param.type = BASICBC_BLOCCYCLIC;
	 }
	 _dirty = true;
  }

  unsigned long
  BasicBCCommon::getBlocSize() {
	 if (_config & DISTRIBUTION_TYPE) {
		return _param.blocsize;
	 } else {
		std::cerr << "[BasicBC] BlockSize has not been set yet\n";
		abort();
	 }
  }

  void
  BasicBCCommon::setEltSize(unsigned long size)
  {
#ifdef DEBUG_INTERNAL
	 std::cerr << "-- setEltSize: " << size << std::endl;
#endif
	 _config |= ELEMENT_SIZE;

	 if (_param.unitsize == size) return ; // nothing changed

	 _param.unitsize = size;
	 _dirty = true;
  }
  
  void
  BasicBCCommon::setTotalNbElt(unsigned long elt_nb)
  {
#ifdef DEBUG_INTERNAL
	 std::cerr << "-- setTotalNbElt: " << elt_nb << std::endl;
#endif
	 _config |= TOTAL_SIZE;

	 if (_glen == elt_nb) return ; // nothing changed

	 _glen = elt_nb;
	 _dirty = true;
  }

  unsigned long
  BasicBCCommon::getTotalNbElt() {
	 if ((_config & TOTAL_SIZE) == TOTAL_SIZE) {
		return _glen;
	 } else {
		  std::cerr << "[BasicBC] getTotalNbElt invoked when info was not set (config="<<_config<<")\n";
		  abort();
	 }
  }

  unsigned long
  BasicBCCommon::getLocalNbElt() {
	 if ((_llen==0)||(_dirty)) {
		const unsigned OK = TOTAL_SIZE | ELEMENT_SIZE | DISTRIBUTION_TYPE | SRC_TOPOLOGY | DST_TOPOLOGY | NODE_RANK; // Assuming sequential ???
		if ((_config & OK) != OK) {
		  std::cerr << "[BasicBC] getLocalNbElt invoked when config was not finished (config="<<_config<<")\n";
		  abort();
		}
		_sbsz = blocSize(_glen*_param.unitsize, _sTopo.total, _param);
		_llen = TotalNumberOfElementProc(_glen*_param.unitsize, _nodeRank, _sTopo.total, _sbsz);
		_gfirst = ULONG_MAX;
		_glast = 0;
		_dirty = false;
	 }
	 return _llen/_param.unitsize;
  }
  
  unsigned long
  BasicBC_Client::getFirst() {
	 //	 std::cerr << "s:"<<_sbsz<<" u:"<<_param.unitsize<<std::endl;
	 if (_gfirst == ULONG_MAX)
		_gfirst=(_nodeRank*_sbsz)/_param.unitsize;
	 return _gfirst;
  }

  unsigned long
  BasicBC_Server::getFirst() {
	 if (_gfirst == ULONG_MAX)
		_gfirst=(_nodeRank*_dbsz)/_param.unitsize;
	 return _gfirst;
  }

  unsigned long
  BasicBC_Client::getLast() {
	 if (_glast == 0) {
		unsigned bz = _sbsz/_param.unitsize;
		unsigned long base = _sTopo.total*bz;
		unsigned long mul = _glen / base;
		//		std::cerr << "gl:"<<_glen<<" b:"<<base<<" mul:"<<mul<<std::endl;
		_glast=(base*mul) + ((_nodeRank+1)*bz);
		if (_glast > _glen)
		  _glast=_glen;
	 }
	 return _glast;
  }

  unsigned long
  BasicBC_Server::getLast() {
	 if (_glast == 0) {
		unsigned bz = _dbsz/_param.unitsize;
		unsigned long base = _dTopo.total*bz;
		unsigned long mul = _glen / base;
		//		std::cerr << "gl:"<<_glen<<" b:"<<base<<" mul:"<<mul<<std::endl;
		//		std::cerr << "r:"<<_nodeRank<<std::endl;
		_glast=(base*mul) + ((_nodeRank+1)*bz);
		if (_glast > _glen)
		  _glast=_glen;
	 }
	 return _glast;
  }

  unsigned long
  BasicBC_Client::getStep() {
	 std::cerr << "sbsz: "<<_sbsz<<std::endl;
	 if (_sbsz==0)
		_sbsz = blocSize(_glen*_param.unitsize, _sTopo.total, _param);
	 return _sbsz/_param.unitsize;
  }

  unsigned long
  BasicBC_Server::getStep() {
	 return _dbsz/_param.unitsize;
  }


  PieceToSend* 
  BasicBC_Client::computePiecesToSend(unsigned& size_out)
  {
	 /*
	  * store src/dst/sz/start
	  *
	  * where start is a local offset to apply to the vector
	  *
	  */

	 // Check Config
	 const unsigned OK = TOTAL_SIZE | ELEMENT_SIZE | DISTRIBUTION_TYPE | SRC_TOPOLOGY | DST_TOPOLOGY | NODE_RANK ; // Assuming sequential => no !!
	 if ((_config & OK) != OK) {
		std::cerr << "[BasicBC] computePiecestoSend invoked when config was not finished (config="<<_config<<")\n";
	 }
	 // const unsigned OK2 = OK | SRC_TOPOLOGY|DST_TOPOLOGY;
	 // if ((_config & OK2) != OK2) {
	 // 	std::cerr << "[BasicBC] ** warning src and/or dst topology not initialized -- assuming 1<->1 communication\n";
	 // }

	 // INIT ALL DATA
	 _clientDescr.bsz  = _param.blocsize;
	 _clientDescr.glen = _glen;

	 _infolists=new info_vector_t(_dTopo.total);
	 for(unsigned i=0;i<_dTopo.total;i++)
		(*_infolists)[i]=new info_list_t();


	 // END INIT ALL DATA
	 
	 PieceToSend * sched;
	 unsigned long golen = _glen*_param.unitsize;

	 // We need to compute _lstart and _llen
	 _sbsz = blocSize( golen, _sTopo.total, _param);
	 _lstart = _nodeRank*_sbsz;
	 _lstart = (_lstart < golen ? _lstart:golen);
	 _llen   = TotalNumberOfElementProc(golen, _nodeRank, _sTopo.total, _sbsz);

#ifdef DEBUG_INTERNAL
	 //  std::cerr << "In computePiecesToSend-------------------- all in octet !\n";
	 fprintf(stderr, "-- stopo: %d\tdtopo: %d\n", _sTopo.total, _dTopo.total);
	 fprintf(stderr, "-- golen: %ld\ttype: %d\tsd.start %ld\tllen: %d\n", golen,
				_param.type, _lstart, _llen);
#endif

	 if (_sTopo.total == _dTopo.total) {
#ifdef DEBUG_INTERNAL
		fprintf(stderr, "-- easy: n<->n case !\n");
#endif
		size_out = 1;
		sched = new PieceToSend[1];
		sched[0].sourceNode = _nodeRank; // my self :)
		sched[0].destNode   = _nodeRank; // the corresponding node
		sched[0].size       = _llen;     // msg size
    
		info_t* inf = new info_t();
    
		inf->gstart         = _lstart/_param.unitsize; // used start as id -> same at server side
		inf->lstart         = 0;       // do not apply any offset to local ptr
		inf->msg_size       = _llen; 
		inf->sent_size      = 0;

		// add inf to the list
		(*_infolists)[_nodeRank]->push_back(inf);
		// sched points to the list
		sched[0].id         = (void*) (*_infolists)[_nodeRank];
		
#ifdef DEBUG_INTERNAL
		fprintf(stderr, "--  %2d -> %2d : gstart:%2u lstart:%2u len:%2d\n",
				  sched[0].sourceNode, sched[0].destNode, inf->gstart, inf->lstart, inf->msg_size);
#endif
	 } else {        
		if (_param.type == BASICBC_BLOC ) {
		  // that's a standard bloc redistribution
#ifdef DEBUG_INTERNAL
		  fprintf(stderr, "-- medium: n<->m bloc redistribution case !\n");
#endif

		  unsigned long slow;
		  unsigned long shigh;
		  computeBlocBounds(&slow, &shigh, golen, _nodeRank, _sTopo.total, _sbsz, 0);
    
#ifdef DEBUG_INTERNAL
		  std::cerr << "--  sbsz: "<<_sbsz<<" sbounds: "<<slow<<" - "<<shigh<<std::endl;
#endif

		  unsigned dbsz = blocSize(golen, _dTopo.total, _param);
		  
#ifdef DEBUG_INTERNAL
		  std::cerr << "--  dbsz: "<<dbsz<<std::endl;
#endif

		  unsigned fpid, lpid;
		  fpid = getProcRangeInf(slow,  dbsz);
		  lpid = getProcRangeSup(shigh, dbsz);
    
#ifdef DEBUG_INTERNAL
		  fprintf(stderr, "--  loop from %d to %d width dtotal: %d\n", fpid, lpid, _dTopo.total);
#endif
    
		  // for each dest bloc
		  size_out = lpid-fpid+1;
		  sched = new PieceToSend[size_out];
		  for(unsigned i=fpid; i <= lpid; i++) {
			 PieceToSend& s   =  sched[i-fpid];	
			 s.sourceNode     = _nodeRank;
			 s.destNode       = i;
			 
			 info_t* inf      = new info_t();

			 unsigned tmp     = i*dbsz;
			 unsigned tmp2;
			 tmp2 = ( slow >= tmp)?slow:tmp; // max : global offset in octet as ID
			 inf->gstart      = tmp2 / _param.unitsize;
			 inf->lstart      = tmp2 - _lstart;

			 tmp += dbsz;
			 unsigned end     = ( shigh <= tmp)?shigh:tmp; // min

			 inf->msg_size  =  s.size  = end - tmp2;
			 inf->sent_size = 0;

			 // add inf to the list
			 (*_infolists)[i-fpid]->push_back(inf);
			 // sched points to the list
			 s.id             = (void*) (*_infolists)[i-fpid];

#ifdef DEBUG_INTERNAL
			 fprintf(stderr, "--  %2d -> %2d : gstart:%2u lstart:%2u len:%2d\n",
						s.sourceNode, s.destNode, inf->gstart, inf->lstart, inf->msg_size);
#endif
		  }
		} else {
		  // it is a bloccyclic distribution	
#ifdef DEBUG_INTERNAL
		  fprintf(stderr, "-- hard: n<->m bloccyclic redistribution case !\n");
#endif
		  unsigned stbsz  = _sbsz * _sTopo.total;
		  unsigned nbbloc = NumberOfBlocProc(golen, _sTopo.total, _sbsz, _nodeRank);

#ifdef DEBUG_INTERNAL
		  std::cerr << "--  sbsz: "<<_sbsz<<" nbbloc: "<<nbbloc<<std::endl;
#endif

		  // for each src bloc, find a dst node
		  // But only one struct for each dst -> pre-count

		  // should be equal to the number of real dest !
		  size_out = 0;
		  unsigned *tmp_dnb    = (unsigned*) calloc(_dTopo.total, sizeof(unsigned)); // zeroed array
		  PieceToSend **tmp_dor = (PieceToSend**) calloc(_dTopo.total, sizeof(PieceToSend*)); // zeroed array
		  for(unsigned b=0; b<nbbloc; b++) {
			 unsigned gb            = b * _sTopo.total + _nodeRank; // global bloc id
			 unsigned drank         = OwnerBloc(gb, _dTopo.total);
			 if (tmp_dnb[drank]++ == 0) // detect if it is the 1st time
				size_out++;
		  }
		  // Allocating output
		  sched = new PieceToSend[size_out];
		  unsigned cur=0;
		  for(unsigned i=0; i<_dTopo.total; i++) {
			 sched[cur].sourceNode = _nodeRank;
			 sched[cur].destNode   = i;
			 sched[cur].size       = 0;
			 sched[cur].id         = (void*) (*_infolists)[i]; 
			 tmp_dor[i]            = &sched[cur];   // an index if valid only if tmp_dnb[index]>0 !!!!
			 cur+= (tmp_dnb[i]>0?1:0); // increment order only if this dest is not zero
		  }
	
#ifdef DEBUG_INTERNAL
		  std::cerr << "-- size_out: "<<size_out<<std::endl;
#endif

		  for(unsigned b=0; b<nbbloc; b++) {
			 unsigned gb    = b * _sTopo.total + _nodeRank; // global bloc id
			 unsigned drank = OwnerBloc(gb, _dTopo.total);
			 
			 PieceToSend* s = tmp_dor[drank];

			 // Allocate new element
			 info_t* inf    = new info_t();

			 inf->gstart    = ((stbsz*b) + (_nodeRank*_sbsz))/ _param.unitsize;
			 inf->lstart    = b * _sbsz;
			 inf->msg_size  = BlocNumberOfElementProc(golen, _nodeRank, _sTopo.total, _sbsz, b);
			 inf->sent_size = 0;

			 // Accumulate msg length in PieceToSend
			 s->size           += inf->msg_size;
	
			 // insert it in the list
			 info_list_t* ilsp  = (info_list_t*) s->id; // pointer to a list
			 ilsp->push_back(inf);

	
#ifdef DEBUG_INTERNAL
			 fprintf(stderr, "--  %2d -> %2d : gstart:%2u lstart:%2u len:%2d\n",
						s->sourceNode, s->destNode, inf->gstart, inf->lstart, inf->msg_size);
#endif
		  }
		}
	 }
	 
#ifdef DEBUG_INTERNAL
	 //  std::cerr << "computePiecesToSend-------------------- done\n";
#endif

	 return sched;
  }

  void
  BasicBC_Client::setDataPtr(void* dataPtr)
  {
	 _clientBuffer = (char *) dataPtr;
#ifdef DEBUG_INTERNAL
	 std::cerr << "-- _clientBuffer set to "<<dataPtr<<std::endl;
#endif
  }

  void *
  BasicBC_Client::getClientData(void *pid, unsigned dnode, unsigned long & remaining_size_octet, unsigned long & returned_length_element, bool & end)  
  {
	 info_list_t* ilsp  = (info_list_t*) pid;

#ifdef DEBUG_INTERNAL2
	 std::cerr << "-- getClientData : _clientBuffer=" << (void*) _clientBuffer << std::endl;
	 std::cerr << "-- getClientData : " << dnode << " " << remaining_size_octet << std::endl;
#endif

	 if (_descr_to_be_cleaned) {
		_descr_to_be_cleaned = false;
		_clientDescr.ids.clear();
	 }

	 // look for 1st one not "empty"
	 unsigned long rst_data=0;
	 info_list_t::const_iterator ci;
	 for(ci=ilsp->begin(); ci!=ilsp->end(); ++ci) {
		rst_data = (*ci)->msg_size - (*ci)->sent_size;
		if (rst_data>0)
		  break;
	 }

	 // No more element: return immediatly
	 if (ci == ilsp->end())
		{
#ifdef DEBUG_INTERNAL2
		  std::cerr << "-- getClientData0 : no more block for this argument\n";
#endif
		  returned_length_element = 0;
		  end = true;
		  return NULL;
		}
	 // No more data of this element but there are more
	 if (rst_data == 0)
		{
#ifdef DEBUG_INTERNAL2
		  std::cerr << "-- getClientData0 : no more data for this bloc\n";
#endif
		  returned_length_element = 0;
		  end = false;
		  return NULL;
		}
	 // *ci assumed ok here (else return above !!)
	 info_t* info = *ci;

	 // More asked data as I can provide -> give all the data of this bloc
	 if (remaining_size_octet >= rst_data) {
		unsigned descr_len       = _clientDescr.ids.size();
		_clientDescr.ids.resize(descr_len+1);
		_clientDescr.ids[descr_len].gstart = info->gstart+(info->sent_size/_param.unitsize);
		
		unsigned start           = info->lstart+info->sent_size;
		info->sent_size         += rst_data;
		returned_length_element  = rst_data / _param.unitsize; // should divide;
		remaining_size_octet    -= rst_data;
		end                      = (remaining_size_octet == 0);

		_clientDescr.ids[descr_len].len    = returned_length_element;

#ifdef DEBUG_INTERNAL2
		std::cerr << "-- getClientData : gstart:"<<_clientDescr.ids[descr_len].gstart<<" #element:"<<returned_length_element<<std::endl;
#endif

		return (void *)&_clientBuffer[start];
	 } else  {
      // I have more data than requested
      unsigned descr_len       = _clientDescr.ids.size();
      _clientDescr.ids.resize(descr_len+1);
      _clientDescr.ids[descr_len].gstart    = info->gstart+(info->sent_size/_param.unitsize);

      unsigned start           = info->lstart+info->sent_size;
      returned_length_element  = (remaining_size_octet+_param.unitsize-1)/_param.unitsize; // max
      unsigned mlen            = returned_length_element*_param.unitsize;
      info->sent_size         += mlen;
      remaining_size_octet    -= mlen;
      end                      = true;

      _clientDescr.ids[descr_len].gstart    = returned_length_element;

#ifdef DEBUG_INTERNAL2
      std::cerr << "-- getClientData1 : gstart:"<<_clientDescr.ids[descr_len].gstart<<" #element:"<<returned_length_element<<std::endl;
#endif

      return (void *)&_clientBuffer[start];
    }
  }

  void
  BasicBC_Client::clientFree() // always called at the end of an invocation (reset some variables)
  {

#ifdef DEBUG_INTERNAL
	 std::cerr << "-- clientFree\n";
#endif
#ifdef MEMORY
	 if (_cur_id == -1) {
		this->clear_info_list(*_infolists);
	 } else {
		this->reset_info_list(*_infolists);
	 }
#endif
  }

  void*
  BasicBC_Client::getDescr()
  {
#ifdef DEBUG_INTERNAL
	 std::cerr << "-- descr: glen: "<<_clientDescr.glen<<" #element: "<<_clientDescr.ids.size()<<" bsz: "<<_clientDescr.bsz;
	 for(unsigned i=0;i<_clientDescr.ids.size();i++) {
		std::cerr<<" ["<<_clientDescr.ids[i].gstart<<":"<<_clientDescr.ids[i].len<<"]";
	 }
	 std::cerr<<std::endl;
#endif
	 _descr_to_be_cleaned = true;
	 return &_clientDescr;
  }

  bool
  BasicBC_Server::insertData(void* rcptBuf, unsigned long element_nb) // can be 0 !!!
  {
	 // We are receiving one bloc
	 unsigned gstart = _serverDescr->ids[_descr_ids_counter].gstart; // in octet !!!
	 unsigned lstart;

	 unsigned llen;
	 if (element_nb==0)
		llen = _serverDescr->ids[_descr_ids_counter].len;
	 else
		llen = element_nb*_param.unitsize;
	 _descr_ids_counter++;

	 // Need to compute lstart :(
	 if (_param.type == BASICBC_BLOC ) {
		lstart = (gstart*_param.unitsize)/ _dbsz; // bloc number
		lstart = (gstart*_param.unitsize) - (lstart*_dbsz);
	 } else {
		lstart = (gstart*_param.unitsize)/ _dbsz; // bloc number
		lstart /= _dTopo.total;                   // local number
	 }

	 char*    rptr   = &_serverBuffer[lstart];
#ifdef DEBUG_INTERNAL
	 std::cerr << "  gstart: "<< gstart<<" lstart: "<<lstart<<" #element " << element_nb<<" llen: "<<llen<<std::endl;
#endif
	 memcpy(rptr, rcptBuf, llen);


#ifdef DEBUG_INTERNAL
	 if (llen > _serverToReceived) {
		std::cerr << " #missing data: "<<_serverToReceived<<std::endl;
		std::cerr << " llen: "<<llen<<std::endl;
		abort();
	 }
#endif

	 _serverToReceived -= llen;
#ifdef DEBUG_INTERNAL
	 std::cerr << " #missing data: "<<_serverToReceived<<std::endl;
	 std::cerr << "data rcv : " << llen << std::endl;
#endif
	 return (_serverToReceived == 0);
  }

  void *
  BasicBC_Server::getServerData(unsigned long & length) // in element !
  {
	 length= _llen / _param.unitsize;
#ifdef DEBUG_INTERNAL
	 std::cerr << "-- getServerData: ptr : "<<(void*)_serverBuffer<<" #element:"<<length<<std::endl;
#endif  
	 return (void*) _serverBuffer;
  }

  void
  BasicBC_Server::setDescr(void * descr)
  {
	 _serverDescr = (BasicBCDescr*) descr;
	 _descr_ids_counter=0;

	 // Descr already received ??
	 if ( _serverBuffer == NULL) {
      // 1st time -> allocate memory & init
      //this->setEltSize(_descr->usz); BUG: usz can be different in client and server (32bit vs 64bit)
      this->setTotalNbElt(_serverDescr->glen);
      this->setBlocSize(_serverDescr->bsz);

		const unsigned OK = TOTAL_SIZE | ELEMENT_SIZE | DISTRIBUTION_TYPE | SRC_TOPOLOGY|DST_TOPOLOGY | NODE_RANK; // Assuming sequential => NO
		if ((_config & OK) != OK) {
		  std::cerr << "[BasicBC] setDescr invoked when config was not finished (config="<<_config<<")\n";
		  abort();
		}
		// const unsigned OK2 = OK | SRC_TOPOLOGY|DST_TOPOLOGY;
		// if ((_config & OK2) != OK2) {
		//   std::cerr << "[BasicBC] ** warning src and/or dst topology not initialized in setDescr -- assuming 1<->1 communication\n";
		// }

      unsigned long golen = _serverDescr->glen*_param.unitsize;
      _dbsz = blocSize( golen, _dTopo.total, _param);
      _llen  = TotalNumberOfElementProc(golen, _nodeRank, _dTopo.total, _dbsz);
      
#ifdef DEBUG_INTERNAL
      fprintf(stderr, "-- setDescr: stopo: %d\tdtopo: %d\n",_sTopo.total, _dTopo.total);
      fprintf(stderr, "   setDescr: golen: %ld\tllen %d\tbsz:%ld\n", _serverDescr->glen, _llen, _serverDescr->bsz);
#endif  
      _serverBuffer = (char*) malloc(_llen);
#ifdef DEBUG_INTERNAL
      std::cerr << "-- serverMalloc: "<<(void*)_serverBuffer<<" - "<<_llen<<std::endl;
      std::cerr << "-- missing data: "<<_llen<<std::endl;
#endif
      _serverToReceived = _llen;
    } else {
#ifdef DEBUG_INTERNAL
      std::cerr << "-- Contribution on an already initiated communication\n";
#endif
    }
  }
  
  void
  BasicBC_Server::serverFree() 
  {
#ifdef DEBUG_INTERNAL
	 std::cerr << "-- serverFree: "<<(void*)_serverBuffer<<std::endl;
#endif
	 free(_serverBuffer);
	 _serverBuffer=NULL; // the orb (or the user) need to free the memory!
  }

#ifdef MEMORY
  void
  BasicBC_Impl::internalSetComId(long id) // called by constructor
  { 
	 info_list_map_t::iterator it = _info_list_map.find(id);

	 if (it == _info_list_map.end() ) {
#ifdef DEBUG_INTERNAL
		std::cerr << "setComiInternal: set 1st time communication id "<<id<<std::endl;
#endif
		// we must allocate a new one
		_infolists=this->allocate_new_info_list();
#ifdef DEBUG_INTERNAL
		std::cerr << "setComIdInternal: allocating a new _infolists: "<< _infolists<<std::endl;
#endif
		_info_list_map[id] = _infolists;
	 } else {
#ifdef DEBUG_INTERNAL
		std::cerr << "setComIdInternal set (not 1st time) communication id "<<id<<std::endl;
#endif
		_infolists   = it->second;
	 }
	 _cur_id = id;
  }

  bool
  BasicBC_Impl::setComId(long id)
  { 
	 if (_cur_id == id) return true; // already initialized
	 this->internalSetComId(id);
	 return true;
  }


  void
  BasicBC_Impl::internalFreeComId(info_list_map_t::iterator& it)
  {
	 if (it == _info_list_map.end() ) {
#ifdef DEBUG_INTERNAL
		std::cerr << "freeComid: communication id  not found (ignoring)\n";
#endif
	 } else {
#ifdef DEBUG_INTERNAL
		std::cerr << "freeComid: freeing communication "<<it->second<<std::endl;
#endif
		info_vector_t& info = *it->second;
		// for each destNode
		for(unsigned i=0; i< _dTopo.total; i++) {
		  // for each element of the list
		  info_list_t *ilp = info[i];
		  for(info_list_t::const_iterator ci=ilp->begin(); ci!=ilp->end(); ++ci)
			 delete (*ci); // elements of the list
		  info[i] -> clear();
		  delete info[i]; // a list
		}
		delete it->second;
	 }
  }

  bool
  BasicBC_Impl::freeComId(long id)
  { 
	 if (id == -1) {
#ifdef DEBUG_INTERNAL
		std::cerr << "freeComId: not allowed to free com id -1 (ignoring)\n";
#endif
		return true;
	 }
	 
	 if (_cur_id == id) {
#ifdef DEBUG_INTERNAL
		std::cerr << "freeComId: not allowed to free current com (ignoring)\n";
#endif
		return true;
	 }

#ifdef DEBUG_INTERNAL
	 std::cerr << "freeComid: communication id: "<<id<<std::endl;
#endif

	 info_list_map_t::iterator it = _info_list_map.find(id);
	 this->internalFreeComId(it);
	 _info_list_map.erase(it);

	 return true;
  }


  mxn::BasicBC_Impl::info_vector_t*
  BasicBC_Impl::allocate_new_info_list()
  {
#ifdef DEBUG_INTERNAL
	 std::cerr << "allocate new info list to dTopo size "<<_dTopo.total<<std::endl;
	 if (_infolists!=NULL) {
		std::cerr << "** WARNING _ infolist is not empty!!!\n";
	 }
#endif
	 info_vector_t*  iv =  new info_vector_t(_dTopo.total);
	 for(unsigned i=0;i<_dTopo.total;i++)
		(*iv)[i]=new info_list_t();
	 return iv;
  }
  
  void
  BasicBC_Impl::reset_info_list(mxn::BasicBC_Impl::info_vector_t& iv)
  {
#ifdef DEBUG_INTERNAL
	 std::cerr << "-- reset info list\n";
#endif
	 // for each destNode
	 for(unsigned i=0; i< _dTopo.total; i++) {
		// for each element of the list
		info_list_t *ilp = iv[i];
		for(info_list_t::const_iterator ci=ilp->begin(); ci!=ilp->end(); ++ci)
		  (*ci)->sent_size = 0;
	 }
  }

  void
  BasicBC_Impl::clear_info_list(mxn::BasicBC_Impl::info_vector_t& iv)
  {
#ifdef DEBUG_INTERNAL
	 std::cerr << "-- clear info list\n";
#endif
	 // for each destNode
	 for(unsigned i=0; i< _dTopo.total; i++) {
		// for each element of the list
		info_list_t *ilp = iv[i];
		for(info_list_t::const_iterator ci=ilp->begin(); ci!=ilp->end(); ++ci)
		  delete (*ci);
		iv[i] -> clear();
	 }
  }
#endif


////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

  class llcm_BasicBC_Client : public llcmcpp::Component {
  private:
	 BasicBC_Client co;
  public:
	 llcm_BasicBC_Client() {}
	 ~llcm_BasicBC_Client() {}
	 
	 void* get(const char* pname) {
		if (strcmp(pname, "bcc")==0) {
		  return static_cast<BasicClientConfig*>(&co);
		} else if (strcmp(pname, "dlc")==0) {
		  return static_cast<mxn::DistributionLibraryClient*>(&co);
		} else if (strcmp(pname, "bc")==0) {
			//TODO!!!! this is not an interface !!!
		  return static_cast<mxn::impl::BasicBC*>(&co);
		} else {
		  std::cerr << "[llcm_BasicBC_Client] get : unknown port "<<pname<<" (returning NULL)\n";
		  return 0;
		}
	 }
	 void set(const char* pname, void* value) {
		if (strcmp(pname, "srcTopo")==0) { 
		  Topology_t stopo;
		  stopo.total=(unsigned long) value;
		  co.setSourceTopology(stopo);
		} else if (strcmp(pname, "dstTopo")==0) { 
		  Topology_t dtopo;
		  dtopo.total=(unsigned long) value;
		  co.setDestTopology(dtopo);
		} else {
		  std::cerr << "[llcm_BasicBC_Server] set : unknown port "<<pname<<std::endl;
		}
	 }
  };

  LLCMCPP_CREATE(llcm_BasicBC_Client)

  class llcm_BasicBC_Server : public llcmcpp::Component {
  private:
	 BasicBC_Server co;
  public:
	 llcm_BasicBC_Server() {}
	 ~llcm_BasicBC_Server() {}
	 
	 void* get(const char* pname) {
		if (strcmp(pname, "bcs")==0) {
		  return static_cast<BasicBCConfig*>(&co);
		} else if (strcmp(pname, "dls")==0) {
		  return static_cast<mxn::DistributionLibraryServer*>(&co);
		} else if (strcmp(pname, "bc")==0) {
			//TODO  !!! this is not an interface !!!
		  return static_cast<mxn::impl::BasicBC*>(&co);
		} else {
		  std::cerr << "[llcm_BasicBC_Server] get : unknown port "<<pname<<" (returning NULL)\n";
		  return 0;
		}
	 }

	 void set(const char* pname, void* value) {
		if (strcmp(pname, "srcTopo")==0) { 
		  Topology_t stopo;
		  stopo.total=(unsigned long) value;
		  co.setSourceTopology(stopo);
		} else if (strcmp(pname, "dstTopo")==0) { 
		  Topology_t dtopo;
		  dtopo.total=(unsigned long) value;
		  co.setDestTopology(dtopo);
		} else {
		  std::cerr << "[llcm_BasicBC_Server] set : unknown port "<<pname<<std::endl;
		}
	 }
  };

LLCMCPP_CREATE(llcm_BasicBC_Server)

} // namespace

}
