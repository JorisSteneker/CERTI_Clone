// This interface is used to access the services of the RTI. 

#ifndef RTI_RTI1516ambassador_h
#define RTI_RTI1516ambassador_h

#include <RTI/RTIambassador.h>
#include <RTI/RTIambassadorFactory.h>
#include "RTI1516ambPrivateRefs.h"


namespace rti1516
{
	class RTI_EXPORT RTI1516ambassador : RTIambassador
	{
		friend std::auto_ptr< RTIambassador >
			RTIambassadorFactory::createRTIambassador(std::vector< std::wstring > & args);
		//throw (BadInitializationParameter,
		//	RTIinternalError);

	private:
		RTI1516ambPrivateRefs* privateRefs ;

		// Helper functions
		template<typename T> void
			assignAHSAndExecuteService(const rti1516::AttributeHandleSet &AHS, T &req, T &rep);
		template<typename T> void
			assignPHVMAndExecuteService(const rti1516::ParameterHandleValueMap &PHVM, T &req, T &rep);
		template<typename T> void 
			assignAHVMAndExecuteService(const rti1516::AttributeHandleValueMap &AHVM, T &req, T &rep);
		// Helper function for CallBacks
		bool __tick_kernel(bool, TickTime, TickTime)
			throw (SpecifiedSaveLabelDoesNotExist, RTIinternalError);

	protected:
		RTI1516ambassador()
			throw ();

	public:
		virtual
			~RTI1516ambassador();
		// throw ()

		// 4.2
		virtual void createFederationExecution
			(std::wstring const & federationExecutionName,
			std::wstring const & fullPathNameToTheFDDfile,
			std::wstring const & logicalTimeImplementationName = L"")
			throw (FederationExecutionAlreadyExists,
			CouldNotOpenFDD,
			ErrorReadingFDD,
			CouldNotCreateLogicalTimeFactory,
			RTIinternalError);

		// 4.3
		virtual void destroyFederationExecution 
			(std::wstring const & federationExecutionName)
			throw (FederatesCurrentlyJoined,
			FederationExecutionDoesNotExist,
			RTIinternalError);

		// 4.4
		virtual FederateHandle joinFederationExecution 
			(std::wstring const & federateType,
			std::wstring const & federationExecutionName,
			FederateAmbassador & federateAmbassador)
			throw (FederateAlreadyExecutionMember,
			FederationExecutionDoesNotExist,
			SaveInProgress,
			RestoreInProgress,
			CouldNotCreateLogicalTimeFactory,
			RTIinternalError);

		// 4.5
		virtual void resignFederationExecution
			(ResignAction resignAction)
			throw (OwnershipAcquisitionPending,
			FederateOwnsAttributes,
			FederateNotExecutionMember,
			RTIinternalError);

		// 4.6
		virtual void registerFederationSynchronizationPoint
			(std::wstring const & label,
			VariableLengthData const & theUserSuppliedTag)
			throw (FederateNotExecutionMember,
			SaveInProgress,
			RestoreInProgress,
			RTIinternalError);

		virtual void registerFederationSynchronizationPoint
			(std::wstring const & label,
			VariableLengthData const & theUserSuppliedTag,
			FederateHandleSet const & syncSet)
			throw (FederateNotExecutionMember,
			SaveInProgress,
			RestoreInProgress,
			RTIinternalError);

		// 4.9
		virtual void synchronizationPointAchieved
			(std::wstring const & label)
			throw (SynchronizationPointLabelNotAnnounced,
			FederateNotExecutionMember,
			SaveInProgress,
			RestoreInProgress,
			RTIinternalError);

		// 4.11
		virtual void requestFederationSave
			(std::wstring const & label)
			throw (FederateNotExecutionMember,
			SaveInProgress,
			RestoreInProgress,
			RTIinternalError);

		virtual void requestFederationSave
			(std::wstring const & label,
			LogicalTime const & theTime)
			throw (LogicalTimeAlreadyPassed,
			InvalidLogicalTime,
			FederateUnableToUseTime,
			FederateNotExecutionMember,
			SaveInProgress,
			RestoreInProgress,
			RTIinternalError);

		// 4.13
		virtual void federateSaveBegun ()
			throw (SaveNotInitiated,
			FederateNotExecutionMember,
			RestoreInProgress,
			RTIinternalError);

		// 4.14
		virtual void federateSaveComplete ()
			throw (FederateHasNotBegunSave,
			FederateNotExecutionMember,
			RestoreInProgress,
			RTIinternalError);

		virtual void federateSaveNotComplete()
			throw (FederateHasNotBegunSave,
			FederateNotExecutionMember,
			RestoreInProgress,
			RTIinternalError);

		// 4.16
		virtual void queryFederationSaveStatus ()
			throw (FederateNotExecutionMember,
			RestoreInProgress,
			RTIinternalError);

		// 4.18
		virtual void requestFederationRestore
			(std::wstring const & label)
			throw (FederateNotExecutionMember,
			SaveInProgress,
			RestoreInProgress,
			RTIinternalError);

		// 4.22
		virtual void federateRestoreComplete ()
			throw (RestoreNotRequested,
			FederateNotExecutionMember,
			SaveInProgress,
			RTIinternalError);

