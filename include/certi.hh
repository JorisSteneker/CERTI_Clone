// ----------------------------------------------------------------------------
// CERTI - HLA RunTime Infrastructure
// Copyright (C) 2002-2005  ONERA
//
// This program is free software ; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation ; either version 2 of
// the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY ; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this program ; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
//
// $Id: certi.hh,v 3.29 2008/06/11 15:19:20 rousse Exp $
// ----------------------------------------------------------------------------
#include <vector>

#ifndef CERTI_HH_INCLUDED
#define CERTI_HH_INCLUDED
#if defined(_WIN32)	
    #include <windows.h>
    #include <algorithm>	
    #include <process.h>
    #define	sleep(a)	      Sleep(a * 1000)
    #define	usleep(a)	      Sleep(a / 1000)
    typedef unsigned short        ushort;
    typedef	int	              pid_t;			      
    #define  strcasecmp           strcmp
    #define	STAT_FUNCTION         _stat
    #define	STAT_STRUCT           struct _stat
    /* TCP/IP related typedef and includes */
    #ifndef _WINSOCK2API_
        #ifndef _WINSOCKAPI_
	   #include <winsock2.h>             
	#endif    
    #endif	
    typedef u_long			in_addr_t;
    typedef unsigned short		in_port_t;
    #ifdef _MSC_VER
        typedef unsigned __int64        uint64_t;
        typedef __int64			 int64_t;
        typedef unsigned __int32	uint32_t;
        typedef __int32			 int32_t;
        typedef unsigned __int16	uint16_t;
        typedef __int16			 int16_t;
        typedef unsigned __int8	         uint8_t;
        typedef __int8			  int8_t;
        #ifdef _M_X64 
           #define  CERTI_INT64_CONSTANT(val)  (val##L)
           #define  CERTI_INT64_FORMAT         "l"
        #else
           #define  CERTI_INT64_CONSTANT(val)  (val##LL)
           #define  CERTI_INT64_FORMAT         "ll"
        #endif
        #ifndef __func__
        #define __func__ "<unknown>"
        #endif
    #else
       #include <stdint.h>
       #ifdef __x86_64__
          #define  CERTI_INT64_CONSTANT(val)  (val##L)
          #define  CERTI_INT64_FORMAT         "l"
       #else
          #define  CERTI_INT64_CONSTANT(val)  (val##LL)
          #define  CERTI_INT64_FORMAT         "ll"
       #endif
    #endif
#else
    #include <stdint.h>
    #define  STAT_FUNCTION		stat
    #define  STAT_STRUCT		struct stat
    #ifdef __x86_64__
       #define  CERTI_INT64_CONSTANT(val)  (val##L)
       #define  CERTI_INT64_FORMAT         "l"
    #else
       #define  CERTI_INT64_CONSTANT(val)  (val##LL)
       #define  CERTI_INT64_FORMAT         "ll"
    #endif
#endif

#include "RTI.hh"
	
/**
 * @defgroup libCERTI The CERTI library.
 * @ingroup CERTI_Libraries
 * The CERTI library contains CERTI specific HLA implementation
 */
   
// Deprecated HLA macros still in use
// Will be suppressed asap
#define MAX_BYTES_PER_VALUE 500
//#define MAX_FEDERATE_NAME_LENGTH 64
#undef MAX_USER_TAG_LENGTH
#define MAX_USER_TAG_LENGTH 64
//#undef MAX_ATTRIBUTES_PER_CLASS
//#define MAX_ATTRIBUTES_PER_CLASS 50
#define MAX_BYTES_PER_VALUETYPE 128
//#undef MAX_PARAMETERS_PER_CLASS
//#define MAX_PARAMETERS_PER_CLASS 50

// next used in RTIG::processIncomingMessage method
#define BUFFER_EXCEPTION_REASON_SIZE 256

