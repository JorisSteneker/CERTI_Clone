// ----------------------------------------------------------------------------
// CERTI - HLA RunTime Infrastructure
// Copyright (C) 2002, 2003  ONERA
//
// This file is part of CERTI-libCERTI
//
// CERTI-libCERTI is free software ; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation ; either version 2 of
// the License, or (at your option) any later version.
//
// CERTI-libCERTI is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY ; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this program ; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA
//
// $Id: Message_RW.cc,v 3.15 2003/07/09 16:00:59 breholee Exp $
// ----------------------------------------------------------------------------

#include <config.h>
#include "Message.hh"

namespace certi {

static pdCDebug D("MESSAGE", "(Message_RW) ");

// You can comment this out if you don't want to optimize network messages.
#define USE_HEADER_AND_BODY

// ----------------------------------------------------------------------------
//! Read NetworkMessage Objects from Socket objects.
void
Message::read(SocketUN *socket)
    throw (NetworkError,
           NetworkSignal)
{
#ifdef USE_HEADER_AND_BODY

    Boolean Result ;

    // 1- Read Header
    Result = readHeader(socket);

    // 2- if ReadHeader returned RTI_TRUE, Read Body.
    if (Result == RTI_TRUE)
        readBody(socket);

#else
    socket->receive((void *) this, sizeof(Message));
#endif
}

// ----------------------------------------------------------------------------
//! Read a Message Body from a Socket, should be called after ReadHeader.
void
Message::readBody(SocketUN *socket)
{
    MessageBody Body ;

    if (header.bodySize == 0)
        throw RTIinternalError("ReadBody should not have been called.");

    // 1. Read Body from socket.

    socket->receive((void *) Body.getBuffer(), header.bodySize);

    // 3. Read informations from Message Body according to message type.

    if (header.exception != e_NO_EXCEPTION) {
        Body.readString(exceptionReason, MAX_EXCEPTION_REASON_LENGTH);
    }
    else {

        // 1- Prepare Body Structure according to Message Type
        switch(header.type) {

            // --- No Variable Part, Body not empty ---

          case CREATE_FEDERATION_EXECUTION:
          case DESTROY_FEDERATION_EXECUTION:
            readFederationName(&Body);
            break ;

          case REGISTER_FEDERATION_SYNCHRONIZATION_POINT:
          case ANNOUNCE_SYNCHRONIZATION_POINT:
          case REQUEST_FEDERATION_RESTORE_FAILED:
            readLabel(&Body);
            readTag(&Body);
            break ;

          case SYNCHRONIZATION_POINT_REGISTRATION_SUCCEEDED:
          case SYNCHRONIZATION_POINT_ACHIEVED:
          case FEDERATION_SYNCHRONIZED:
          case REQUEST_FEDERATION_SAVE:
          case INITIATE_FEDERATE_SAVE:
          case REQUEST_FEDERATION_RESTORE:
          case REQUEST_FEDERATION_RESTORE_SUCCEEDED:
          case INITIATE_FEDERATE_RESTORE:
            readLabel(&Body);
            break ;

          case IS_ATTRIBUTE_OWNED_BY_FEDERATE:
          case QUERY_ATTRIBUTE_OWNERSHIP:
            // B.c. Objectid, AttribHandle and Tag.
            object = Body.readLongInt();
            attribute = Body.readShortInt();
            readTag(&Body);
            break ;

          case ATTRIBUTE_IS_NOT_OWNED:
          case INFORM_ATTRIBUTE_OWNERSHIP:
            object = Body.readLongInt();
            attribute = Body.readShortInt();
            federate = Body.readShortInt();
            break ;

          case NEGOTIATED_ATTRIBUTE_OWNERSHIP_DIVESTITURE:
          case REQUEST_ATTRIBUTE_OWNERSHIP_ASSUMPTION:
          case ATTRIBUTE_OWNERSHIP_ACQUISITION:
          case REQUEST_ATTRIBUTE_OWNERSHIP_RELEASE:
          case ATTRIBUTE_OWNERSHIP_DIVESTITURE_NOTIFICATION:
            object = Body.readLongInt();
            handleArraySize = Body.readShortInt();
            readHandleArray(&Body);
            readTag(&Body);
            break ;

          case ATTRIBUTE_OWNERSHIP_ACQUISITION_IF_AVAILABLE:
          case ATTRIBUTE_OWNERSHIP_ACQUISITION_NOTIFICATION:
          case ATTRIBUTE_OWNERSHIP_UNAVAILABLE:
          case UNCONDITIONAL_ATTRIBUTE_OWNERSHIP_DIVESTITURE:
          case CANCEL_NEGOTIATED_ATTRIBUTE_OWNERSHIP_DIVESTITURE:
          case ATTRIBUTE_OWNERSHIP_RELEASE_RESPONSE:
          case CANCEL_ATTRIBUTE_OWNERSHIP_ACQUISITION:
          case CONFIRM_ATTRIBUTE_OWNERSHIP_ACQUISITION_CANCELLATION:
            object = Body.readLongInt();
            handleArraySize = Body.readShortInt();
            readHandleArray(&Body);
            break ;

	  case DDM_ASSOCIATE_REGION:
	    object = Body.readLongInt();
	    region = Body.readLongInt();
	    boolean = Boolean(Body.readShortInt());
	    handleArraySize = Body.readShortInt();
            readHandleArray(&Body);
	    break ;

	  case DDM_SUBSCRIBE_ATTRIBUTES:
	    objectClass = Body.readLongInt();
	    region = Body.readLongInt();
	    boolean = Boolean(Body.readShortInt());
	    handleArraySize = Body.readShortInt();
            readHandleArray(&Body);
	    break ;

	  case DDM_UNASSOCIATE_REGION:
	    object = Body.readLongInt();
	    region = Body.readLongInt();
	    break ;

	  case DDM_UNSUBSCRIBE_ATTRIBUTES:	    
	    object = Body.readLongInt();
	    region = Body.readLongInt();
	    break ;

	  case DDM_SUBSCRIBE_INTERACTION:
	  case DDM_UNSUBSCRIBE_INTERACTION:
	    interactionClass = Body.readLongInt();
	    region = Body.readLongInt();
	    boolean = Boolean(Body.readShortInt());
	    break ;
	    
          case GET_ATTRIBUTE_SPACE_HANDLE:
            objectClass = Body.readLongInt();
            attribute = Body.readLongInt();
            space = Body.readLongInt();
            break ;

          case DDM_CREATE_REGION:
            space = Body.readLongInt();
            number = Body.readLongInt();
            region = Body.readLongInt();
            break ;

          case GET_INTERACTION_SPACE_HANDLE:
            interactionClass = Body.readLongInt();
            space = Body.readLongInt();
            break ;

            // --- MessageJ_R_Struct --

          case JOIN_FEDERATION_EXECUTION:
            readFederationName(&Body);
            readFederateName(&Body);
            break ;

            // --- MessageO_I_Struct ---

          case PUBLISH_OBJECT_CLASS:
          case SUBSCRIBE_OBJECT_CLASS_ATTRIBUTES:
            readHandleArray(&Body);
            break ;

          case REGISTER_OBJECT_INSTANCE:
            object = Body.readLongInt();
            readName(&Body);
            break ;

          case UPDATE_ATTRIBUTE_VALUES:
          case REFLECT_ATTRIBUTE_VALUES:
            // B.c. object, Tag, HandleArray[], ValueArray[] and RAction.
            object = Body.readLongInt();
            readTag(&Body);
            readHandleArray(&Body);
            readValueArray(&Body);
            readResignAction(&Body);
            break ;

          case DISCOVER_OBJECT_INSTANCE:
          case DELETE_OBJECT_INSTANCE:
          case REMOVE_OBJECT_INSTANCE:
            // B.c. object, Tag, Label, RAction
            object = Body.readLongInt();
            readTag(&Body);
            readName(&Body); /*FAYET 25.07.01*/
            readLabel(&Body);
            readResignAction(&Body);
            break ;

          case GET_OBJECT_CLASS_HANDLE:
          case GET_OBJECT_CLASS_NAME:
          case GET_ATTRIBUTE_HANDLE:
          case GET_ATTRIBUTE_NAME:
            // B.c. Name(and attribute)
            readName(&Body);
            attribute = Body.readShortInt();
            break ;

          case GET_SPACE_HANDLE:
          case GET_SPACE_NAME:
            this->readName(&Body);
            this->space = Body.readLongInt();
            break ;

          case GET_DIMENSION_HANDLE:
          case GET_DIMENSION_NAME:
            this->readName(&Body);
            this->dimension = Body.readLongInt();
            this->space = Body.readLongInt();
            break ;

          case SEND_INTERACTION:
          case RECEIVE_INTERACTION:
            // B.c. Tag, HandleArray[], ValueArray[], RAction
            readTag(&Body);
            readHandleArray(&Body);
            readValueArray(&Body);
            readResignAction(&Body);
            break ;

          case GET_INTERACTION_CLASS_HANDLE:
          case GET_INTERACTION_CLASS_NAME:
          case GET_PARAMETER_HANDLE:
          case GET_PARAMETER_NAME:
            // Body contains Name and ParamHandle
            readName(&Body);
            parameter = Body.readShortInt();
            break ;

            // --- MessageT_O_Struct, Body not empty ---

          case CHANGE_ATTRIBUTE_TRANSPORTATION_TYPE:
          case CHANGE_ATTRIBUTE_ORDER_TYPE:
            // B.c. object, HandleArray
            object = Body.readLongInt();
            readHandleArray(&Body);
            break ;

	  case DDM_MODIFY_REGION:
	    readExtents(Body);
	    break ;
	    
            // -- Default Handler --

          default:
            D.Out(pdExcept, "Unknown Type %d in ReadBody.", header.type);
            throw RTIinternalError("Message: Unknown Type for Body(Read).");
        }
    }
}

// ----------------------------------------------------------------------------
/*! Read a Header from a socket, and process it to read its content. Return
  RTI_TRUE if the ReadBody Method has to be called.
*/
Boolean
Message::readHeader(SocketUN *socket)
{
    // 1- Read Header from Socket
    socket->receive((void *) &header, sizeof(MessageHeader));

    // 2- Parse Header(Static Part)
    type = header.type ;
    exception = header.exception ;

    // If the message carry an exception, the Body will only contain the
    // exception reason.

    if (exception != e_NO_EXCEPTION)
        return RTI_TRUE ;

    // 2- Parse Header according to its type(Variable Part)
    // NULL, UAV and SendInteraction are the most common ones.

    switch(type) {

        // --- No Variable Part, Body not empty ---

      case CREATE_FEDERATION_EXECUTION: // Body contains NomFederation
      case DESTROY_FEDERATION_EXECUTION: // Body contains NomFedere
      case REGISTER_FEDERATION_SYNCHRONIZATION_POINT: // Body contains Label
      case SYNCHRONIZATION_POINT_REGISTRATION_SUCCEEDED:
      case ANNOUNCE_SYNCHRONIZATION_POINT:
      case SYNCHRONIZATION_POINT_ACHIEVED: // Body contains Label
      case FEDERATION_SYNCHRONIZED:
      case INITIATE_FEDERATE_SAVE:
      case IS_ATTRIBUTE_OWNED_BY_FEDERATE:// B.c. object, attribute and Tag.
      case QUERY_ATTRIBUTE_OWNERSHIP: // B.c. object and attribute.
      case ATTRIBUTE_IS_NOT_OWNED:
      case INFORM_ATTRIBUTE_OWNERSHIP:
      case NEGOTIATED_ATTRIBUTE_OWNERSHIP_DIVESTITURE:
      case REQUEST_ATTRIBUTE_OWNERSHIP_ASSUMPTION:
      case ATTRIBUTE_OWNERSHIP_ACQUISITION_IF_AVAILABLE:
      case ATTRIBUTE_OWNERSHIP_ACQUISITION_NOTIFICATION:
      case ATTRIBUTE_OWNERSHIP_UNAVAILABLE:
      case UNCONDITIONAL_ATTRIBUTE_OWNERSHIP_DIVESTITURE:
      case ATTRIBUTE_OWNERSHIP_ACQUISITION:
      case REQUEST_ATTRIBUTE_OWNERSHIP_RELEASE:
      case ATTRIBUTE_OWNERSHIP_DIVESTITURE_NOTIFICATION:
      case CANCEL_NEGOTIATED_ATTRIBUTE_OWNERSHIP_DIVESTITURE:
      case ATTRIBUTE_OWNERSHIP_RELEASE_RESPONSE:
      case CANCEL_ATTRIBUTE_OWNERSHIP_ACQUISITION:
      case CONFIRM_ATTRIBUTE_OWNERSHIP_ACQUISITION_CANCELLATION:
      case GET_ATTRIBUTE_SPACE_HANDLE:
      case GET_INTERACTION_SPACE_HANDLE:
      case DDM_CREATE_REGION:
      case REQUEST_FEDERATION_RESTORE:
      case REQUEST_FEDERATION_RESTORE_SUCCEEDED:
      case REQUEST_FEDERATION_RESTORE_FAILED:
      case FEDERATE_RESTORE_COMPLETE:
      case FEDERATE_RESTORE_NOT_COMPLETE:
      case FEDERATION_RESTORED:
      case FEDERATION_NOT_RESTORED:
      case FEDERATION_RESTORE_BEGUN:
      case DDM_ASSOCIATE_REGION:
      case DDM_UNASSOCIATE_REGION:
      case DDM_SUBSCRIBE_ATTRIBUTES:
      case DDM_UNSUBSCRIBE_ATTRIBUTES:
      case DDM_SUBSCRIBE_INTERACTION:
      case DDM_UNSUBSCRIBE_INTERACTION:
        break ;

        // --- MessageJ_R_Struct --

      case RESIGN_FEDERATION_EXECUTION: // No Body
        resignAction = header.VP.J_R.action ;
        break ;

      case INITIATE_FEDERATE_RESTORE:
      case JOIN_FEDERATION_EXECUTION: // Body contains NomFederation&NomFedere
        federate = header.VP.J_R.federate ;
        break ;

        // --- MessageO_I_Struct, No Body ---

      case UNPUBLISH_OBJECT_CLASS:
      case UNSUBSCRIBE_OBJECT_CLASS:
        objectClass = header.VP.O_I.handle ;
        break ;


      case PUBLISH_INTERACTION_CLASS:
      case UNPUBLISH_INTERACTION_CLASS:
      case SUBSCRIBE_INTERACTION_CLASS:
      case UNSUBSCRIBE_INTERACTION_CLASS:
      case TURN_INTERACTIONS_ON:
      case TURN_INTERACTIONS_OFF:
        interactionClass = header.VP.O_I.handle ;
        break ;

        // --- MessageO_I_Struct, Body not Empty ---

      case PUBLISH_OBJECT_CLASS: // Body contains HandleArray
      case SUBSCRIBE_OBJECT_CLASS_ATTRIBUTES: // Body contains HandleArray
      case REGISTER_OBJECT_INSTANCE: // Body contains object
      case UPDATE_ATTRIBUTE_VALUES: // B.c. object, Tag, HandleArray[]
        // ValueArray[] and resignAction.
      case DISCOVER_OBJECT_INSTANCE: // B.c. object, Tag and resignAction
      case REFLECT_ATTRIBUTE_VALUES: // B.c. object, Tag, HandleArray[]
        // and ValueArray[]
      case DELETE_OBJECT_INSTANCE: // Body contains object, Tag.
      case REMOVE_OBJECT_INSTANCE: // B.c. object, Tag, Label&resignAction
      case GET_OBJECT_CLASS_HANDLE: // Body contains Name
      case GET_OBJECT_CLASS_NAME: // Body contains Name
      case GET_ATTRIBUTE_HANDLE: // B.c. Name and attribute.
      case GET_ATTRIBUTE_NAME: // B.c. Name and attribute.
        objectClass = header.VP.O_I.handle ;
        handleArraySize = header.VP.O_I.size ;
        date = header.VP.O_I.date ;
        break ;

      case SEND_INTERACTION: // B.c. Tag, HandleArray[], ValueArray[]
      case RECEIVE_INTERACTION: // B.c. Tag, HandleArray[], ValueArray[], resignAction
      case GET_INTERACTION_CLASS_HANDLE: // Body contains Name
      case GET_INTERACTION_CLASS_NAME: // Body contains Name
      case GET_PARAMETER_HANDLE: // Body contains Name and parameter
      case GET_PARAMETER_NAME: // Body contains Name and parameter
        interactionClass = header.VP.O_I.handle ;
        handleArraySize = header.VP.O_I.size ;
        date = header.VP.O_I.date ;
        break ;

      case GET_SPACE_HANDLE:
      case GET_SPACE_NAME:
      case GET_DIMENSION_NAME:
      case GET_DIMENSION_HANDLE:
        this->space = header.VP.O_I.handle ;
        handleArraySize = header.VP.O_I.size ;
        date = header.VP.O_I.date ;
        break ;

        // --- ReqIDStruct, No Body ---

      case REQUEST_ID:
        idCount = header.VP.ReqID.count ;
        firstId = header.VP.ReqID.first ;
        lastId = header.VP.ReqID.last ;
        break ;

        // --- MessageT_O_Struct, No Body ---

      case CHANGE_INTERACTION_TRANSPORTATION_TYPE:
      case CHANGE_INTERACTION_ORDER_TYPE:
        interactionClass = header.VP.T_O.handle ;
        transport = header.VP.T_O.transport ;
        order = header.VP.T_O.order ;
        break ;

        // Message_DDM, no body
      case DDM_DELETE_REGION:
        region = header.VP.ddm.region ;
        break ;

	// Message_DDM, Body
      case DDM_MODIFY_REGION:
	region = header.VP.ddm.region ;
	break ;	
	
        // --- MessageT_O_Struct, Body not empty ---

      case CHANGE_ATTRIBUTE_TRANSPORTATION_TYPE: // B.c. object, HandleArray.
      case CHANGE_ATTRIBUTE_ORDER_TYPE: // B.c. object, HandleArray.
        handleArraySize = header.VP.T_O.size ;
        transport = header.VP.T_O.transport ;
        order = header.VP.T_O.order ;
        break ;

        // --- TimeStruct, No Body ---
        // case REQUEST_FEDERATION_TIME:
      case QUERY_LBTS:
      case QUERY_FEDERATE_TIME:
      case TIME_ADVANCE_REQUEST:
      case NEXT_EVENT_REQUEST:
      case TIME_ADVANCE_GRANT:
        date = header.VP.time.date ;
        break ;

      case MODIFY_LOOKAHEAD:
      case QUERY_LOOKAHEAD:
        lookahead = header.VP.time.date ;
        break ;

      case ENABLE_TIME_REGULATION:
      case DISABLE_TIME_REGULATION:
      case ENABLE_TIME_CONSTRAINED:
      case DISABLE_TIME_CONSTRAINED:
      case TICK_REQUEST:
        boolean = header.VP.time.mode ;
        break ;


        // -- Default Handler --

      default:
        D.Out(pdExcept, "Unknown type %d in ReadHeader.", header.type);
        throw RTIinternalError("Message: Received unknown Header type.");
    }

    // 4- If Header.bodySize is not 0, return RTI_TRUE, else RTI_FALSE

    if (header.bodySize == 0)
        return RTI_FALSE ;
    else
        return RTI_TRUE ;
}


// ---------------
// -- ReadLabel --
// ---------------

void
Message::readHandleArray(MessageBody *Body)
{
    Body->readBlock((char *) handleArray,
                    handleArraySize * sizeof(AttributeHandle));
}


// ---------------
// -- ReadLabel --
// ---------------

void
Message::readLabel(MessageBody *Body)
{
    Body->readString(label, MAX_USER_TAG_LENGTH);
}


// --------------
// -- ReadName --
// --------------

void
Message::readName(MessageBody *Body)
{
    Body->readString(name, MAX_USER_TAG_LENGTH);
}


// -----------------------
// -- ReadNomFederation --
// -----------------------

void
Message::readFederationName(MessageBody *Body)
{
    Body->readString(federationName, MAX_FEDERATION_NAME_LENGTH);
}


// -------------------
// -- ReadNomFedere --
// -------------------

void
Message::readFederateName(MessageBody *Body)
{
    Body->readString(federateName, MAX_FEDERATE_NAME_LENGTH);
}


// ---------------------
// -- ReadresignAction --
// ---------------------

void
Message::readResignAction(MessageBody*)
{
    // BUG: Should do something.
}


// -------------
// -- ReadTag --
// -------------

void
Message::readTag(MessageBody *Body)
{
    Body->readString(tag, MAX_USER_TAG_LENGTH);
}

// ----------------------------------------------------------------------------
//! readValueArray.
void
Message::readValueArray(MessageBody *Body)
{
    for (int i = 0 ; i < handleArraySize ; i ++) {
        Body->readString(valueArray[i], MAX_BYTES_PER_VALUE);
    }
}

// ----------------------------------------------------------------------------
//! Write NetworkMessage Objects to Socket objects.
void
Message::write(SocketUN *socket)
    throw (NetworkError, NetworkSignal)
{
#ifdef USE_HEADER_AND_BODY

    bool result ;

    result = writeHeader(socket);
    if (result)
        writeBody(socket);

#else
    socket->send((void *) this, sizeof(Message));
#endif
}

// ----------------------------------------------------------------------------
/*! Prepare and write a Body to a socket, should be called after WriteHeader.
  The message is written onto the socket by WriteHeader if no body is
  required, or by WriteBody is a body has been required by WriteHeader.
*/
void
Message::writeBody(SocketUN *socket)
{
    MessageBody Body ;

    // 0- Copy the Header at the beginning of the Body, in order to
    // make a single Socket->Emettre call while sending both.
    // WARNING: As the Body size is not known yet, we will have to
    // change it in the copy also!
    Body.writeBlock((char *) &header, sizeof(MessageHeader));

    // If the message carry an exception, the Body will only contain the
    // exception reason.

    if (header.exception != e_NO_EXCEPTION) {
        Body.writeString(exceptionReason);
    }
    else {

        // 1- Prepare Body Structure according to Message type
        switch(header.type) {

            // --- No Variable Part, Body not empty ---

          case CREATE_FEDERATION_EXECUTION:
          case DESTROY_FEDERATION_EXECUTION:
            Body.writeString(federationName);
            break ;

          case REGISTER_FEDERATION_SYNCHRONIZATION_POINT:
          case ANNOUNCE_SYNCHRONIZATION_POINT:
          case REQUEST_FEDERATION_RESTORE_FAILED:
            Body.writeString(label);
            Body.writeString(tag);
            break ;

          case SYNCHRONIZATION_POINT_REGISTRATION_SUCCEEDED:
          case SYNCHRONIZATION_POINT_ACHIEVED:
          case FEDERATION_SYNCHRONIZED:
            Body.writeString(label);
            break ;

          case IS_ATTRIBUTE_OWNED_BY_FEDERATE:
          case QUERY_ATTRIBUTE_OWNERSHIP:
            // B.c. object, attribute, Tag
            Body.writeLongInt(object);
            Body.writeShortInt(attribute);
            Body.writeString(tag);
            break ;

          case ATTRIBUTE_IS_NOT_OWNED:
          case INFORM_ATTRIBUTE_OWNERSHIP:
            Body.writeLongInt(object);
            Body.writeShortInt(attribute);
            Body.writeShortInt(federate);
            break ;

          case NEGOTIATED_ATTRIBUTE_OWNERSHIP_DIVESTITURE:
          case REQUEST_ATTRIBUTE_OWNERSHIP_ASSUMPTION:
          case ATTRIBUTE_OWNERSHIP_ACQUISITION:
          case REQUEST_ATTRIBUTE_OWNERSHIP_RELEASE:
            Body.writeLongInt(object);
            Body.writeShortInt(handleArraySize);
            writeHandleArray(&Body);
            Body.writeString(tag);
            break ;

          case ATTRIBUTE_OWNERSHIP_ACQUISITION_IF_AVAILABLE:
          case ATTRIBUTE_OWNERSHIP_ACQUISITION_NOTIFICATION:
          case ATTRIBUTE_OWNERSHIP_UNAVAILABLE:
          case UNCONDITIONAL_ATTRIBUTE_OWNERSHIP_DIVESTITURE:
          case ATTRIBUTE_OWNERSHIP_DIVESTITURE_NOTIFICATION:
          case CANCEL_NEGOTIATED_ATTRIBUTE_OWNERSHIP_DIVESTITURE:
          case ATTRIBUTE_OWNERSHIP_RELEASE_RESPONSE:
          case CANCEL_ATTRIBUTE_OWNERSHIP_ACQUISITION:
          case CONFIRM_ATTRIBUTE_OWNERSHIP_ACQUISITION_CANCELLATION:
            Body.writeLongInt(object);
            Body.writeShortInt(handleArraySize);
            writeHandleArray(&Body);
            break ;

	  case DDM_ASSOCIATE_REGION:
	    Body.writeLongInt(object);
	    Body.writeLongInt(region);
	    Body.writeShortInt(boolean);
            Body.writeShortInt(handleArraySize);
            writeHandleArray(&Body);
	    break ;

	  case DDM_SUBSCRIBE_ATTRIBUTES:
	    Body.writeLongInt(objectClass);
	    Body.writeLongInt(region);
	    Body.writeShortInt(boolean);
            Body.writeShortInt(handleArraySize);
            writeHandleArray(&Body);
	    break ;

	  case DDM_UNASSOCIATE_REGION:
	    Body.writeLongInt(object);
	    Body.writeLongInt(region);
	    break ;

	  case DDM_UNSUBSCRIBE_ATTRIBUTES:
	    Body.writeLongInt(objectClass);
	    Body.writeLongInt(region);
	    break ;
	    
	  case DDM_SUBSCRIBE_INTERACTION:
	  case DDM_UNSUBSCRIBE_INTERACTION:
	    Body.writeLongInt(interactionClass);
	    Body.writeLongInt(region);
	    Body.writeShortInt(boolean);
	    break ;
	    
          case GET_ATTRIBUTE_SPACE_HANDLE:
            Body.writeLongInt(objectClass);
            Body.writeLongInt(attribute);
            Body.writeLongInt(space);
            break ;

          case DDM_CREATE_REGION:
            Body.writeLongInt(space);
            Body.writeLongInt(number);
            Body.writeLongInt(region);
            break ;

          case GET_INTERACTION_SPACE_HANDLE:
            Body.writeLongInt(interactionClass);
            Body.writeLongInt(space);
            break ;

            // --- MessageJ_R_Struct --

          case JOIN_FEDERATION_EXECUTION:
            Body.writeString(federationName);
            Body.writeString(federateName);
            break ;

            // --- MessageO_I_Struct ---

          case PUBLISH_OBJECT_CLASS:
          case SUBSCRIBE_OBJECT_CLASS_ATTRIBUTES:
            writeHandleArray(&Body);
            break ;

          case REGISTER_OBJECT_INSTANCE:
            Body.writeLongInt(object);
            Body.writeString(name);
            break ;

          case UPDATE_ATTRIBUTE_VALUES:
          case REFLECT_ATTRIBUTE_VALUES:
            // B.c. object, Tag, handleArray[], ValueArray[] and resignAction.
            Body.writeLongInt(object);
            Body.writeString(tag);
            writeHandleArray(&Body);
            writeValueArray(&Body);
            writeResignAction(&Body);
            break ;

          case DISCOVER_OBJECT_INSTANCE:
          case DELETE_OBJECT_INSTANCE:
          case REMOVE_OBJECT_INSTANCE:
            // B.c. object, Tag, label, resignAction
            Body.writeLongInt(object);
            Body.writeString(tag);
            Body.writeString(name);
            Body.writeString(label);
            writeResignAction(&Body);
            break ;

          case GET_OBJECT_CLASS_HANDLE:
          case GET_OBJECT_CLASS_NAME:
          case GET_ATTRIBUTE_HANDLE:
          case GET_ATTRIBUTE_NAME:
            // B.c. name(and attribute)
            Body.writeString(name);
            Body.writeShortInt(attribute);
            break ;

          case GET_SPACE_HANDLE:
          case GET_SPACE_NAME:
            Body.writeString(name);
            Body.writeLongInt(space);
            break ;

          case GET_DIMENSION_HANDLE:
          case GET_DIMENSION_NAME:
            Body.writeString(name);
            Body.writeLongInt(dimension);
            Body.writeLongInt(space);
            break ;

          case SEND_INTERACTION:
          case RECEIVE_INTERACTION:
            // B.c. Tag, HandleArray[], ValueArray[], resignAction
            Body.writeString(tag);
            writeHandleArray(&Body);
            writeValueArray(&Body);
            writeResignAction(&Body);
            break ;

          case GET_INTERACTION_CLASS_HANDLE:
          case GET_INTERACTION_CLASS_NAME:
          case GET_PARAMETER_HANDLE:
          case GET_PARAMETER_NAME:
            // Body contains name and parameter
            Body.writeString(name);
            Body.writeShortInt(parameter);
            break ;


            // --- MessageT_O_Struct, Body not empty ---

          case CHANGE_ATTRIBUTE_TRANSPORTATION_TYPE:
          case CHANGE_ATTRIBUTE_ORDER_TYPE:
            // B.c. object, HandleArray
            Body.writeLongInt(object);
            writeHandleArray(&Body);
            break ;

	  case DDM_MODIFY_REGION:
	    writeExtents(Body);
	    break ;
	    
            // -- Default Handler --

          default:
            D.Out(pdExcept, "Unknown type %d in WriteBody.", header.type);
            throw RTIinternalError("Message: Unknown type for Body.");

        }
    }

    // 2- Set Header.bodySize

    // Body Size does not include the copy of the Header!
    header.bodySize = Body.getLength() - sizeof(MessageHeader);

    // Put the real Body Size in the copy of the Header.
    ((MessageHeader *) Body.getBuffer())->bodySize = header.bodySize ;

    // 3- Write Header to socket, then write Body to socket.
    // socket->send((void *) &Header, sizeof(MessageHeader));
    socket->send((void *) Body.getBuffer(), Body.getLength());
}


// ----------------------
// -- WriteHandleArray --
// ----------------------

void Message::writeHandleArray(MessageBody *Body)
{
    Body->writeBlock((char *) handleArray,
                     handleArraySize * sizeof(AttributeHandle));
}

// ----------------------------------------------------------------------------
/*! Prepare and Write a Header to a Socket, and return RTI_TRUE if the
  WriteBody method has to be called.
  The message is written onto the socket by WriteHeader if no body is
  required, or by WriteBody is a body has been required by WriteHeader.
*/
bool
Message::writeHeader(SocketUN *socket)
{
    // 1- Clear Header
    memset((void *) &header, '\0', sizeof(MessageHeader));

    // 2- Fill Header(Static Part)
    header.type = this->type ;
    header.exception = this->exception ;

    // If the message carry an exception, the Body will only contain the
    // exception reason.

    if (exception != e_NO_EXCEPTION) {
        header.bodySize = 1 ;
        return true ;
    }

    // 3- Fill Header(Variable Part)[Sorted by Variable part type]
    // Note: Header.bodySize is not set to the actual Body size, but
    // to zero to indicate there is no Body, or 1 if a Body is needed.

    switch(this->type) {

        // --- No Variable Part, Body not empty ---

      case CREATE_FEDERATION_EXECUTION: // Body contains federationName
      case DESTROY_FEDERATION_EXECUTION: // Body contains federationName
      case REGISTER_FEDERATION_SYNCHRONIZATION_POINT: // Body contains Label
      case SYNCHRONIZATION_POINT_REGISTRATION_SUCCEEDED:
      case ANNOUNCE_SYNCHRONIZATION_POINT:
      case SYNCHRONIZATION_POINT_ACHIEVED: // Body contains Label
      case FEDERATION_SYNCHRONIZED:
      case IS_ATTRIBUTE_OWNED_BY_FEDERATE:// B.c. object, attribute and Tag.
      case QUERY_ATTRIBUTE_OWNERSHIP: // B.c. object and attribute.
      case ATTRIBUTE_IS_NOT_OWNED:
      case INFORM_ATTRIBUTE_OWNERSHIP:
      case NEGOTIATED_ATTRIBUTE_OWNERSHIP_DIVESTITURE:
      case REQUEST_ATTRIBUTE_OWNERSHIP_ASSUMPTION:
      case ATTRIBUTE_OWNERSHIP_ACQUISITION_IF_AVAILABLE:
      case ATTRIBUTE_OWNERSHIP_ACQUISITION_NOTIFICATION:
      case ATTRIBUTE_OWNERSHIP_UNAVAILABLE:
      case UNCONDITIONAL_ATTRIBUTE_OWNERSHIP_DIVESTITURE:
      case ATTRIBUTE_OWNERSHIP_ACQUISITION:
      case REQUEST_ATTRIBUTE_OWNERSHIP_RELEASE:
      case ATTRIBUTE_OWNERSHIP_DIVESTITURE_NOTIFICATION:
      case CANCEL_NEGOTIATED_ATTRIBUTE_OWNERSHIP_DIVESTITURE:
      case ATTRIBUTE_OWNERSHIP_RELEASE_RESPONSE:
      case CANCEL_ATTRIBUTE_OWNERSHIP_ACQUISITION:
      case CONFIRM_ATTRIBUTE_OWNERSHIP_ACQUISITION_CANCELLATION:
      case GET_ATTRIBUTE_SPACE_HANDLE:
      case GET_INTERACTION_SPACE_HANDLE:
      case DDM_CREATE_REGION:
      case DDM_ASSOCIATE_REGION:
      case DDM_UNASSOCIATE_REGION:
      case DDM_SUBSCRIBE_ATTRIBUTES:
      case DDM_UNSUBSCRIBE_ATTRIBUTES:
      case DDM_SUBSCRIBE_INTERACTION:
      case DDM_UNSUBSCRIBE_INTERACTION:
        header.bodySize = 1 ;
        break ;

      case REQUEST_FEDERATION_SAVE: // Body contains Label
        header.VP.O_I.date = date ;
        header.bodySize = 1 ;
        break ;

      case FEDERATE_SAVE_BEGUN:
      case FEDERATE_SAVE_COMPLETE:
      case FEDERATE_SAVE_NOT_COMPLETE:
      case FEDERATION_SAVED:
      case FEDERATION_NOT_SAVED:
      case REQUEST_FEDERATION_RESTORE:
      case REQUEST_FEDERATION_RESTORE_SUCCEEDED:
      case REQUEST_FEDERATION_RESTORE_FAILED:
      case FEDERATE_RESTORE_COMPLETE:
      case FEDERATE_RESTORE_NOT_COMPLETE:
      case FEDERATION_RESTORED:
      case FEDERATION_NOT_RESTORED:
      case FEDERATION_RESTORE_BEGUN:
        header.bodySize = 0 ;
        break ;

        // --- MessageJ_R_Struct --

      case RESIGN_FEDERATION_EXECUTION: // No Body
        header.VP.J_R.action = resignAction ;
        header.bodySize = 0 ;
        break ;

      case INITIATE_FEDERATE_RESTORE:
      case JOIN_FEDERATION_EXECUTION: // Body contains federationName&federateName
        header.VP.J_R.federate = federate ;
        header.bodySize = 1 ;
        break ;

        // --- MessageO_I_Struct, No Body ---

      case UNPUBLISH_OBJECT_CLASS:
      case UNSUBSCRIBE_OBJECT_CLASS:
        header.VP.O_I.handle = this->objectClass ;
        header.bodySize = 0 ;
        break ;

      case PUBLISH_INTERACTION_CLASS:
      case UNPUBLISH_INTERACTION_CLASS:
      case SUBSCRIBE_INTERACTION_CLASS:
      case UNSUBSCRIBE_INTERACTION_CLASS:
      case TURN_INTERACTIONS_ON:
      case TURN_INTERACTIONS_OFF:
        header.VP.O_I.handle = this->interactionClass ;
        header.bodySize = 0 ;
        break ;

        // --- MessageO_I_Struct, Body not Empty ---

      case PUBLISH_OBJECT_CLASS: // Body contains handleArray
      case SUBSCRIBE_OBJECT_CLASS_ATTRIBUTES: // Body contains handleArray
      case REGISTER_OBJECT_INSTANCE: // Body contains object
      case UPDATE_ATTRIBUTE_VALUES: // B.c. object, Tag, handleArray[]
        // ValueArray[] and resignAction.
      case DISCOVER_OBJECT_INSTANCE: // B.c. object, Tag and resignAction
      case REFLECT_ATTRIBUTE_VALUES: // B.c. object, Tag, handleArray[]
        // and ValueArray[]
      case DELETE_OBJECT_INSTANCE: // Body contains object, Tag.
      case REMOVE_OBJECT_INSTANCE: // B.c. object, Tag, label&resignAction
      case GET_OBJECT_CLASS_HANDLE: // Body contains name
      case GET_OBJECT_CLASS_NAME: // Body contains name
      case GET_ATTRIBUTE_HANDLE: // B.c. name and attribute.
      case GET_ATTRIBUTE_NAME: // B.c. name and attribute.
        header.VP.O_I.handle = objectClass ;
        header.VP.O_I.size = handleArraySize ;
        header.VP.O_I.date = date ;
        header.bodySize = 1 ;
        break ;


      case SEND_INTERACTION: // B.c. Tag, handleArray[], ValueArray[]
      case RECEIVE_INTERACTION: // B.c. Tag, handleArray[],
        // ValueArray[], resignAction
      case GET_INTERACTION_CLASS_HANDLE: // Body contains name
      case GET_INTERACTION_CLASS_NAME: // Body contains name
      case GET_PARAMETER_HANDLE: // Body contains name and parameter
      case GET_PARAMETER_NAME: // Body contains name and parameter
        header.VP.O_I.handle = interactionClass ;
        header.VP.O_I.size = handleArraySize ;
        header.VP.O_I.date = date ;
        header.bodySize = 1 ;
        break ;

      case GET_SPACE_HANDLE:
      case GET_SPACE_NAME:
      case GET_DIMENSION_HANDLE:
      case GET_DIMENSION_NAME:
        header.VP.O_I.handle = space ;
        header.VP.O_I.size = handleArraySize ;
        header.VP.O_I.date = date ;
        header.bodySize = 1 ;
        break ;

        // --- ReqIDStruct, No Body ---

      case REQUEST_ID:
        header.VP.ReqID.count = idCount ;
        header.VP.ReqID.first = firstId ;
        header.VP.ReqID.last = lastId ;
        header.bodySize = 0 ;
        break ;

        // --- MessageT_O_Struct, No Body ---

      case CHANGE_INTERACTION_TRANSPORTATION_TYPE:
      case CHANGE_INTERACTION_ORDER_TYPE:
        header.VP.T_O.handle = interactionClass ;
        header.VP.T_O.transport = transport ;
        header.VP.T_O.order = order ;
        header.bodySize = 0 ;
        break ;

        // Message_DDM, no body
      case DDM_DELETE_REGION:
        header.VP.ddm.region = region ;
        header.bodySize = 0 ;
        break ;

	// Message_DDM, Body
      case DDM_MODIFY_REGION:
	header.VP.ddm.region = region ;
	header.bodySize = 1 ;
	break ;

        // --- MessageT_O_Struct, Body not empty ---

      case CHANGE_ATTRIBUTE_TRANSPORTATION_TYPE: // B.c. object, handleArray.
      case CHANGE_ATTRIBUTE_ORDER_TYPE: // B.c. object, handleArray.
        header.VP.T_O.size = handleArraySize ;
        header.VP.T_O.transport = transport ;
        header.VP.T_O.order = order ;
        header.bodySize = 1 ;
        break ;

        // --- TimeStruct, No Body ---
        // case REQUEST_FEDERATION_TIME:
      case QUERY_LBTS:
      case QUERY_FEDERATE_TIME:
      case TIME_ADVANCE_REQUEST:
      case NEXT_EVENT_REQUEST:
      case TIME_ADVANCE_GRANT:
        header.VP.time.date = date ;
        header.bodySize = 0 ;
        break ;


      case MODIFY_LOOKAHEAD:
      case QUERY_LOOKAHEAD:
        header.VP.time.date = lookahead ;
        header.bodySize = 0 ;
        break ;

      case ENABLE_TIME_REGULATION:
      case DISABLE_TIME_REGULATION:
      case ENABLE_TIME_CONSTRAINED:
      case DISABLE_TIME_CONSTRAINED:
      case TICK_REQUEST:
        header.VP.time.mode = boolean ;
        header.bodySize = 0 ;
        break ;

        // -- Default Handler --
      default:
        D.Out(pdExcept, "Unknown type %d in WriteHeader.", header.type);
        throw RTIinternalError("Message: Unknown type for Header.");

    }

    // 4- If Header.bodySize = 0, send message and return RTI_FALSE,
    // Else send nothing(will be done by WriteBody), and return RTI_TRUE.

    if (header.bodySize == 0) {
        socket->send((void *) &header, sizeof(MessageHeader));
        return false ;
    }
    else
        return true ;

}


// ---------------------
// -- WriteresignAction --
// ---------------------

void
Message::writeResignAction(MessageBody*)
{
    // BUG: Should do something.
}


// ---------------------
// -- WriteValueArray --
// ---------------------

void
Message::writeValueArray(MessageBody *Body)
{
    for (int i = 0 ; i < handleArraySize ; i ++) {
        Body->writeString(valueArray[i]);
    }
}

// ----------------------------------------------------------------------------
// writeExtents
//
void
Message::writeExtents(MessageBody &body)
{
    long n = extents ? extents->size() : 0 ;

    for (long i = 0 ; i < n ; ++i) {
	Extent *e = (*extents)[i] ;
	long m = e->getNumberOfRanges();
	body.writeLongInt(m);
	for (long j = 0 ; j < m ; ++j) {
	    body.writeLongInt(e->getRangeLowerBound(j));
	    body.writeLongInt(e->getRangeUpperBound(j));
	}
    }
}

// ----------------------------------------------------------------------------
// readExtents
//
void
Message::readExtents(MessageBody &body)
{
    if (extents) {
	for (vector<Extent *>::iterator i = extents->begin();
	     i != extents->end(); ++i) {
	    delete *i ;
	}
	delete extents ;

    }
    extents = new vector<Extent *>();
    
    long n = body.readLongInt();
    for (long i = 0 ; i < n ; ++i) {
	long m = body.readLongInt();
	Extent *e = new Extent(m);
	for (long j = 0 ; j < m ; ++j) {
	    e->setRangeLowerBound(j, body.readLongInt());
	    e->setRangeUpperBound(j, body.readLongInt());
	}
	extents->push_back(e);
    }
}

} // namespace certi

// $Id: Message_RW.cc,v 3.15 2003/07/09 16:00:59 breholee Exp $