		virtual void federateRestoreNotComplete ()
			throw (RestoreNotRequested,
			FederateNotExecutionMember,
			SaveInProgress,
			RTIinternalError);

		// 4.24
		virtual void queryFederationRestoreStatus ()
			throw (FederateNotExecutionMember,
			SaveInProgress,
			RTIinternalError);

		/////////////////////////////////////
		// Declaration Management Services //
		/////////////////////////////////////

		// 5.2
		virtual void publishObjectClassAttributes
			(ObjectClassHandle theClass,
			AttributeHandleSet const & attributeList)
			throw (ObjectClassNotDefined,
			AttributeNotDefined,
			FederateNotExecutionMember,
			SaveInProgress,
			RestoreInProgress,
			RTIinternalError);

		// 5.3
		virtual void unpublishObjectClass
			(ObjectClassHandle theClass)
			throw (ObjectClassNotDefined,
			OwnershipAcquisitionPending,
			FederateNotExecutionMember,
			SaveInProgress,
			RestoreInProgress,
			RTIinternalError);

		virtual void unpublishObjectClassAttributes
			(ObjectClassHandle theClass,
			AttributeHandleSet const & attributeList)
			throw (ObjectClassNotDefined,
			AttributeNotDefined,
			OwnershipAcquisitionPending,
			FederateNotExecutionMember,
			SaveInProgress,
			RestoreInProgress,
			RTIinternalError);

		// 5.4
		virtual void publishInteractionClass
			(InteractionClassHandle theInteraction)
			throw (InteractionClassNotDefined,
			FederateNotExecutionMember,
			SaveInProgress,
			RestoreInProgress,
			RTIinternalError);

		// 5.5
		virtual void unpublishInteractionClass
			(InteractionClassHandle theInteraction)
			throw (InteractionClassNotDefined,
			FederateNotExecutionMember,
			SaveInProgress,
			RestoreInProgress,
			RTIinternalError);

		// 5.6
		virtual void subscribeObjectClassAttributes
			(ObjectClassHandle theClass,
			AttributeHandleSet const & attributeList,
			bool active = true)
			throw (ObjectClassNotDefined,
			AttributeNotDefined,
			FederateNotExecutionMember,
			SaveInProgress,
			RestoreInProgress,
			RTIinternalError);

		// 5.7
		virtual void unsubscribeObjectClass
			(ObjectClassHandle theClass)
			throw (ObjectClassNotDefined,
			FederateNotExecutionMember,
			SaveInProgress,
			RestoreInProgress,
			RTIinternalError);

		virtual void unsubscribeObjectClassAttributes
			(ObjectClassHandle theClass,
			AttributeHandleSet const & attributeList)
			throw (ObjectClassNotDefined,
			AttributeNotDefined,
			FederateNotExecutionMember,
			SaveInProgress,
			RestoreInProgress,
			RTIinternalError);

		// 5.8
		virtual void subscribeInteractionClass
			(InteractionClassHandle theClass,
			bool active = true)
			throw (InteractionClassNotDefined,
			FederateServiceInvocationsAreBeingReportedViaMOM,
			FederateNotExecutionMember,
			SaveInProgress,
			RestoreInProgress,
			RTIinternalError);

		// 5.9
		virtual void unsubscribeInteractionClass
			(InteractionClassHandle theClass)
			throw (InteractionClassNotDefined,
			FederateNotExecutionMember,
			SaveInProgress,
			RestoreInProgress,
			RTIinternalError);

		////////////////////////////////
		// Object Management Services //
		////////////////////////////////

		// 6.2
		virtual void reserveObjectInstanceName
			(std::wstring const & theObjectInstanceName)
			throw (IllegalName,
			FederateNotExecutionMember,
			SaveInProgress,
			RestoreInProgress,
			RTIinternalError);

		// 6.4
		virtual ObjectInstanceHandle registerObjectInstance
			(ObjectClassHandle theClass)
			throw (ObjectClassNotDefined,
			ObjectClassNotPublished,
			FederateNotExecutionMember,
			SaveInProgress,
			RestoreInProgress,
			RTIinternalError);

		virtual ObjectInstanceHandle registerObjectInstance
			(ObjectClassHandle theClass,
			std::wstring const & theObjectInstanceName)
			throw (ObjectClassNotDefined,
			ObjectClassNotPublished,
			ObjectInstanceNameNotReserved,
			ObjectInstanceNameInUse,
			FederateNotExecutionMember,
			SaveInProgress,
			RestoreInProgress,
			RTIinternalError);

		// 6.6
		virtual void updateAttributeValues
			(ObjectInstanceHandle theObject,
			AttributeHandleValueMap const & theAttributeValues,
			VariableLengthData const & theUserSuppliedTag)
			throw (ObjectInstanceNotKnown,
			AttributeNotDefined,
			AttributeNotOwned,
			FederateNotExecutionMember,
			SaveInProgress,
			RestoreInProgress,
			RTIinternalError);

		virtual MessageRetractionHandle updateAttributeValues
			(ObjectInstanceHandle theObject,
			AttributeHandleValueMap const & theAttributeValues,
			VariableLengthData const & theUserSuppliedTag,
			LogicalTime const & theTime)
			throw (ObjectInstanceNotKnown,
			AttributeNotDefined,
			AttributeNotOwned,
			InvalidLogicalTime,
			FederateNotExecutionMember,
			SaveInProgress,
			RestoreInProgress,
			RTIinternalError);

