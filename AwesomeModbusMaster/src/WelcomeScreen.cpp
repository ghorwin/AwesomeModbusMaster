#include "WelcomeScreen.h"
#include "ui_WelcomeScreen.h"

#include <QDesktopServices>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QTextStream>
#include <QNetworkAccessManager>
#include <QPixmap>
#include <QFileInfo>

#include "AMM_Config.h"
#include "Constants.h"

#include <IBK_Version.h>
#include <IBK_BuildFlags.h>

#include "LanguageHandler.h"
#include "Directories.h"

#include "Constants.h"
#include "Settings.h"
#include "Style.h"

extern const char * const HTML_TEMPLATE;
extern const char * const HTML_NEWS_TEMPLATE;
extern const char * const RECENT_PROJECT_TABLE_TEMPLATE;

WelcomeScreen::WelcomeScreen(QWidget *parent) :
	QWidget(parent),
	m_ui(new Ui::WelcomeScreen),
	m_networkManager(new QNetworkAccessManager)
{
	m_ui->setupUi(this);
	connect(m_networkManager, SIGNAL(finished(QNetworkReply*)),
			this, SLOT(downloadFinished(QNetworkReply*)));
	connect(m_ui->textBrowserMain, SIGNAL(anchorClicked(QUrl)),
			this, SLOT(onAnchorClicked(QUrl)));

	// format text browser
	m_ui->textBrowserMain->setStyleSheet("QTextBrowser { border: none; }");
	m_ui->textBrowserNews->setStyleSheet("QTextBrowser { border: none; }");

	m_ui->toolButtonNewProject->setIcon(QIcon::fromTheme("file_new"));
	m_ui->toolButtonOpenProject->setIcon(QIcon::fromTheme("file_open"));

	// labels need a fixed width to prevent shifting the layout during hovering (bold text is wider)
	// we set the width dynamically here, as it depends on translation
	m_ui->labelRecent->setActive(true);
	QSize s1 = m_ui->labelRecent->sizeHint();
	m_ui->labelRecent->setFixedWidth(s1.width());

	connect(m_ui->labelRecent, SIGNAL(clicked()), this, SLOT(on_labelRecentClicked()));

	on_labelRecentClicked();

	// hide news for now
	m_ui->textBrowserNews->setVisible(false);

}


WelcomeScreen::~WelcomeScreen() {
	delete m_ui;
	delete m_networkManager;
}


void WelcomeScreen::updateWelcomePage() {
	// news text
	QString htmlPageNews = HTML_NEWS_TEMPLATE;
	QString welcomePageSideBar = QString("<p>&nbsp;<p><hr>"); // top-rule starts the side-bar column
	// add update info section
	if (!m_updateInfoNews.isEmpty())
		welcomePageSideBar.append(m_updateInfoNews);
	// download news content (only once per application start)
	if (m_welcomePageNews.isEmpty()) {
		QNetworkRequest request(QUrl(QString::fromLatin1(NEWS_FILE_URL)));
		/*QNetworkReply *reply = */m_networkManager->get(request);
	}
	// insert news
	if (m_welcomePageNews.isEmpty()) {
		welcomePageSideBar.append(QString("<i>%1</i>").arg(tr("Retrieving news content...")));
	}
	else {
		welcomePageSideBar.append(m_welcomePageNews);
	}
	htmlPageNews.replace("${NEWS_CONTENT}", welcomePageSideBar);
	Settings::instance().style()->setHtmlColors(htmlPageNews);
	m_ui->textBrowserNews->setHtml(htmlPageNews);

	// now main page
	QString htmlPage = HTML_TEMPLATE;
	Settings::instance().style()->setHtmlColors(htmlPage);

	// compose recent project file list table

	QString infoString;

	for (int i=0; i<Settings::instance().m_recentProjects.count(); ++i) {
		QFileInfo finfo(Settings::instance().m_recentProjects[i]);
		QString projectInfoBlock = RECENT_PROJECT_TABLE_TEMPLATE;
		QString description = tr("<i><font color=\"${STYLE_H3_COLOR}\">Project not accessible</font></i> <a href=\"premove:%1\">Remove %2 from list</a>").arg( i ).arg( finfo.fileName() );
		Settings::instance().style()->setHtmlColors(description);

		// thumbnails generated from the software have all the same thumbnail size
		projectInfoBlock = projectInfoBlock.replace("${PROJECT_FILENAME}", finfo.fileName());
		projectInfoBlock = projectInfoBlock.replace("${PROJECT_FULL_PATH}", finfo.filePath());
		infoString += projectInfoBlock;
	}

	htmlPage.replace("${RECENT_PROJECT_FILELIST}", infoString);
	m_ui->textBrowserMain->setHtml(htmlPage);
}


