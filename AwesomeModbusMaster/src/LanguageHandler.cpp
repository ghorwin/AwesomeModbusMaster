#include "LanguageHandler.h"
#include <QTranslator>
#include <QSettings>
#include <QCoreApplication>
#include <QLocale>
#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include <QtCore/QLoggingCategory>

#include "Directories.h"
#include "Exception.h"

QString		LanguageHandler::m_organization;
QString		LanguageHandler::m_program;
QString		LanguageHandler::m_languageFilePrefix;

Q_LOGGING_CATEGORY(lcLanguages, "core.languages")

LanguageHandler & LanguageHandler::instance() {
	static LanguageHandler myHandler;
	return myHandler;
}


LanguageHandler::LanguageHandler() :
	applicationTranslator(NULL),
	systemTranslator(NULL)
{
}


LanguageHandler::~LanguageHandler() {
	// get rid of old translators
	// at this time, the application object doesn't live anylonger, so we
	// can savely destruct the translator objects
	delete applicationTranslator; applicationTranslator = NULL;
	delete systemTranslator; systemTranslator = NULL;
}


void LanguageHandler::setup(const QString & organization, const QString & program,
							const QString & languageFilePrefix)
{
	m_organization = organization;
	m_program = program;
	m_languageFilePrefix = languageFilePrefix;
}


QString LanguageHandler::langId() {
	QSettings config(m_organization, m_program);
	QString langid = config.value("LangID", QString() ).toString();
	if (langid.isEmpty()) {
		// try to determine language id from OS
		QString localeName = QLocale::system().name();
		qCDebug(lcLanguages) << QString("System locale: '%1'.").arg(localeName);
		int pos = localeName.indexOf('_');
		if (pos != -1)
			localeName = localeName.left(pos);
		qCDebug(lcLanguages) << QString("Translation required for locale: '%1'.").arg(localeName);
		langid = localeName;

		if (langid.isEmpty())
			langid = "en";
	}
	return langid;
}


void LanguageHandler::setLangId(QString id) {
	QSettings config(m_organization, m_program);
	config.setValue("LangID", id );
}


void LanguageHandler::installTranslator(QString langId) {

	// get rid of old translators
	if (applicationTranslator != NULL) {
		qApp->removeTranslator(applicationTranslator);
		delete applicationTranslator; applicationTranslator = NULL;
	}
	if (systemTranslator != NULL) {
		qApp->removeTranslator(systemTranslator);
		delete systemTranslator; systemTranslator = NULL;
	}


	// create new translators, unless we are using english
	if (langId == "en") {
		QSettings config(m_organization, m_program);
		config.setValue("LangID", langId);
		QLocale loc(QLocale::English);
		loc.setNumberOptions(QLocale::OmitGroupSeparator | QLocale::RejectGroupSeparator);
		QLocale::setDefault(loc);
	}

	QString translationFilePath = Directories::translationsFilePath(langId);
	QString qtTranslationFilePath = Directories::qtTranslationsFilePath(langId);

	qCDebug(lcLanguages) << QString("App translation file path = '%1'.").arg(translationFilePath);
	qCDebug(lcLanguages) << QString("Qt translation file path  = '%1'.").arg(qtTranslationFilePath);

	systemTranslator = new QTranslator;

	// system translator first, filename is for example "qt_de"
	systemTranslator = new QTranslator;
	QFileInfo finfoQt(qtTranslationFilePath);
	if (finfoQt.exists() && systemTranslator->load(finfoQt.fileName(), finfoQt.dir().absolutePath())) {
		qApp->installTranslator(systemTranslator);
		qCDebug(lcLanguages) << QString("Qt translation file loaded successfully");
	}
	else {
		qCWarning(lcLanguages) << QString("Could not load system translator file: '%1'.").arg(finfoQt.fileName());
		qCWarning(lcLanguages) << QString("Qt translation file path  = '%1'.").arg(qtTranslationFilePath);
		// no translator found, remove it again
		delete systemTranslator;
		systemTranslator = nullptr;
	}

	applicationTranslator = new QTranslator;
	QFileInfo finfo(translationFilePath);
	if (finfo.exists() && applicationTranslator->load(finfo.fileName(), finfo.dir().absolutePath())) {
		qCDebug(lcLanguages) << QString("Application translator loaded successfully.");
		qApp->installTranslator(applicationTranslator);
		// remember translator in settings
		QSettings config(m_organization, m_program);
		config.setValue("LangID", langId);
	}
	else {
		qCWarning(lcLanguages) << QString("Could not load application translator file: '%1'.").arg(translationFilePath);
		delete applicationTranslator;
		applicationTranslator = nullptr;
	}


	// now also set the corresponding locale settings (for number display etc.)
	if (langId == "de") {
		QLocale loc(QLocale::German);
		loc.setNumberOptions(QLocale::OmitGroupSeparator | QLocale::RejectGroupSeparator);
		QLocale::setDefault(loc);
	}
}