		// 6.8
		virtual void sendInteraction
			(InteractionClassHandle theInteraction,
			ParameterHandleValueMap const & theParameterValues,
			VariableLengthData const & theUserSuppliedTag)
			throw (InteractionClassNotPublished,
			InteractionClassNotDefined,
			InteractionParameterNotDefined,
			FederateNotExecutionMember,
			SaveInProgress,
			RestoreInProgress,
			RTIinternalError);

		virtual MessageRetractionHandle sendInteraction
			(InteractionClassHandle theInteraction,
			ParameterHandleValueMap const & theParameterValues,
			VariableLengthData const & theUserSuppliedTag,
			LogicalTime const & theTime)
			throw (InteractionClassNotPublished,
			InteractionClassNotDefined,
			InteractionParameterNotDefined,
			InvalidLogicalTime,
			FederateNotExecutionMember,
			SaveInProgress,
			RestoreInProgress,
			RTIinternalError);

		// 6.10
		virtual void deleteObjectInstance
			(ObjectInstanceHandle theObject,
			VariableLengthData const & theUserSuppliedTag)
			throw (DeletePrivilegeNotHeld,
			ObjectInstanceNotKnown,
			FederateNotExecutionMember,
			SaveInProgress,
			RestoreInProgress,
			RTIinternalError);

		virtual MessageRetractionHandle deleteObjectInstance
			(ObjectInstanceHandle theObject,
			VariableLengthData const & theUserSuppliedTag,
			LogicalTime  const & theTime)
			throw (DeletePrivilegeNotHeld,
			ObjectInstanceNotKnown,
			InvalidLogicalTime,
			FederateNotExecutionMember,
			SaveInProgress,
			RestoreInProgress,
			RTIinternalError);

		// 6.12
		virtual void localDeleteObjectInstance
			(ObjectInstanceHandle theObject)
			throw (ObjectInstanceNotKnown,
			FederateOwnsAttributes,
			OwnershipAcquisitionPending,
			FederateNotExecutionMember,
			SaveInProgress,
			RestoreInProgress,
			RTIinternalError);

		// 6.13
		virtual void changeAttributeTransportationType
			(ObjectInstanceHandle theObject,
			AttributeHandleSet const & theAttributes,
			TransportationType theType)
			throw (ObjectInstanceNotKnown,
			AttributeNotDefined,
			AttributeNotOwned,
			FederateNotExecutionMember,
			SaveInProgress,
			RestoreInProgress,
			RTIinternalError);

		// 6.14
		virtual void changeInteractionTransportationType
			(InteractionClassHandle theClass,
			TransportationType theType)
			throw (InteractionClassNotDefined,
			InteractionClassNotPublished,
			FederateNotExecutionMember,
			SaveInProgress,
			RestoreInProgress,
			RTIinternalError);

		// 6.17
		virtual void requestAttributeValueUpdate
			(ObjectInstanceHandle theObject,
			AttributeHandleSet const & theAttributes,
			VariableLengthData const & theUserSuppliedTag)
			throw (ObjectInstanceNotKnown,
			AttributeNotDefined,
			FederateNotExecutionMember,
			SaveInProgress,
			RestoreInProgress,
			RTIinternalError);

		virtual void requestAttributeValueUpdate
			(ObjectClassHandle theClass,
			AttributeHandleSet const & theAttributes,
			VariableLengthData const & theUserSuppliedTag)
			throw (ObjectClassNotDefined,
			AttributeNotDefined,
			FederateNotExecutionMember,
			SaveInProgress,
			RestoreInProgress,
			RTIinternalError);

		///////////////////////////////////
		// Ownership Management Services //
		///////////////////////////////////
		// 7.2
		virtual void unconditionalAttributeOwnershipDivestiture
			(ObjectInstanceHandle theObject,
			AttributeHandleSet const & theAttributes)
			throw (ObjectInstanceNotKnown,
			AttributeNotDefined,
			AttributeNotOwned,
			FederateNotExecutionMember,
			SaveInProgress,
			RestoreInProgress,
			RTIinternalError);

		// 7.3
		virtual void negotiatedAttributeOwnershipDivestiture
			(ObjectInstanceHandle theObject,
			AttributeHandleSet const & theAttributes,
			VariableLengthData const & theUserSuppliedTag)
			throw (ObjectInstanceNotKnown,
			AttributeNotDefined,
			AttributeNotOwned,
			AttributeAlreadyBeingDivested,
			FederateNotExecutionMember,
			SaveInProgress,
			RestoreInProgress,
			RTIinternalError);

		// 7.6
		virtual void confirmDivestiture
			(ObjectInstanceHandle theObject,
			AttributeHandleSet const & confirmedAttributes,
			VariableLengthData const & theUserSuppliedTag)
			throw (ObjectInstanceNotKnown,
			AttributeNotDefined,
			AttributeNotOwned,
			AttributeDivestitureWasNotRequested,
			NoAcquisitionPending,
			FederateNotExecutionMember,
			SaveInProgress,
			RestoreInProgress,
			RTIinternalError);