void WelcomeScreen::setLabelColors(QString color) {
	m_ui->labelRecent->setStyleSheet("QLabel { font-weight: normal}", "QLabel { font-weight: bold}", QString("QLabel { font-weight: bold; color: %1}").arg(color));
}


void WelcomeScreen::onAnchorClicked( const QUrl & link ) {
	// if anchor starts with "pfile:" we emit the "open project" signal
	if (link.toString().startsWith("pfile:")) {
		QString fname = link.toString();
		fname = fname.right(fname.length()-QString("pfile:").size());
		emit openProject(fname);
		return;
	}
	// if anchor starts with "pexample:" we emit the "open example" signal
	else if (link.toString().startsWith("premove:")) {

		// extract project index to delete and remove it from list
		QString index = link.toString();
		index = index.right(index.length()-QString("premove:").size());
		bool ok;
		int ind = index.toInt(&ok);
		if (ok && ind < Settings::instance().m_recentProjects.size()){
			Settings::instance().m_recentProjects.removeAt(ind);
		}

		// refresh view afterwards
		updateWelcomePage();

		// refresh menue structure
		emit updateRecentList();

		return;
	}

	// must be an external link, call desktop services to open webbrowser
	QDesktopServices::openUrl(link);
}


void WelcomeScreen::on_toolButtonNewProject_clicked() {
	emit newProjectClicked();
}

void WelcomeScreen::on_toolButtonOpenProject_clicked() {
	emit openProjectClicked();
}

void WelcomeScreen::downloadFinished(QNetworkReply *reply) {
	if (reply->error()) {
		m_welcomePageNews = QString("<i>%1</i>")
							.arg(tr("unable to download news content."));
	}
	else {
		QByteArray newsRaw = reply->readAll();
		// extract text for current language
		QString news = QString::fromUtf8(newsRaw);
		bool languageFound = false;
		if (news.indexOf("[lang:") != -1) {
#if QT_VERSION >= 0x050e00
			QStringList langTexts = news.split(QString("[lang:"), Qt::SkipEmptyParts);
#else
			QStringList langTexts = news.split(QString("[lang:"), QString::SkipEmptyParts);
#endif
			for (int i=0; i<langTexts.count(); ++i) {
				QString langId = langTexts[i].left(2);
				if (langId == LanguageHandler::instance().langId()) {
					news = langTexts[i].right(langTexts[i].count()-3);
					languageFound = true;
					break;
				}
			}
			if (!languageFound && langTexts.count()>0) {
				// language not found, default to the first language (should be english)
				news = langTexts[0].right(langTexts[0].count()-3);
			}
		}

		// store news text
		m_welcomePageNews = news;
	}

	reply->deleteLater();
	// this will trigger a welcome page update (but no further download attempt)
	updateWelcomePage();
}


void WelcomeScreen::onUpdateInfoRetrieved(int res, QString newVersion) {
	// generate update info text, if update information was successfully retrieved
	// first set current version text
	if (res == 0) {
		// if we have a newer version, add html text with update link
		if (IBK::Version::lesserVersionNumber(LONG_VERSION, newVersion.toStdString())) {
			m_updateInfoNews = tr("<h1><font style=\"color:${STYLE_H1_COLOR}\">An update is available!</font></h1><p>");
			m_updateInfoNews += tr("Current version: <b>%1</b><br>").arg(LONG_VERSION);
			m_updateInfoNews += tr("New Version: <b><font style=\"color:${STYLE_LINKTEXT_COLOR}\">%1</font></b></p><p><a href=\"update:\">Download update</a>").arg(newVersion);
			m_updateInfoNews += "</p><hr>";
			Settings::instance().style()->setHtmlColors(m_updateInfoNews);
		}
	}
	updateWelcomePage();
}


void WelcomeScreen::on_labelRecentClicked() {
	m_ui->labelRecent->setActive(true);
	updateWelcomePage();
}



