/*	SIM-VICUS - Building and District Energy Simulation Tool.

	Copyright (c) 2020-today, Institut für Bauklimatik, TU Dresden, Germany

	Primary authors:
	  Andreas Nicolai  <andreas.nicolai -[at]- tu-dresden.de>
	  Dirk Weiss  <dirk.weiss -[at]- tu-dresden.de>
	  Stephan Hirth  <stephan.hirth -[at]- tu-dresden.de>
	  Hauke Hirsch  <hauke.hirsch -[at]- tu-dresden.de>

	  ... all the others from the SIM-VICUS team ... :-)

	This program is part of SIM-VICUS (https://github.com/ghorwin/SIM-VICUS)

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
*/

#include "ProjectHandler.h"

#include <QStringList>
#include <QString>
#include <QMessageBox>
#include <QFileDialog>
#include <QWizard>
#include <QTextEdit>
#include <QLayout>
#include <QLabel>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QInputDialog>
#include <QTimer>
#include <QCryptographicHash>
#include <QPushButton>


#include <IBK_Exception.h>
#include <IBK_FileUtils.h>
#include <IBK_Path.h>
#include <IBK_assert.h>
#include <IBK_messages.h>

#include "AMM_Config.h"
#include "AMM_KeywordList.h"
#include "Directories.h"

#include "Settings.h"
#include "Constants.h"
#include "LogFileDialog.h"
#include "UndoModifyProject.h"
#include "MainWindow.h"

ProjectHandler * ProjectHandler::m_self = nullptr;

ModificationInfo::~ModificationInfo() {
}

ProjectHandler & ProjectHandler::instance() {
	Q_ASSERT_X(m_self != nullptr, "[ProjectHandler::instance]",
		"You must not access ProjectHandler::instance() when the is no ProjectHandler "
		"instance (anylonger).");
	return *m_self;
}


ProjectHandler::ProjectHandler() :
	m_project(nullptr),
	m_modified(false)
{
	IBK_ASSERT(m_self == nullptr);
	m_self = this;
}


ProjectHandler::~ProjectHandler( ){
	// free owned project, if any
	delete m_project;
	m_self = nullptr;
}


bool ProjectHandler::newProject(AMM::Config * project) {
	createProject();
	if (project != nullptr) {
		*m_project = *project; // copy over project
	}
	else {
		// add default data
	}

	// once the project has been read, perform "post-read" actions
	bool have_modified_project = false;

	setModified(AllModified);

	// this will clear the modified flag again (since we just read the project) except if we had made some automatic
	// fixes above
	m_modified = have_modified_project;

	// signal UI that we now have a project
	emit updateActions();

	// issue a call to user-dialog fixes/adjustments
	QTimer::singleShot(0, this, SIGNAL(fixProjectAfterRead()));

	return true;
}


bool ProjectHandler::closeProject(QWidget * parent) {

	// if no project exists, simply return true
	if (!isValid())
		return true;

	// ask user for confirmation to save, if project was modified
	if (isModified()) {

		// ask user for confirmation to save
		int result = QMessageBox::question(
				parent,
				tr("Save project before closing"),
				tr("Would you like to save the project before closing it?"),
				QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel
				);

		// user bails out?
		if (result == QMessageBox::Cancel)
			return false; // project not closed

		// saving requested by user
		if (result == QMessageBox::Save) {

			SaveResult res;

			// let user pick new filename
			if (m_projectFile.isEmpty())
				res = saveWithNewFilename(parent);
			else
				res = saveProject(parent, m_projectFile);

			// saving failed ?
			if (res != SaveOK)
				return false;

		}

	} // if (isModified())

	// remove autosave file upon close
	QFile::remove(m_projectFile + "~"); // remove backup file.

	// saving succeeded, now we can close the project
	destroyProject();

	// signal application that we have no longer a project, and thus all project-related
	// actions should be disabled
	emit updateActions();

	return true;
}