		// 7.8
		virtual void attributeOwnershipAcquisition
			(ObjectInstanceHandle theObject,
			AttributeHandleSet const & desiredAttributes,
			VariableLengthData const & theUserSuppliedTag)
			throw (ObjectInstanceNotKnown,
			ObjectClassNotPublished,
			AttributeNotDefined,
			AttributeNotPublished,
			FederateOwnsAttributes,
			FederateNotExecutionMember,
			SaveInProgress,
			RestoreInProgress,
			RTIinternalError);

		// 7.9
		virtual void attributeOwnershipAcquisitionIfAvailable
			(ObjectInstanceHandle theObject,
			AttributeHandleSet const & desiredAttributes)
			throw (ObjectInstanceNotKnown,
			ObjectClassNotPublished,
			AttributeNotDefined,
			AttributeNotPublished,
			FederateOwnsAttributes,
			AttributeAlreadyBeingAcquired,
			FederateNotExecutionMember,
			SaveInProgress,
			RestoreInProgress,
			RTIinternalError);

		// 7.12
		virtual void attributeOwnershipDivestitureIfWanted
			(ObjectInstanceHandle theObject,
			AttributeHandleSet const & theAttributes,
			AttributeHandleSet & theDivestedAttributes) // filled by RTI
			throw (ObjectInstanceNotKnown,
			AttributeNotDefined,
			AttributeNotOwned,
			FederateNotExecutionMember,
			SaveInProgress,
			RestoreInProgress,
			RTIinternalError);

		// 7.13
		virtual void cancelNegotiatedAttributeOwnershipDivestiture
			(ObjectInstanceHandle theObject,
			AttributeHandleSet const & theAttributes)
			throw (ObjectInstanceNotKnown,
			AttributeNotDefined,
			AttributeNotOwned,
			AttributeDivestitureWasNotRequested,
			FederateNotExecutionMember,
			SaveInProgress,
			RestoreInProgress,
			RTIinternalError);

		// 7.14
		virtual void cancelAttributeOwnershipAcquisition
			(ObjectInstanceHandle theObject,
			AttributeHandleSet const & theAttributes)
			throw (ObjectInstanceNotKnown,
			AttributeNotDefined,
			AttributeAlreadyOwned,
			AttributeAcquisitionWasNotRequested,
			FederateNotExecutionMember,
			SaveInProgress,
			RestoreInProgress,
			RTIinternalError);

		// 7.16
		virtual void queryAttributeOwnership
			(ObjectInstanceHandle theObject,
			AttributeHandle theAttribute)
			throw (ObjectInstanceNotKnown,
			AttributeNotDefined,
			FederateNotExecutionMember,
			SaveInProgress,
			RestoreInProgress,
			RTIinternalError);

		// 7.18
		virtual bool isAttributeOwnedByFederate
			(ObjectInstanceHandle theObject,
			AttributeHandle theAttribute)
			throw (ObjectInstanceNotKnown,
			AttributeNotDefined,
			FederateNotExecutionMember,
			SaveInProgress,
			RestoreInProgress,
			RTIinternalError);

		//////////////////////////////
		// Time Management Services //
		//////////////////////////////

		// 8.2
		virtual void enableTimeRegulation
			(LogicalTimeInterval const & theLookahead)
			throw (TimeRegulationAlreadyEnabled,
			InvalidLookahead,
			InTimeAdvancingState,
			RequestForTimeRegulationPending,
			FederateNotExecutionMember,
			SaveInProgress,
			RestoreInProgress,
			RTIinternalError);

		// 8.4
		virtual void disableTimeRegulation ()
			throw (TimeRegulationIsNotEnabled,
			FederateNotExecutionMember,
			SaveInProgress,
			RestoreInProgress,
			RTIinternalError);

		// 8.5
		virtual void enableTimeConstrained ()
			throw (TimeConstrainedAlreadyEnabled,
			InTimeAdvancingState,
			RequestForTimeConstrainedPending,
			FederateNotExecutionMember,
			SaveInProgress,
			RestoreInProgress,
			RTIinternalError);

		// 8.7
		virtual void disableTimeConstrained ()
			throw (TimeConstrainedIsNotEnabled,
			FederateNotExecutionMember,
			SaveInProgress,
			RestoreInProgress,
			RTIinternalError);

		// 8.8
		virtual void timeAdvanceRequest
			(LogicalTime const & theTime)
			throw (InvalidLogicalTime,
			LogicalTimeAlreadyPassed,
			InTimeAdvancingState,
			RequestForTimeRegulationPending,
			RequestForTimeConstrainedPending,
			FederateNotExecutionMember,
			SaveInProgress,
			RestoreInProgress,
			RTIinternalError);

		// 8.9
		virtual void timeAdvanceRequestAvailable
			(LogicalTime const & theTime)
			throw (InvalidLogicalTime,
			LogicalTimeAlreadyPassed,
			InTimeAdvancingState,
			RequestForTimeRegulationPending,
			RequestForTimeConstrainedPending,
			FederateNotExecutionMember,
			SaveInProgress,
			RestoreInProgress,
			RTIinternalError);

