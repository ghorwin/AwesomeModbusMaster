#include "UndoModifyProject.h"
#include "ProjectHandler.h"

UndoModifyProject::UndoModifyProject(const QString & label, const AMM::Config & newConfig) :
	m_config(newConfig)
{
	setText( label );
}


void UndoModifyProject::undo() {

	// exchange Project
	std::swap( theProject(), m_config );

	// tell project that the grid has changed
	ProjectHandler::instance().setModified( ProjectHandler::AllModified);
}


void UndoModifyProject::redo() {
	undo();
}