namespace certi {

// RTI Shortcuts
typedef RTI::AttributeHandle AttributeHandle ;
typedef RTI::AttributeHandleSet AttributeHandleSet ;
typedef RTI::DimensionHandle DimensionHandle ;
typedef RTI::EventRetractionHandle EventRetractionHandle ;
typedef RTI::Exception Exception ;
typedef RTI::ExtentIndex ExtentIndex ;
typedef RTI::FederateHandle FederateHandle ;
typedef RTI::Handle Handle ;
typedef RTI::InteractionClassHandle InteractionClassHandle ;
typedef RTI::ObjectClassHandle ObjectClassHandle ;
typedef RTI::ObjectHandle ObjectHandle ;
typedef RTI::OrderType OrderType ;
typedef RTI::ParameterHandle ParameterHandle ;
typedef RTI::SpaceHandle SpaceHandle ;
typedef RTI::TransportType TransportType ;
typedef RTI::ULong ULong ;
typedef RTI::UShort UShort ;

// RTI Shortcuts (exceptions)
typedef RTI::ArrayIndexOutOfBounds ArrayIndexOutOfBounds ;
typedef RTI::AsynchronousDeliveryAlreadyDisabled AsynchronousDeliveryAlreadyDisabled ;
typedef RTI::AsynchronousDeliveryAlreadyEnabled  AsynchronousDeliveryAlreadyEnabled ;
typedef RTI::AttributeAcquisitionWasNotRequested AttributeAcquisitionWasNotRequested ;
typedef RTI::AttributeAcquisitionWasNotCanceled AttributeAcquisitionWasNotCanceled ;
typedef RTI::AttributeAlreadyBeingAcquired AttributeAlreadyBeingAcquired ;
typedef RTI::AttributeAlreadyBeingDivested AttributeAlreadyBeingDivested ;
typedef RTI::AttributeAlreadyOwned AttributeAlreadyOwned ;
typedef RTI::AttributeDivestitureWasNotRequested AttributeDivestitureWasNotRequested ;
typedef RTI::AttributeNotDefined AttributeNotDefined ;
typedef RTI::AttributeNotKnown AttributeNotKnown ;
typedef RTI::AttributeNotOwned AttributeNotOwned ;
typedef RTI::AttributeNotPublished AttributeNotPublished ;
typedef RTI::ConcurrentAccessAttempted ConcurrentAccessAttempted ;
typedef RTI::CouldNotDiscover CouldNotDiscover ;
typedef RTI::CouldNotOpenFED CouldNotOpenFED ;
typedef RTI::CouldNotRestore CouldNotRestore ;
typedef RTI::DeletePrivilegeNotHeld DeletePrivilegeNotHeld ;
typedef RTI::DimensionNotDefined DimensionNotDefined ;
typedef RTI::EnableTimeConstrainedPending EnableTimeConstrainedPending ;
typedef RTI::EnableTimeConstrainedWasNotPending EnableTimeConstrainedWasNotPending ;
typedef RTI::EnableTimeRegulationPending EnableTimeRegulationPending ;
typedef RTI::EnableTimeRegulationWasNotPending EnableTimeRegulationWasNotPending ;
typedef RTI::ErrorReadingFED ErrorReadingFED ;
typedef RTI::EventNotKnown EventNotKnown ;
typedef RTI::FederateAlreadyExecutionMember FederateAlreadyExecutionMember ;
typedef RTI::FederateInternalError FederateInternalError ;
typedef RTI::FederateLoggingServiceCalls FederateLoggingServiceCalls ;
typedef RTI::FederateNotExecutionMember FederateNotExecutionMember ;
typedef RTI::FederateOwnsAttributes FederateOwnsAttributes ;
typedef RTI::FederateWasNotAskedToReleaseAttribute FederateWasNotAskedToReleaseAttribute ;
typedef RTI::FederatesCurrentlyJoined FederatesCurrentlyJoined ;
typedef RTI::FederationExecutionAlreadyExists FederationExecutionAlreadyExists ;
typedef RTI::FederationExecutionDoesNotExist FederationExecutionDoesNotExist ;
typedef RTI::FederationTimeAlreadyPassed FederationTimeAlreadyPassed ;
typedef RTI::HandleValuePairMaximumExceeded HandleValuePairMaximumExceeded ;
typedef RTI::InteractionClassNotDefined InteractionClassNotDefined ;
typedef RTI::InteractionClassNotKnown InteractionClassNotKnown ;
typedef RTI::InteractionClassNotPublished InteractionClassNotPublished ;
typedef RTI::InteractionClassNotSubscribed InteractionClassNotSubscribed ;
typedef RTI::InteractionParameterNotDefined InteractionParameterNotDefined ;
typedef RTI::InteractionParameterNotKnown InteractionParameterNotKnown ;
typedef RTI::InvalidExtents InvalidExtents ;
typedef RTI::InvalidFederationTime InvalidFederationTime ;
typedef RTI::InvalidHandleValuePairSetContext InvalidHandleValuePairSetContext ;
typedef RTI::InvalidLookahead InvalidLookahead ;
typedef RTI::InvalidOrderingHandle InvalidOrderingHandle ;
typedef RTI::InvalidRegionContext InvalidRegionContext ;
typedef RTI::InvalidResignAction InvalidResignAction ;
typedef RTI::InvalidRetractionHandle InvalidRetractionHandle ;
typedef RTI::InvalidTransportationHandle InvalidTransportationHandle ;
typedef RTI::MemoryExhausted MemoryExhausted ;
typedef RTI::NameNotFound NameNotFound ;
typedef RTI::ObjectClassNotDefined ObjectClassNotDefined ;
typedef RTI::ObjectClassNotKnown ObjectClassNotKnown ;
typedef RTI::ObjectClassNotPublished ObjectClassNotPublished ;
typedef RTI::ObjectClassNotSubscribed ObjectClassNotSubscribed ;
typedef RTI::ObjectNotKnown ObjectNotKnown ;
typedef RTI::ObjectAlreadyRegistered ObjectAlreadyRegistered ;
typedef RTI::OwnershipAcquisitionPending OwnershipAcquisitionPending ;
typedef RTI::RegionInUse RegionInUse ;
typedef RTI::RegionNotKnown RegionNotKnown ;
typedef RTI::RestoreInProgress RestoreInProgress ;
typedef RTI::RestoreNotRequested RestoreNotRequested ;
typedef RTI::RTIinternalError RTIinternalError ;
typedef RTI::SpaceNotDefined SpaceNotDefined ;
typedef RTI::SaveInProgress SaveInProgress ;
typedef RTI::SaveNotInitiated SaveNotInitiated ;
typedef RTI::SpecifiedSaveLabelDoesNotExist SpecifiedSaveLabelDoesNotExist ;
typedef RTI::SynchronizationPointLabelWasNotAnnounced SynchronizationPointLabelWasNotAnnounced ;
typedef RTI::TimeAdvanceAlreadyInProgress TimeAdvanceAlreadyInProgress ;
typedef RTI::TimeAdvanceWasNotInProgress TimeAdvanceWasNotInProgress ;
typedef RTI::TimeConstrainedAlreadyEnabled TimeConstrainedAlreadyEnabled ;
typedef RTI::TimeConstrainedWasNotEnabled TimeConstrainedWasNotEnabled ;
typedef RTI::TimeRegulationAlreadyEnabled TimeRegulationAlreadyEnabled ;
typedef RTI::TimeRegulationWasNotEnabled TimeRegulationWasNotEnabled ;
typedef RTI::UnableToPerformSave UnableToPerformSave ;
typedef RTI::ValueCountExceeded ValueCountExceeded ;
typedef RTI::ValueLengthExceeded ValueLengthExceeded ;

// CERTI Exception-derived Exceptions (including deprecated HLA exceptions)
RTI_EXCEPT(FederateNotPublishing)
RTI_EXCEPT(FederateNotSubscribing)
RTI_EXCEPT(InvalidObjectHandle)
RTI_EXCEPT(InvalidOrderType)
RTI_EXCEPT(InvalidTransportType)
RTI_EXCEPT(SecurityError)
RTI_EXCEPT(CouldNotOpenRID)
RTI_EXCEPT(ErrorReadingRID)
RTI_EXCEPT(FederationAlreadyPaused)
RTI_EXCEPT(FederationNotPaused)
RTI_EXCEPT(AttributeNotSubscribed)
RTI_EXCEPT(FederateAlreadyPaused)
RTI_EXCEPT(FederateDoesNotExist)
RTI_EXCEPT(FederateNameAlreadyInUse)
RTI_EXCEPT(FederateNotPaused)
RTI_EXCEPT(IDsupplyExhausted)
RTI_EXCEPT(InvalidDivestitureCondition)
RTI_EXCEPT(InvalidFederationTimeDelta)
RTI_EXCEPT(InvalidRoutingSpace)
RTI_EXCEPT(NoPauseRequested)
RTI_EXCEPT(NoResumeRequested)
RTI_EXCEPT(TooManyIDsRequested)
RTI_EXCEPT(UnimplementedService)
RTI_EXCEPT(UnknownLabel)
RTI_EXCEPT(NetworkSignal)
RTI_EXCEPT(NetworkError)

// Deprecated HLA types still in use
typedef char AttributeValue[MAX_BYTES_PER_VALUE + 1] ;
//typedef std::vector<char> AttributeValue ;

//  typedef std::vector<char>  AttributeValue;
typedef RTI::Double FederationTime ;
typedef UShort ObjectHandlecount ;
typedef char *ObjectName ;
typedef AttributeValue ParameterValue ;
typedef char *ParameterName ;
typedef AttributeValue ValueName ;
  typedef char ValueType[MAX_BYTES_PER_VALUETYPE + 1] ;
  //  typedef std::vector<char>  ValueType;
struct Value {
    ValueName name ;
    ValueName value ;
    ValueName type ;
    ULong length ;
};
typedef RTI::Double FederationTimeDelta ;
enum ObjectRemovalReason {
    OUT_OF_REGION = 1,
    OBJECT_DELETED,
    NO_LONGER_SUBSCRIBED
};
typedef Handle RegionHandle ;

// Constants (former HLA constants)
const TransportType RELIABLE = 1 ;
const TransportType BEST_EFFORT = 2 ;
const OrderType RECEIVE = 1 ;
const OrderType TIMESTAMP = 2 ;

// Constants
const int MAX_BACKLOG = 256 ;

// next introduced in order to manage value including any char (as \0)
typedef  struct vlp
    {
    AttributeValue value ;          // value is a lot of bytes
    ULong length ;                  // length i.e. bytes number of the value
} ValueLengthPair, ParameterLengthPair ;

} // namespace certi