		// 8.10
		virtual void nextMessageRequest
			(LogicalTime const & theTime)
			throw (InvalidLogicalTime,
			LogicalTimeAlreadyPassed,
			InTimeAdvancingState,
			RequestForTimeRegulationPending,
			RequestForTimeConstrainedPending,
			FederateNotExecutionMember,
			SaveInProgress,
			RestoreInProgress,
			RTIinternalError);

		// 8.11
		virtual void nextMessageRequestAvailable
			(LogicalTime const & theTime)
			throw (InvalidLogicalTime,
			LogicalTimeAlreadyPassed,
			InTimeAdvancingState,
			RequestForTimeRegulationPending,
			RequestForTimeConstrainedPending,
			FederateNotExecutionMember,
			SaveInProgress,
			RestoreInProgress,
			RTIinternalError);

		// 8.12
		virtual void flushQueueRequest
			(LogicalTime const & theTime)
			throw (InvalidLogicalTime,
			LogicalTimeAlreadyPassed,
			InTimeAdvancingState,
			RequestForTimeRegulationPending,
			RequestForTimeConstrainedPending,
			FederateNotExecutionMember,
			SaveInProgress,
			RestoreInProgress,
			RTIinternalError);

		// 8.14
		virtual void enableAsynchronousDelivery ()
			throw (AsynchronousDeliveryAlreadyEnabled,
			FederateNotExecutionMember,
			SaveInProgress,
			RestoreInProgress,
			RTIinternalError);

		// 8.15
		virtual void disableAsynchronousDelivery ()
			throw (AsynchronousDeliveryAlreadyDisabled,
			FederateNotExecutionMember,
			SaveInProgress,
			RestoreInProgress,
			RTIinternalError);

		// 8.16
		virtual bool queryGALT (LogicalTime & theTime)
			throw (FederateNotExecutionMember,
			SaveInProgress,
			RestoreInProgress,
			RTIinternalError);

		// 8.17
		virtual void queryLogicalTime (LogicalTime & theTime)
			throw (FederateNotExecutionMember,
			SaveInProgress,
			RestoreInProgress,
			RTIinternalError);

		// 8.18
		virtual bool queryLITS (LogicalTime & theTime)
			throw (FederateNotExecutionMember,
			SaveInProgress,
			RestoreInProgress,
			RTIinternalError);

		// 8.19
		virtual void modifyLookahead
			(LogicalTimeInterval const & theLookahead)
			throw (TimeRegulationIsNotEnabled,
			InvalidLookahead,
			InTimeAdvancingState,
			FederateNotExecutionMember,
			SaveInProgress,
			RestoreInProgress,
			RTIinternalError);

		// 8.20
		virtual void queryLookahead (LogicalTimeInterval & interval)
			throw (TimeRegulationIsNotEnabled,
			FederateNotExecutionMember,
			SaveInProgress,
			RestoreInProgress,
			RTIinternalError);

		// 8.21
		virtual void retract
			(MessageRetractionHandle theHandle)
			throw (InvalidRetractionHandle,
			TimeRegulationIsNotEnabled,
			MessageCanNoLongerBeRetracted,
			FederateNotExecutionMember,
			SaveInProgress,
			RestoreInProgress,
			RTIinternalError);

		// 8.23
		virtual void changeAttributeOrderType
			(ObjectInstanceHandle theObject,
			AttributeHandleSet const & theAttributes,
			OrderType theType)
			throw (ObjectInstanceNotKnown,
			AttributeNotDefined,
			AttributeNotOwned,
			FederateNotExecutionMember,
			SaveInProgress,
			RestoreInProgress,
			RTIinternalError);

		// 8.24
		virtual void changeInteractionOrderType
			(InteractionClassHandle theClass,
			OrderType theType)
			throw (InteractionClassNotDefined,
			InteractionClassNotPublished,
			FederateNotExecutionMember,
			SaveInProgress,
			RestoreInProgress,
			RTIinternalError);

		//////////////////////////////////
		// Data Distribution Management //
		//////////////////////////////////

		// 9.2
		virtual RegionHandle createRegion
			(DimensionHandleSet const & theDimensions)
			throw (InvalidDimensionHandle,
			FederateNotExecutionMember,
			SaveInProgress,
			RestoreInProgress,
			RTIinternalError);

		// 9.3
		virtual void commitRegionModifications
			(RegionHandleSet const & theRegionHandleSet)
			throw (InvalidRegion,
			RegionNotCreatedByThisFederate,
			FederateNotExecutionMember,
			SaveInProgress,
			RestoreInProgress,
			RTIinternalError);

		// 9.4
		virtual void deleteRegion
			(RegionHandle theRegion)
			throw (InvalidRegion,
			RegionNotCreatedByThisFederate,
			RegionInUseForUpdateOrSubscription,
			FederateNotExecutionMember,
			SaveInProgress,
			RestoreInProgress,
			RTIinternalError);

		// 9.5
		virtual ObjectInstanceHandle registerObjectInstanceWithRegions
			(ObjectClassHandle theClass,
			AttributeHandleSetRegionHandleSetPairVector const &
			theAttributeHandleSetRegionHandleSetPairVector)
			throw (ObjectClassNotDefined,
			ObjectClassNotPublished,
			AttributeNotDefined,
			AttributeNotPublished,
			InvalidRegion,
			RegionNotCreatedByThisFederate,
			InvalidRegionContext,
			FederateNotExecutionMember,
			SaveInProgress,
			RestoreInProgress,
			RTIinternalError);

