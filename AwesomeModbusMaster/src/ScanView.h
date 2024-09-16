#ifndef SCANVIEW_H
#define SCANVIEW_H

#include <QWidget>

namespace Ui {
	class ScanView;
}

/*! The widget for the "scan for modbus server" functionality. */
class ScanView : public QWidget {
	Q_OBJECT

public:
	explicit ScanView(QWidget *parent = nullptr);
	~ScanView() override;

private:
	Ui::ScanView *m_ui;
};

#endif // SCANVIEW_H
