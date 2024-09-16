#ifndef MAINCONTENTWINDOW_H
#define MAINCONTENTWINDOW_H

#include <QWidget>

namespace Ui {
class MainContentWindow;
}

/*! Holds the actual content of the application. */
class MainContentWindow : public QWidget {
	Q_OBJECT

public:
	explicit MainContentWindow(QWidget *parent = nullptr);
	~MainContentWindow() override;

private:
	Ui::MainContentWindow *m_ui;
};

#endif // MAINCONTENTWINDOW_H