		virtual ObjectInstanceHandle registerObjectInstanceWithRegions
			(ObjectClassHandle theClass,
			AttributeHandleSetRegionHandleSetPairVector const &
			theAttributeHandleSetRegionHandleSetPairVector,
			std::wstring const & theObjectInstanceName)
			throw (ObjectClassNotDefined,
			ObjectClassNotPublished,
			AttributeNotDefined,
			AttributeNotPublished,
			InvalidRegion,
			RegionNotCreatedByThisFederate,
			InvalidRegionContext,
			ObjectInstanceNameNotReserved,
			ObjectInstanceNameInUse,
			FederateNotExecutionMember,
			SaveInProgress,
			RestoreInProgress,
			RTIinternalError);

		// 9.6
		virtual void associateRegionsForUpdates
			(ObjectInstanceHandle theObject,
			AttributeHandleSetRegionHandleSetPairVector const &
			theAttributeHandleSetRegionHandleSetPairVector)
			throw (ObjectInstanceNotKnown,
			AttributeNotDefined,
			InvalidRegion,
			RegionNotCreatedByThisFederate,
			InvalidRegionContext,
			FederateNotExecutionMember,
			SaveInProgress,
			RestoreInProgress,
			RTIinternalError);

		// 9.7
		virtual void unassociateRegionsForUpdates
			(ObjectInstanceHandle theObject,
			AttributeHandleSetRegionHandleSetPairVector const &
			theAttributeHandleSetRegionHandleSetPairVector)
			throw (ObjectInstanceNotKnown,
			AttributeNotDefined,
			InvalidRegion,
			RegionNotCreatedByThisFederate,
			FederateNotExecutionMember,
			SaveInProgress,
			RestoreInProgress,
			RTIinternalError);

		// 9.8
		virtual void subscribeObjectClassAttributesWithRegions
			(ObjectClassHandle theClass,
			AttributeHandleSetRegionHandleSetPairVector const &
			theAttributeHandleSetRegionHandleSetPairVector,
			bool active = true)
			throw (ObjectClassNotDefined,
			AttributeNotDefined,
			InvalidRegion,
			RegionNotCreatedByThisFederate,
			InvalidRegionContext,
			FederateNotExecutionMember,
			SaveInProgress,
			RestoreInProgress,
			RTIinternalError);

		// 9.9
		virtual void unsubscribeObjectClassAttributesWithRegions
			(ObjectClassHandle theClass,
			AttributeHandleSetRegionHandleSetPairVector const &
			theAttributeHandleSetRegionHandleSetPairVector)
			throw (ObjectClassNotDefined,
			AttributeNotDefined,
			InvalidRegion,
			RegionNotCreatedByThisFederate,
			FederateNotExecutionMember,
			SaveInProgress,
			RestoreInProgress,
			RTIinternalError);

		// 9.10
		virtual void subscribeInteractionClassWithRegions
			(InteractionClassHandle theClass,
			RegionHandleSet const & theRegionHandleSet,
			bool active = true)
			throw (InteractionClassNotDefined,
			InvalidRegion,
			RegionNotCreatedByThisFederate,
			InvalidRegionContext,
			FederateServiceInvocationsAreBeingReportedViaMOM,
			FederateNotExecutionMember,
			SaveInProgress,
			RestoreInProgress,
			RTIinternalError);

		// 9.11
		virtual void unsubscribeInteractionClassWithRegions
			(InteractionClassHandle theClass,
			RegionHandleSet const & theRegionHandleSet)
			throw (InteractionClassNotDefined,
			InvalidRegion,
			RegionNotCreatedByThisFederate,
			FederateNotExecutionMember,
			SaveInProgress,
			RestoreInProgress,
			RTIinternalError);

		// 9.12
		virtual void sendInteractionWithRegions
			(InteractionClassHandle theInteraction,
			ParameterHandleValueMap const & theParameterValues,
			RegionHandleSet const & theRegionHandleSet,
			VariableLengthData const & theUserSuppliedTag)
			throw (InteractionClassNotDefined,
			InteractionClassNotPublished,
			InteractionParameterNotDefined,
			InvalidRegion,
			RegionNotCreatedByThisFederate,
			InvalidRegionContext,
			FederateNotExecutionMember,
			SaveInProgress,
			RestoreInProgress,
			RTIinternalError);

		virtual MessageRetractionHandle sendInteractionWithRegions
			(InteractionClassHandle theInteraction,
			ParameterHandleValueMap const & theParameterValues,
			RegionHandleSet const & theRegionHandleSet,
			VariableLengthData const & theUserSuppliedTag,
			LogicalTime const & theTime)
			throw (InteractionClassNotDefined,
			InteractionClassNotPublished,
			InteractionParameterNotDefined,
			InvalidRegion,
			RegionNotCreatedByThisFederate,
			InvalidRegionContext,
			InvalidLogicalTime,
			FederateNotExecutionMember,
			SaveInProgress,
			RestoreInProgress,
			RTIinternalError);