// Define this if you want the complete object and interaction tree to
// be displayed as the FED file is read.
// #define CREAD_DISPLAYS_ROOTOBJ

// If you want statistics to be printed at the end of the execution,
// uncomment the next line.
#define RTI_PRINTS_STATISTICS

// You can comment the next line out if you don't want to use Multicast.
// #define FEDERATION_USES_MULTICAST

// If this macro is defined, some messages will be sent on UDP sockets
// by the RTIG to the RTIAs. Some messages types are NULL messages, and
// all Reflect Attributes Values messages, and Receive Interaction messages,
// even those with data asking for a "RELIABLE" service level.
// #define HLA_USES_UDP

// The next macro must contain the path name of the Audit File. It should
// be an absolute path, but it may be a relative path for testing reasons.
#define RTIG_AUDIT_FILENAME "RTIG.log"

// Define the lower audit level you need, from AUDIT_MIN_LEVEL(0, all)
// to AUDIT_MAX_LEVEL(10, min audit logging).
// Level 0 : NULL messages(time synchronization)
// Level 1 : Update Attribute Values messages
// Level 2 : Send Interaction messages
// Level 6 : Register / Delete Objects messages
// Level 7 : Publish / Subscribe messages
// Level 8 : Pause, Resume, Set Time regulating/constrained messages
// Level 9 : Create/Join/Resign/Destroy Federation msgs, Connections.
// Level 10: RTIG and Audit start/stop.
#define AUDIT_CURRENT_LEVEL 5

