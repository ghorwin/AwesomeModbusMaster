#ifndef WelcomeScreenH
#define WelcomeScreenH

#include <QWidget>
#include <QUrl>

namespace Ui {
	class WelcomeScreen;
}

class QNetworkAccessManager;
class QNetworkReply;

/*! The welcome screen, showing news and recently projects. */
class WelcomeScreen : public QWidget {
	Q_OBJECT

public:

	explicit WelcomeScreen(QWidget *parent = nullptr);
	~WelcomeScreen() override;

	/*! Updates the welcome page content. */
	void updateWelcomePage();
	void setLabelColors(QString color);

public slots:
	/*! Triggered when user clicks on a project file or external link. */
	void onAnchorClicked( const QUrl & link );

	void onStyleChanged();

signals:
	/*! Emitted when user clicked on the file name of a recently used file. */
	void openProject(const QString & projectFile);

	void newProjectClicked();
	void openProjectClicked();

	/*! Emitted, when user has removed a project from the recent files list. */
	void updateRecentList();

private slots:
	void on_toolButtonNewProject_clicked();
	void on_toolButtonOpenProject_clicked();
	void downloadFinished(QNetworkReply *reply);

	/*! Triggered by update manager. */
	void onUpdateInfoRetrieved(int, QString);

	void on_labelRecentClicked();

private:
	Ui::WelcomeScreen	*m_ui;

	QNetworkAccessManager	*m_networkManager;

	/*! Contains information about current and available newer version (content is set in onUpdateInfoRetrieved()). */
	QString					m_updateInfoNews;
	/*! Holds the news content for the welcome page. */
	QString					m_welcomePageNews;
};

#endif // WelcomeScreenH
