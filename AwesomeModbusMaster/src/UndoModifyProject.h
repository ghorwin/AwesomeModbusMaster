#ifndef UndoModifyProjectH
#define UndoModifyProjectH

#include <AMM_Config.h>

#include "UndoCommandBase.h"

class UndoModifyProject : public UndoCommandBase {
	Q_DECLARE_TR_FUNCTIONS(UndoModifyProject)
public:
	UndoModifyProject(const QString & label, const AMM::Config & newConfig);

	virtual void undo();
	virtual void redo();

private:

	/*! Cache for entire project/config data (this might be large!). */
	AMM::Config	m_config;
};

#endif // UndoModifyProjectH
