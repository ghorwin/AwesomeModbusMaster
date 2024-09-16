#ifndef LOGGINGVIEW_H
#define LOGGINGVIEW_H

#include <QWidget>

namespace Ui {
class LoggingView;
}

class LoggingView : public QWidget
{
	Q_OBJECT

public:
	explicit LoggingView(QWidget *parent = nullptr);
	~LoggingView();

private:
	Ui::LoggingView *ui;
};

#endif // LOGGINGVIEW_H
