// -*- mode:C++ ; tab-width:4 ; c-basic-offset:4 ; indent-tabs-mode:nil -*-
// ----------------------------------------------------------------------------
// CERTI - HLA RunTime Infrastructure
// Copyright (C) 2003  ONERA
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
// $Id: XmlParser.hh,v 3.11 2007/08/31 12:47:40 erk Exp $
// ----------------------------------------------------------------------------

#ifndef _CERTI_XML_PARSER_HH
#define _CERTI_XML_PARSER_HH

// Project
#include "RootObject.hh"
#include "ObjectClass.hh"
#include "Interaction.hh"

// Libraries
#ifdef HAVE_XML
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#endif

// Standard libraries
#include <string>

namespace certi {

class CERTI_EXPORT XmlParser
{
public:
    XmlParser(RootObject*);
    RootObject* parse(std::string);
    static bool exists(void);
    
#if HAVE_XML
	class CleanXmlGetProp {
		public:
		CleanXmlGetProp(_xmlNode* node, const xmlChar* propName) {
			prop = xmlGetProp(node,propName);
		}
		operator const char*() {
			return reinterpret_cast<const char*>(prop);
		}
		~CleanXmlGetProp(){
			xmlFree(prop);
		}
		private:
			xmlChar* prop;
	};
#endif
private:
    void parseClass(ObjectClass *);
    void parseInteraction(Interaction *);
    void parseRoutingSpace(void);

    int freeObjectClassHandle ;
    int freeInteractionClassHandle ;
    int freeAttributeHandle ;
    int freeParameterHandle ;
    int freeSpaceHandle ;

    std::string filename ;
    RootObject* root ;
    
#ifdef HAVE_XML
    xmlDocPtr doc ;
    xmlNodePtr cur ;    
#endif 
};

} // namespace certi

#endif // _CERTI_XML_PARSER_HH

// $Id: XmlParser.hh,v 3.11 2007/08/31 12:47:40 erk Exp $