		// 9.13
		virtual void requestAttributeValueUpdateWithRegions
			(ObjectClassHandle theClass,
			AttributeHandleSetRegionHandleSetPairVector const & theSet,
			VariableLengthData const & theUserSuppliedTag)
			throw (ObjectClassNotDefined,
			AttributeNotDefined,
			InvalidRegion,
			RegionNotCreatedByThisFederate,
			InvalidRegionContext,
			FederateNotExecutionMember,
			SaveInProgress,
			RestoreInProgress,
			RTIinternalError);

		//////////////////////////
		// RTI Support Services //
		//////////////////////////

		// 10.2
		virtual ObjectClassHandle getObjectClassHandle
			(std::wstring const & theName)
			throw (NameNotFound,
			FederateNotExecutionMember,
			RTIinternalError);

		// 10.3
		virtual std::wstring getObjectClassName
			(ObjectClassHandle theHandle)
			throw (InvalidObjectClassHandle,
			FederateNotExecutionMember,
			RTIinternalError);

		// 10.4
		virtual AttributeHandle getAttributeHandle
			(ObjectClassHandle whichClass,
			std::wstring const & theAttributeName)
			throw (InvalidObjectClassHandle,
			NameNotFound,
			FederateNotExecutionMember,
			RTIinternalError);

		// 10.5
		virtual std::wstring getAttributeName
			(ObjectClassHandle whichClass,
			AttributeHandle theHandle)   
			throw (InvalidObjectClassHandle,
			InvalidAttributeHandle,
			AttributeNotDefined,
			FederateNotExecutionMember,
			RTIinternalError);

		// 10.6
		virtual InteractionClassHandle getInteractionClassHandle
			(std::wstring const & theName)
			throw (NameNotFound,
			FederateNotExecutionMember,
			RTIinternalError);

		// 10.7
		virtual std::wstring getInteractionClassName
			(InteractionClassHandle theHandle)
			throw (InvalidInteractionClassHandle,
			FederateNotExecutionMember,
			RTIinternalError);

		// 10.8
		virtual ParameterHandle getParameterHandle
			(InteractionClassHandle whichClass,
			std::wstring const & theName)
			throw (InvalidInteractionClassHandle,
			NameNotFound,
			FederateNotExecutionMember,
			RTIinternalError);

		// 10.9
		virtual std::wstring getParameterName
			(InteractionClassHandle whichClass,
			ParameterHandle theHandle)   
			throw (InvalidInteractionClassHandle,
			InvalidParameterHandle,
			InteractionParameterNotDefined,
			FederateNotExecutionMember,
			RTIinternalError);

		// 10.10
		virtual ObjectInstanceHandle getObjectInstanceHandle
			(std::wstring const & theName)
			throw (ObjectInstanceNotKnown,
			FederateNotExecutionMember,
			RTIinternalError);

		// 10.11
		virtual std::wstring getObjectInstanceName
			(ObjectInstanceHandle theHandle)
			throw (ObjectInstanceNotKnown,
			FederateNotExecutionMember,
			RTIinternalError);

		// 10.12
		virtual DimensionHandle getDimensionHandle
			(std::wstring const & theName)
			throw (NameNotFound,
			FederateNotExecutionMember,
			RTIinternalError);

		// 10.13
		virtual std::wstring getDimensionName
			(DimensionHandle theHandle)
			throw (InvalidDimensionHandle,
			FederateNotExecutionMember,
			RTIinternalError);

		// 10.14
		virtual unsigned long getDimensionUpperBound
			(DimensionHandle theHandle)   
			throw (InvalidDimensionHandle,
			FederateNotExecutionMember,
			RTIinternalError);

		// 10.15
		virtual DimensionHandleSet getAvailableDimensionsForClassAttribute
			(ObjectClassHandle theClass,
			AttributeHandle theHandle)   
			throw (InvalidObjectClassHandle,
			InvalidAttributeHandle,
			AttributeNotDefined,
			FederateNotExecutionMember,
			RTIinternalError);

		// 10.16
		virtual ObjectClassHandle getKnownObjectClassHandle
			(ObjectInstanceHandle theObject)
			throw (ObjectInstanceNotKnown,
			FederateNotExecutionMember,
			RTIinternalError);

		// 10.17
		virtual DimensionHandleSet getAvailableDimensionsForInteractionClass
			(InteractionClassHandle theClass)
			throw (InvalidInteractionClassHandle,
			FederateNotExecutionMember,
			RTIinternalError);

		// 10.18
		virtual TransportationType getTransportationType
			(std::wstring const & transportationName)
			throw (InvalidTransportationName,
			FederateNotExecutionMember,
			RTIinternalError);

		// 10.19
		virtual std::wstring getTransportationName
			(TransportationType transportationType)
			throw (InvalidTransportationType,
			FederateNotExecutionMember,
			RTIinternalError);

		// 10.20
		virtual OrderType getOrderType
			(std::wstring const & orderName)
			throw (InvalidOrderName,
			FederateNotExecutionMember,
			RTIinternalError);

		// 10.21
		virtual std::wstring getOrderName
			(OrderType orderType)
			throw (InvalidOrderType,
			FederateNotExecutionMember,
			RTIinternalError);

		// 10.22
		virtual void enableObjectClassRelevanceAdvisorySwitch ()
			throw (ObjectClassRelevanceAdvisorySwitchIsOn,
			FederateNotExecutionMember,
			SaveInProgress,
			RestoreInProgress,
			RTIinternalError);

