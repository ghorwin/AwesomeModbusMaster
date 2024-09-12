#include "AMM_Config.h"
#include "AMM_KeywordList.h"

#include <algorithm>
#include <set>

#include <tinyxml.h>

namespace AMM {

/*! Test function that checks that all objects in the given vector have different m_id parameters. */
template <typename T>
void checkForUniqueIDs(const std::vector<T> & vec, const char * const typeIDString) {
	FUNCID(NANDRAD::checkForUniqueIDs);

	std::set<unsigned int> usedIDs;

	for (const T & t : vec) {
		if (usedIDs.find(t.m_id) != usedIDs.end())
			throw IBK::Exception(IBK::FormatString("Duplicate ID #%1 in list of type '%2'.")
								 .arg(t.m_id).arg(typeIDString), FUNC_ID);
		usedIDs.insert(t.m_id);
	}
}


void Config::readXML(const QString & filename) {
	FUNCID(Config::readXML);

//	TiXmlDocument doc;
//	TiXmlElement * xmlElem = openXMLFile(m_placeholders, filenamePath, "AwesomeModbusMaster", doc);
//	if (!xmlElem)
//		return; // empty project, this means we are using only defaults

//	// we read our subsections from this handle
//	TiXmlHandle xmlRoot = TiXmlHandle(xmlElem);

//	// Directory Placeholders
//	xmlElem = xmlRoot.FirstChild("DirectoryPlaceholders").Element();
//	if (xmlElem) {
//		readDirectoryPlaceholdersXML(xmlElem);
//		// check for duplicate sections
//		xmlElem = xmlRoot.Child("DirectoryPlaceholders", 1).Element();
//		if (xmlElem != nullptr) {
//			throw IBK::Exception(IBK::FormatString("Duplicate section 'DirectoryPlaceholders'."), FUNC_ID);
//		}
//	}

//	// add the project directory to the placeholders map
//	m_placeholders[IBK::PLACEHOLDER_PROJECT_DIR] = filenamePath.parentPath();

//	try {
//		xmlElem = xmlRoot.FirstChild("Config").Element();
//		if (xmlElem) {
//			readXMLPrivate(xmlElem);
//		}
//	}
//	catch (IBK::Exception & ex) {
//		throw IBK::Exception(ex, IBK::FormatString("Error reading project '%1'.").arg(filename), FUNC_ID);
//	}

//	// check uniqueness of all IDs in all separate id spaces
//	// Note: all objects in the ID-based object lists have an m_id data member. Hence, we can implement
//	//       the check in a template function.

//	checkForUniqueIDs(m_materials, "Material");
//	checkForUniqueIDs(m_constructionTypes, "ConstructionType");
//	checkForUniqueIDs(m_zones, "Zone");
//	checkForUniqueIDs(m_constructionInstances, "ConstructionInstance");

//	if(!m_hydraulicNetworks.empty()) {
//		// put all flow elements into a common id space
//		std::vector<HydraulicNetworkElement> flowElems;
//		// check all networks
//		for(const HydraulicNetwork &nw : m_hydraulicNetworks) {
//			checkForUniqueIDs(nw.m_components, "HydraulicNetworkComponent");
//			checkForUniqueIDs(nw.m_pipeProperties, "HydraulicNetworkPipeProperties");
//			// at the moment just store elements into vector
//			if(!flowElems.empty())
//				flowElems.insert(flowElems.end(), nw.m_elements.begin(), nw.m_elements.end());
//			else
//				flowElems = nw.m_elements;
//		}
//		// enforce a unique id for flow elements of all networks
//		checkForUniqueIDs(flowElems,"HydraulicNetworkElement");
//	}

//	// we check for duplicate object lists here, because these are referenced by name, rather than ID
//	checkForUniqueNames(m_outputs.m_grids, "OutputGrid");
//	checkForUniqueNames(m_objectLists, "ObjectList");


//	// The following objects share a common ID space and must be unique:
//	// - Zone
//	// - ConstructionInstance
//	// - EmbeddedObjects
//	// - Location.Sensors
//	std::set<unsigned int> usedIDs;

//	try {
//		// check all zone ids against each other and against
//		// all entries in usedIDs container
//		checkForUniqueGeometryIDs(m_zones, usedIDs);
//	}
//	catch (IBK::Exception & ex) {
//		throw IBK::Exception(ex, "Zone ID was previously used (Zones, Constructions, Sensors share same ID space).", FUNC_ID);
//	}
//	try {
//		// check all constructionInstances for unique ids and store
//		// it in usedIDs container
//		checkForUniqueGeometryIDs(m_constructionInstances, usedIDs);
//		// loop through all constructions and check for unique ids of
//		// embedded objects
//		for(const ConstructionInstance &conInstance : m_constructionInstances) {
//			checkForUniqueGeometryIDs(conInstance.m_embeddedObjects, usedIDs);
//		}
//	}
//	catch (IBK::Exception & ex) {
//		throw IBK::Exception(ex, "Construction ID was previously used (Zones, Constructions, Sensors share same ID space).", FUNC_ID);
//	}
//	try {
//		// check all sensor ids against each other and against
//		// all entries in usedIDs container
//		checkForUniqueGeometryIDs(m_location.m_sensors, usedIDs);
//	}
//	catch (IBK::Exception & ex) {
//		throw IBK::Exception(ex, "Sensor ID was previously used (Zones, Constructions, Sensors share same ID space).", FUNC_ID);
//	}

//	// Note:
//	// - the check for duplicate output definitions is done during output initialization

//	// uniqueness check for models
//	m_models.checkForUniqueIDs();
}


void Config::writeXML(const QString & filename) const {
	TiXmlDocument doc;
	TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "UTF-8", "" );
	doc.LinkEndChild( decl );

	TiXmlElement * root = new TiXmlElement( "AwesomeModbusMaster" );
	doc.LinkEndChild(root);

//	root->SetAttribute("fileVersion", VERSION);

//	// update interface-zone comment
//	for (ConstructionInstance & con : const_cast<std::vector<ConstructionInstance>&>(m_constructionInstances)) {
//		con.m_interfaceA.updateComment(m_zones);
//		con.m_interfaceB.updateComment(m_zones);
//	}

//	writeDirectoryPlaceholdersXML(root);
//	writeXMLPrivate(root);

	doc.SaveFile( filename.toStdString().c_str() );
}



} // namespace AMM