void ProjectHandler::loadProject(QWidget * parent, QString fileName,	bool silent) {
	FUNCID(ProjectHandler::loadProject);

	// we must not have a project loaded
	IBK_ASSERT(!isValid());

	bool autosaveLoaded = false;
	do {
		m_reload = false;

		// create a new project
		createProject();

		try {
			if (!read(parent, fileName))
				throw IBK::Exception(IBK::FormatString("Error reading project '%1'").arg(fileName.toStdString()), FUNC_ID);
			// project read successfully

		}
		catch (IBK::Exception & ex) {
			ex.writeMsgStackToError();
			if (!silent) {

				QMessageBox::critical(
							parent,
							tr("Error loading project"),
							tr("Error loading project file '%1', see error log file '%2' for details.")
							.arg(fileName)
							.arg(Directories::globalLogFile())
							);

				LogFileDialog dlg;
				dlg.setLogFile(Directories::globalLogFile(), fileName, true);
				dlg.exec();
			}
			// remove project again
			destroyProject();

			// Note: no need to emit updateActions() here since view state hasn't finished.
			if( !m_reload)
				return;
		}
	} while(m_reload);

	try {
		// once the project has been read, perform "post-read" actions
		bool have_modified_project = false;

		setModified(AllModified); // notify all views that the entire data has changed

		// this will clear the modified flag again (since we just read the project) except if we had made some automatic
		// fixes above
		m_modified = have_modified_project;

		// fix project file again if we have read a backup
		if (autosaveLoaded) {
			fileName.chop(1); // remove trailing ~
			m_projectFile.chop(1);
			// also update time stamp to that of original project, otherwise we get a modified warning here
			m_lastReadTime = QFileInfo(fileName).lastModified();
		}

		// this updates the filename in main menu, hence we need to fix the backup file name before this call
		emit updateActions();
	}
	catch (IBK::Exception & ex) {
		ex.writeMsgStackToError();

		// project data was incomplete, we show an error message and default to empty project
		if (!silent) {
			QMessageBox::critical(
					parent,
					tr("Error loading project"),
					tr("Data in project was missing/invalid, see error log '%1' for details.").arg(Directories::globalLogFile())
			);
		}
		// remove project again
		destroyProject();
		return;
	}

	// If we have read an old project file, the fileName and project().projectFile
	// will be different, because the extension was changed. In this case
	// we leave the modification state to modified and do not add the file to the
	// recent file list.

	// if the filenames are the same, we can savely assume that the project is not
	// modified and we add the file to the recent file list
	if (fileName == m_projectFile) {
		// add project file name to recent file list
		addToRecentFiles(fileName);
	} // if (fileName == m_projectFile)


	// issue a call to user-dialog fixes/adjustments
	QTimer::singleShot(0, this, SIGNAL(fixProjectAfterRead()));
}


void ProjectHandler::reloadProject(QWidget * parent) {
	QString projectFileName = projectFile();
	m_modified = false; // so that closeProject doesn't ask questions
	closeProject(parent);
	loadProject(parent, projectFileName, false); // emits updateActions() if project was successfully loaded
}


ProjectHandler::SaveResult ProjectHandler::saveWithNewFilename(QWidget * parent) {

	// determine default path from current project file
	QString currentPath = QFileInfo(m_projectFile).filePath();

	// ask user for filename
	QString filename = QFileDialog::getSaveFileName(
			parent,
			tr("Specify configuration file"),
			currentPath,
			tr("AMM config files (*.ammxml);;All files (*.*)"),
			nullptr,
			Settings::instance().m_dontUseNativeDialogs ? QFileDialog::DontUseNativeDialog : QFileDialog::Options()
		);


	if (filename.isEmpty()) return SaveCancelled; // cancelled

	QString fnamebase = QFileInfo(filename).baseName();
	if (fnamebase.isEmpty()) {
		QMessageBox::critical(parent, tr("Invalid file name"), tr("Please enter a valid file name!"));
		return SaveCancelled;
	}

	// remove previous backup file
	QFile::remove(m_projectFile + "~"); // remove backup file

	// relay to saveProject() which updates modified flag and emits corresponding signals.
	if (saveProject(parent, filename) != SaveOK)
		return SaveFailed; // saving failed

	return SaveOK;
}


ProjectHandler::SaveResult ProjectHandler::saveProject(QWidget * parent, const QString & fileName, bool addToRecentFilesList) {

	// check project file ending, if there is none append it
	QString fname = fileName;
	if (!fname.endsWith(".ammxml"))
		fname.append( ".ammxml" );

	// updated created and lastEdited tags
	if (m_project->m_created.isEmpty())
		m_project->m_created = QDateTime::currentDateTime().toString(Qt::TextDate);
	m_project->m_lastModified = QDateTime::currentDateTime().toString(Qt::TextDate);

	// save project file
	if (!write(parent, fname)) {
		QMessageBox::critical(
				parent,
				tr("Saving failed"),
				tr("Error while saving project file, see error log file '%1' for details.").arg(Directories::globalLogFile())
				);

		return SaveFailed;
	}

	// clear modified flag
	m_modified = false;
	// signal UI to update project status
	emit updateActions();

	// add project file name to recent file list
	if (addToRecentFilesList)
		addToRecentFiles(fname);


	return SaveOK; // saving succeeded
}


void ProjectHandler::setModified(int modificationType, ModificationInfo * data) {
	// special case:  modification type = NotModified
	//ModificationTypes modType = static_cast<ModificationTypes>(modificationType);
	//switch (modType) {

	//	default: ; // skip all others
	//}
	m_modified = true;

	emit modified(modificationType, data);
	emit updateActions();
}


const AMM::Config & ProjectHandler::project() const {
	FUNCID(ProjectHandler::project);

	if (m_project == nullptr)
		throw IBK::Exception("Must not call project() on invalid ProjectHandler.", FUNC_ID);
	return *m_project;
}