void WelcomeScreen::onStyleChanged() {
	// set colors of clickable labels
	if (Settings::instance().style()->m_theme == Style::TT_Dark)
		setLabelColors("#eb9a2f");
	else
		setLabelColors("#eb9a2f");
	updateWelcomePage(); // this also updates html colors
	update();


	QPixmap pixmap = QIcon::fromTheme("VICUS_Logo").pixmap(250);
	m_ui->labelLogo->setPixmap(pixmap);
}



const char * const HTML_TEMPLATE =
		"<html>\n"
		"<head>\n"
		"<style type=\"text/css\">\n"
		"body     { font-size: medium; color: ${STYLE_TEXT_COLOR}; background-color: ${STYLE_BACKGROUND_COLOR}; margin-left:0px }\n"
		"a        { color: ${STYLE_LINKTEXT_COLOR}; text-decoration:none; }\n"
		"a:hover  { color: ${STYLE_LINKTEXT_HOVER_COLOR}; background-color: ${STYLE_LINKTEXT_HOVER_BACKGROUND_COLOR}; }\n"
		"p        { font-size: medium; text-align: justify; margin-top:0px; margin-bottom:8px;   }\n"
		"h1       { font-size: large; color: ${STYLE_H1_COLOR}; font-weight:bold; \n"
		"           text-decoration: none; margin-top:15px; margin-bottom:15px }\n"
		"h2       { font-size: medium; color: ${STYLE_H2_COLOR}; font-weight:bold; margin-top:15px; margin-bottom:6px }\n"
		"h3       { font-size: medium; color: ${STYLE_H3_COLOR}; font-weight:bold; margin-top:10px; margin-bottom:2px}\n"
		"table    { font-size: medium }\n"
		"b        { color: black }\n"
		"pre      { font-size: small; font-family: monospace;courier }\n"
		"li       { text-align: justify }\n"
		"</style>\n"
		"\n"
		"</head>\n"
		"<body>\n"
		"${RECENT_PROJECT_FILELIST}\n"
		"</body>\n"
		"</html>\n"
		"\n";


const char * const HTML_NEWS_TEMPLATE =
		"<html>\n"
		"<head>\n"
		"<style type=\"text/css\">\n"
		"body     { font-size: medium; color: ${STYLE_TEXT_COLOR}; background-color: ${STYLE_BACKGROUND_COLOR} }\n"
		"a        { color: ${STYLE_LINKTEXT_COLOR}; text-decoration:none; }\n"
		"a:hover  { color: ${STYLE_LINKTEXT_HOVER_COLOR}; background-color: ${STYLE_LINKTEXT_HOVER_BACKGROUND_COLOR}; }\n"
		"p        { font-size: medium; text-align: justify; margin-top:0px; margin-bottom:8px;   }\n"
		"h1       { font-size: large; color: ${STYLE_H1_COLOR}; font-weight:bold; \n"
		"           text-decoration: none; margin-top:15px; margin-bottom:15px }\n"
		"h2       { font-size: medium; color: ${STYLE_H2_COLOR}; font-weight:bold; margin-top:15px; margin-bottom:6px }\n"
		"h3       { font-size: medium; color: ${STYLE_H3_COLOR}; font-weight:bold; margin-top:10px; margin-bottom:2px}\n"
		"table    { font-size: medium }\n"
		"b        { color: black }\n"
		"pre      { font-size: small; font-family: monospace;courier }\n"
		"li       { text-align: justify }\n"
		"</style>\n"
		"\n"
		"</head>\n"
		"<body>\n"
		"${NEWS_CONTENT}\n"
		"</body>\n"
		"</html>\n"
		"\n";


const char * const RECENT_PROJECT_TABLE_TEMPLATE =
		"<table border=\"0\" cellspacing=\"2\" cellpadding=\"0\">\n"
		"<tr valign=center><th width=\"${THUMBNAILSIZE}\" rowspan=\"3\">${IMG_FILENAME}</th><th align=left>${PROJECT_FILENAME}</th></tr>\n"
		"<tr valign=center><td align=left><a href=\"pfile:${PROJECT_FULL_PATH}\">${PROJECT_FULL_PATH}</a></td></tr>\n"
		"<tr valign=top><td align=justify>${PROJECT_DESCRIPTION}</td></tr>\n"
		"</table>\n"
		"<br>\n"
		"\n";