// ------------------------------
// -- GSSAPI Security Services --
// ------------------------------

// Comment out HLA_USES_GSSAPI to choose NOT to use GSSAPI services.
//#define HLA_USES_GSSAPI

// Server Principal Name
//(must be like "<rtip principal name>@<network machine name>")
#define HLA_SERVER_PRINCIPAL_NAME "RTIG@server"

// Expiration delay for GSSAPI sessions(must be greater then the longer
// lifetime of a federate).[In seconds]
#define HLA_GSS_SESSION_DURATION 3600

// You can use GSSAPI at least to authenticate federates(needed to use
// security levels), to sign all messages(source authentication), or
// to sign AND encrypt message.[last option]
// Just choose one of the three options below and affect it to the
// RTI_GSSAPI_USAGE macro.
#define RTI_GSS_NOTHING 0
#define RTI_GSS_SIGN_ONLY 1
#define RTI_GSS_ENCRYPT 2

#define RTI_GSSAPI_USAGE RTI_GSS_NOTHING

// Type of Security services needed : Can be any combination(OR) of the
// following flags(see your GSSAPI documentation for further details) :
//
// GSS_C_DELEG_FLAG Credential delegation(not supported by HLA anyway)
// GSS_C_MUTUAL_FLAG Mutual authentication
// GSS_C_REPLAY_FLAG Replay detection
// GSS_C_SEQUENCE_FLAG Out of sequence message detection.
//
// Note: Currently, no option is suppported by our default GSSAPI mechanism.
#define HLA_GSS_FLAGS 0


