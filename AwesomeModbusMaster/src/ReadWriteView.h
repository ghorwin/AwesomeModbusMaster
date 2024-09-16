#ifndef READWRITEVIEW_H
#define READWRITEVIEW_H

#include <QWidget>

namespace Ui {
class ReadWriteView;
}

class ReadWriteView : public QWidget
{
	Q_OBJECT

public:
	explicit ReadWriteView(QWidget *parent = nullptr);
	~ReadWriteView();

private:
	Ui::ReadWriteView *ui;
};

#endif // READWRITEVIEW_H