void ProjectHandler::updateLastReadTime() {
	FUNCID(ProjectHandler::updateLastReadTime);
	if (!isValid())
		throw IBK::Exception("Must not call updateLastReadTime() on invalid project.", FUNC_ID);
	m_lastReadTime = QFileInfo(projectFile()).lastModified().addSecs(5);
}



// *** PRIVATE MEMBER FUNCTIONS ***

void ProjectHandler::createProject() {
	Q_ASSERT(m_project == nullptr);

	m_project = new AMM::Config;
	m_projectFile.clear();
	m_modified = false; // new projects are never modified
}


void ProjectHandler::destroyProject() {
	Q_ASSERT(m_project != nullptr);

	delete m_project;
	m_project = nullptr;
	m_projectFile.clear();
}


bool ProjectHandler::read(QWidget * /*parent*/, const QString & fname) {
	FUNCID(ProjectHandler::read);

	// check that we have a project, should be newly created
	Q_ASSERT(isValid());

	if (!QFileInfo::exists(fname)) {
		IBK::IBK_Message(IBK::FormatString("File '%1' does not exist or permissions are missing for accessing the file.")
						 .arg(fname.toStdString()), IBK::MSG_ERROR, FUNC_ID);
		return false;
	}

	try {

		// filename is converted to utf8 before calling readXML
		m_project->readXML(fname);
		m_projectFile = fname;

		m_lastReadTime = QFileInfo(fname).lastModified();

		// after reading the project file, we should update the views
		// this is done in a subsequent call to setModified() from the calling function
		return true;
	}
	catch (IBK::Exception & ex) {
		ex.writeMsgStackToError();
	}
	catch (std::exception & ex) {
		// this shouldn't happen, unless we have something weird going on
		IBK::IBK_Message(IBK::FormatString("std::exception caught: %1").arg(ex.what()), IBK::MSG_ERROR, FUNC_ID);
	}

	return false;
}


bool ProjectHandler::write(QWidget *parent, const QString & fname) const {
	FUNCID(ProjectHandler::write);
	Q_ASSERT(isValid());

	// if project file exists, create backup file
	if (QFile(fname).exists()) {
		// if backup file exists already, delete it
		bool fileExists = QFile(fname + ".bak").exists();
		if (fileExists) {
			// try to delete the backup file
			if (QFile(fname + ".bak").remove())
				fileExists = false;
		}
		if (!fileExists && !QFile(fname).copy(fname + ".bak"))
			IBK::IBK_Message(IBK::FormatString("Cannot create backup file '%1' (path does not exists or missing permissions).")
							 .arg((fname + ".bak").toStdString()), IBK::MSG_ERROR, FUNC_ID);
	}

	// create file
	QFile file(fname);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
		IBK::IBK_Message(IBK::FormatString("Cannot create/write file '%1' (path does not exists or missing permissions).")
						 .arg(fname.toStdString()), IBK::MSG_ERROR, FUNC_ID);
		return false;
	}
	file.close();

	try {
		IBK::Path filename(fname.toStdString());
		IBK::Path oldProjectDir(m_projectFile.toStdString());
		std::map<std::string, IBK::Path> pmap;
		try {
			oldProjectDir = oldProjectDir.parentPath();
			pmap["Project Directory"] = oldProjectDir;
		}
		catch (...) {
			// invalid old path -> we shouldn't have "Project Directory" placeholder in this case...
		}

		// filename is converted to utf8 before calling writeXML
		m_project->writeXML(fname);

		// also set the project file name
		m_projectFile = fname;
		*const_cast<QDateTime*>(&m_lastReadTime) = QFileInfo(fname).lastModified();

		// remove backup file again
		QFile(fname + ".bak").remove();

		return true;
	}
	catch (IBK::Exception & ex) {
		ex.writeMsgStackToError();
		// restore file from backup file, but keep backup file just in case
		QFile(fname + ".bak").copy(fname);
	}
	catch (std::exception & ex) {
		// this shouldn't happen, unless we have something weird going on
		IBK::IBK_Message(IBK::FormatString("std::exception caught: %1").arg(ex.what()), IBK::MSG_ERROR, FUNC_ID);
	}
	return false;
}



void ProjectHandler::addToRecentFiles(const QString& fname) {

	Settings & si = Settings::instance();
	//qDebug() << si.m_recentProjects;

	// compose absolute file name
	QFileInfo finfo(fname);
	QString filePath =  finfo.absoluteFilePath();

	// check if recent project file is already in the list
	int i = si.m_recentProjects.indexOf(filePath);

	if (i != -1) {
		// already there, move it to front
		si.m_recentProjects.removeAt(i);
		si.m_recentProjects.push_front(filePath);
	}
	else {
		si.m_recentProjects.push_front(filePath);
		while (static_cast<unsigned int>(si.m_recentProjects.count()) > si.m_maxRecentProjects)
			si.m_recentProjects.pop_back();
	}

	// update recent project list
	emit updateRecentProjects();
}