/* 
 * Basic bit swapping functions
 */
#define CERTI_UINT16_SWAP_BYTES(val)	((uint16_t) ( \
    (((uint16_t) (val) & (uint16_t) 0x00ffU) << 8) |  \
    (((uint16_t) (val) & (uint16_t) 0xff00U) >> 8)))

#define CERTI_UINT32_SWAP_BYTES(val)	((uint32_t) (     \
    (((uint32_t) (val) & (uint32_t) 0x000000ffU) << 24) | \
    (((uint32_t) (val) & (uint32_t) 0x0000ff00U) <<  8) | \
    (((uint32_t) (val) & (uint32_t) 0x00ff0000U) >>  8) | \
    (((uint32_t) (val) & (uint32_t) 0xff000000U) >> 24)))

#define CERTI_UINT64_SWAP_BYTES(val)	((uint64_t) (                   \
      (((uint64_t) (val) &						\
	(uint64_t) CERTI_INT64_CONSTANT(0x00000000000000ffU)) << 56) |	\
      (((uint64_t) (val) &						\
	(uint64_t) CERTI_INT64_CONSTANT(0x000000000000ff00U)) << 40) |	\
      (((uint64_t) (val) &						\
	(uint64_t) CERTI_INT64_CONSTANT(0x0000000000ff0000U)) << 24) |	\
      (((uint64_t) (val) &						\
	(uint64_t) CERTI_INT64_CONSTANT(0x00000000ff000000U)) <<  8) |	\
      (((uint64_t) (val) &						\
	(uint64_t) CERTI_INT64_CONSTANT(0x000000ff00000000U)) >>  8) |	\
      (((uint64_t) (val) &						\
	(uint64_t) CERTI_INT64_CONSTANT(0x0000ff0000000000U)) >> 24) |	\
      (((uint64_t) (val) &						\
	(uint64_t) CERTI_INT64_CONSTANT(0x00ff000000000000U)) >> 40) |	\
      (((uint64_t) (val) &						\
	(uint64_t) CERTI_INT64_CONSTANT(0xff00000000000000U)) >> 56)))
#endif // CERTI_HH_INCLUDED

// $Id: certi.hh,v 3.29 2008/06/11 15:19:20 rousse Exp $