		// 10.23
		virtual void disableObjectClassRelevanceAdvisorySwitch ()
			throw (ObjectClassRelevanceAdvisorySwitchIsOff,
			FederateNotExecutionMember,
			SaveInProgress,
			RestoreInProgress,
			RTIinternalError);

		// 10.24
		virtual void enableAttributeRelevanceAdvisorySwitch ()
			throw (AttributeRelevanceAdvisorySwitchIsOn,
			FederateNotExecutionMember,
			SaveInProgress,
			RestoreInProgress,
			RTIinternalError);

		// 10.25
		virtual void disableAttributeRelevanceAdvisorySwitch ()
			throw (AttributeRelevanceAdvisorySwitchIsOff,
			FederateNotExecutionMember,
			SaveInProgress,
			RestoreInProgress,
			RTIinternalError);

		// 10.26
		virtual void enableAttributeScopeAdvisorySwitch ()
			throw (AttributeScopeAdvisorySwitchIsOn,
			FederateNotExecutionMember,
			SaveInProgress,
			RestoreInProgress,
			RTIinternalError);

		// 10.27
		virtual void disableAttributeScopeAdvisorySwitch ()
			throw (AttributeScopeAdvisorySwitchIsOff,
			FederateNotExecutionMember,
			SaveInProgress,
			RestoreInProgress,
			RTIinternalError);

		// 10.28
		virtual void enableInteractionRelevanceAdvisorySwitch ()
			throw (InteractionRelevanceAdvisorySwitchIsOn,
			FederateNotExecutionMember,
			SaveInProgress,
			RestoreInProgress,
			RTIinternalError);

		// 10.29
		virtual void disableInteractionRelevanceAdvisorySwitch ()
			throw (InteractionRelevanceAdvisorySwitchIsOff,
			FederateNotExecutionMember,
			SaveInProgress,
			RestoreInProgress,
			RTIinternalError);

		// 10.30
		virtual
			DimensionHandleSet getDimensionHandleSet
			(RegionHandle theRegionHandle)
			throw (InvalidRegion,
			FederateNotExecutionMember,
			SaveInProgress,
			RestoreInProgress,
			RTIinternalError);

		// 10.31
		virtual
			RangeBounds getRangeBounds
			(RegionHandle theRegionHandle,
			DimensionHandle theDimensionHandle)
			throw (InvalidRegion,
			RegionDoesNotContainSpecifiedDimension,
			FederateNotExecutionMember,
			SaveInProgress,
			RestoreInProgress,
			RTIinternalError);

		// 10.32
		virtual void setRangeBounds
			(RegionHandle theRegionHandle,
			DimensionHandle theDimensionHandle,
			RangeBounds const & theRangeBounds)
			throw (InvalidRegion,
			RegionNotCreatedByThisFederate,
			RegionDoesNotContainSpecifiedDimension,
			InvalidRangeBound,
			FederateNotExecutionMember,
			SaveInProgress,
			RestoreInProgress,
			RTIinternalError);

		// 10.33
		virtual unsigned long normalizeFederateHandle
			(FederateHandle theFederateHandle)
			throw (FederateNotExecutionMember,
			InvalidFederateHandle,
			RTIinternalError);

		// 10.34
		virtual unsigned long normalizeServiceGroup
			(ServiceGroupIndicator theServiceGroup)
			throw (FederateNotExecutionMember,
			InvalidServiceGroup,
			RTIinternalError);

		// 10.37
		virtual bool evokeCallback(double approximateMinimumTimeInSeconds)
			throw (FederateNotExecutionMember,
			RTIinternalError);

		// 10.38
		virtual bool evokeMultipleCallbacks(double approximateMinimumTimeInSeconds,
			double approximateMaximumTimeInSeconds)
			throw (FederateNotExecutionMember,
			RTIinternalError);

		// 10.39
		virtual void enableCallbacks ()
			throw (FederateNotExecutionMember,
			SaveInProgress,
			RestoreInProgress,
			RTIinternalError);

		// 10.40
		virtual void disableCallbacks ()
			throw (FederateNotExecutionMember,
			SaveInProgress,
			RestoreInProgress,
			RTIinternalError);

		virtual FederateHandle decodeFederateHandle(
			VariableLengthData const & encodedValue) const;

		virtual ObjectClassHandle decodeObjectClassHandle(
			VariableLengthData const & encodedValue) const;

		virtual InteractionClassHandle decodeInteractionClassHandle(
			VariableLengthData const & encodedValue) const;

		virtual ObjectInstanceHandle decodeObjectInstanceHandle(
			VariableLengthData const & encodedValue) const;

		virtual AttributeHandle decodeAttributeHandle(
			VariableLengthData const & encodedValue) const;

		virtual ParameterHandle decodeParameterHandle(
			VariableLengthData const & encodedValue) const;

		virtual DimensionHandle decodeDimensionHandle(
			VariableLengthData const & encodedValue) const;

		virtual MessageRetractionHandle decodeMessageRetractionHandle(
			VariableLengthData const & encodedValue) const;

		virtual RegionHandle decodeRegionHandle(
			VariableLengthData const & encodedValue) const;

	};
}

#endif // RTI_RTI1516ambassador_h
