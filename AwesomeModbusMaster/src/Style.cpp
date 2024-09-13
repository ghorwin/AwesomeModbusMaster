#include "Style.h"

#include <QFile>
#include <QApplication>

#include <QPlainTextEdit>
#include <QLayout>
#include <QTableView>
#include <QTreeView>
#include <QLineEdit>
#include <QHeaderView>
#include <QListView>
#include <QDebug>
#include <QColor>
#include <QtCore/QLoggingCategory>

#include "Settings.h"

Q_LOGGING_CATEGORY(lcUitStyle, "core.style")

void Style::setStyle(ThemeType theme) {
	QFile fileDark(":/stylesheets/dark.qss");

	if ( theme == TT_Dark && fileDark.exists()) {

		fileDark.open(QFile::ReadOnly);
		m_styleSheet = QLatin1String(fileDark.readAll());
		qApp->setStyleSheet(m_styleSheet);
		qCDebug(lcUitStyle) << "Using dark style";

		m_colorLogProgressText						= "#c0c0c0";
		m_colorLogErrorText							= "#ff2222";
		m_colorLogWarningText						= "#f0dc00";
		m_colorLogDebugText							= "#39b1d9";

		m_alternativeBackgroundBright				= "#73580e";
		m_alternativeBackgroundDark					= "#57430b";
		m_alternativeBackgroundText					= "#ffedce";

		m_summationRowColorDarker					= "#FFc380";
		m_summationRowColor							= "#FFE3a0";
		m_summationRowColorLighter					= "#FFF3c0";

		m_editFieldErrorColor						= "#FF8060";
		m_readOnlyTableBackground					= "#fff0d0";

		m_lessonsLearnedEffortColor					= "#FFE3a0";
		m_lessonsLearnedCorrectionColor				= "#e0ffc0";
		m_lessonsLearnedResultsColor				= "#fadbd8";
		m_lessonsLearnedCommentColor				= "#dfdfff";
	}
	else {
		QFile fileWhite(":/stylesheets/normal.qss"); // TODO : distinguish between Linux and Windows
		if (fileWhite.exists()) {

			qCDebug(lcUitStyle) << "Using bright style";
			fileWhite.open(QFile::ReadOnly);
			m_styleSheet = QLatin1String(fileWhite.readAll());
			qApp->setStyleSheet(m_styleSheet);
		}
		else {
			// clear style sheet for default style.
			qApp->setStyleSheet("");
		}

		// set specific background/font colors
		m_alternativeBackgroundBright				= "#ffffff";
		m_alternativeBackgroundDark					= "#dfdfff";
		m_alternativeBackgroundText					= "#000000";

		m_colorLogProgressText						= "#202020";
		m_colorLogErrorText							= "#ab0000";
		m_colorLogWarningText						= "#b17d00";
		m_colorLogDebugText							= "#39b1d9";

		m_summationRowColorDarker					= "#FFc380";
		m_summationRowColor							= "#FFE3a0";
		m_summationRowColorLighter					= "#FFF3c0";

		m_editFieldErrorColor						= "#FF8060";
		m_readOnlyTableBackground					= "#FFF0D0";

		m_finishedProcessColor						= "#8080c0";

		m_bookingOkColor							= "#e0ffc0";
		m_bookingNeedsUpdateColor					= "#b0bbff";

		m_lessonsLearnedEffortColor					= "#FFE3a0";
		m_lessonsLearnedCorrectionColor				= "#e0ffc0";
		m_lessonsLearnedResultsColor				= "#fadbd8";
		m_lessonsLearnedCommentColor				= "#dfdfff";
	}

	// our application font in style sheet is Segoe UI on windows, with 9pt size
#ifdef Q_OS_WIN
	qApp->setFont(QFont("Segoe UI", 9));
#else
	qApp->setFont(QFont("Ubuntu", 10));
#endif // Q_OS_WIN
	// our tables have point size 8pt, so compute the table row height based on that size
	QFont f(qApp->font());
	f.setPointSize(8);
	QFontMetrics fm(f);
	m_viewRowHeight = fm.height();
	m_viewFontPointSize = 8; // needed for Linux style fixes
	m_dayBookingTableColumnWidth = fm.boundingRect("8888.").width();
	qCDebug(lcUitStyle) << "Table View Row Height (based on font size):" << m_viewRowHeight;
	qCDebug(lcUitStyle) << "Table View Colunm Width (based on font size):" << m_dayBookingTableColumnWidth;
}


void Style::formatPlainTextEdit(QPlainTextEdit * textEdit) const {
	// customize log window
	textEdit->setContextMenuPolicy(Qt::NoContextMenu);
	textEdit->setReadOnly(true);
	textEdit->setUndoRedoEnabled(false);
	textEdit->setWordWrapMode(QTextOption::NoWrap);
}


void Style::formatDatabaseTableView(QTableView * v, bool readOnly) const {
	v->verticalHeader()->setDefaultSectionSize(m_viewRowHeight);
	v->verticalHeader()->setVisible(false);
	v->horizontalHeader()->setMinimumSectionSize(m_viewRowHeight);
	v->setSelectionBehavior(QAbstractItemView::SelectRows);
	v->setSelectionMode(QAbstractItemView::SingleSelection);
	v->setAlternatingRowColors(true);
	v->setSortingEnabled(true);
	v->sortByColumn(0, Qt::AscendingOrder);

	if (v->model() != nullptr)
		for (int r=0, rows = v->model()->rowCount(); r<rows; ++r)
			v->setRowHeight(r, m_viewRowHeight);

	if (readOnly) {
		v->setStyleSheet("QTableView {"
							"  background: #fff0d0;"
						  "}");
	}
}


void Style::formatListView(QListView * v) const {
	v->setAlternatingRowColors(true);
#ifdef Q_OS_WIN
	// no need to adjust stylesheet on Windows
#else
	QString viewStyleSheet = QString("QListView {font-size:%1pt;}").arg(m_viewFontPointSize);
	v->setStyleSheet(viewStyleSheet);
#endif
}


void Style::formatReadOnlyLineEdit(QLineEdit * le) const {
	le->setAutoFillBackground(true);
	le->setStyleSheet("QLineEdit:read-only {"
						"  background: #fff0d0;"
					  "}");
}


unsigned int Style::columnWidth(const QString & s) const {
	QFont f(qApp->font());
	f.setPointSize(8);
	QFontMetrics fm(f);
	return fm.boundingRect(s+"xx").width(); // add two xx for the margin left and right
}


void Style::formatDatabaseTreeView(QTreeView * v) const {
	v->header()->setMinimumSectionSize(m_viewRowHeight);
	v->setSelectionBehavior(QAbstractItemView::SelectRows);
	v->setSelectionMode(QAbstractItemView::SingleSelection);
	v->setAlternatingRowColors(true);
	v->setSortingEnabled(false);
	v->setIndentation(m_viewRowHeight*2);
//	v->sortByColumn(0, Qt::AscendingOrder);
#if !defined(Q_OS_WIN)
	// Since we have an applition-wide style sheet, we must specify our customizations also via stylesheet.
	// Since we only tweak font sizes, this works for both bright and dark style.
	QString headerStyleSheet = QString("QHeaderView::section {font-size:%1pt;}").arg(m_viewFontPointSize);
	v->header()->setStyleSheet(headerStyleSheet);
	QString viewStyleSheet = QString("QTreeView {font-size:%1pt;}").arg(m_viewFontPointSize);
	v->setStyleSheet(viewStyleSheet);
#endif
}


void Style::resizeTableColumnToContents(QTableView * v, int column, bool enlargeOnly) {
	int w = v->columnWidth(column);
	v->resizeColumnToContents(0);
	int w2 = v->columnWidth(column);
	if (enlargeOnly && w2 < w)
		v->setColumnWidth(column, w);
}


void Style::setErrorStateLineEdit(QLineEdit * lineEdit, const QString & toolTip) const {
	QPalette errorPal;
	errorPal.setColor(QPalette::Base, m_editFieldErrorColor);
	lineEdit->setPalette(errorPal);
	if (!toolTip.isEmpty())
		lineEdit->setToolTip(toolTip);
}


void Style::resetErrorStateLineEdit(QLineEdit * lineEdit, bool clearToolTip) const {
	if (clearToolTip)
		lineEdit->setToolTip(QString());
	if (lineEdit->isReadOnly())
		formatReadOnlyLineEdit(lineEdit);
	else
		lineEdit->setPalette(QPalette());
}


void Style::setHtmlColors(QString & htmlCode) {
	switch (m_theme) {
		case TT_Dark :
		{
			htmlCode.replace("${STYLE_TEXT_COLOR}", "#F0F0F0");
			htmlCode.replace("${STYLE_BACKGROUND_COLOR}", "#212124");
			htmlCode.replace("${STYLE_LINKTEXT_COLOR}", "#ffbf14");
			htmlCode.replace("${STYLE_LINKTEXT_HOVER_COLOR}", "#ffffff");
			htmlCode.replace("${STYLE_LINKTEXT_HOVER_BACKGROUND_COLOR}", "#19232D");
			htmlCode.replace("${STYLE_H1_COLOR}", "#ff7e16");
			htmlCode.replace("${STYLE_H2_COLOR}", "#ff5b1a");
			htmlCode.replace("${STYLE_H3_COLOR}", "#ff5b1a");
		} break;

		case TT_Normal :
		default:
		{
			htmlCode.replace("${STYLE_TEXT_COLOR}", qApp->palette().color(QPalette::Text).name());
			htmlCode.replace("${STYLE_BACKGROUND_COLOR}", qApp->palette().color(QPalette::Background).name());
			htmlCode.replace("${STYLE_LINKTEXT_COLOR}", "#0053A6");
			htmlCode.replace("${STYLE_LINKTEXT_HOVER_COLOR}", "#1C7DEF");
			htmlCode.replace("${STYLE_LINKTEXT_HOVER_BACKGROUND_COLOR}", qApp->palette().color(QPalette::Background).name());
			htmlCode.replace("${STYLE_H1_COLOR}", "#003264");
			htmlCode.replace("${STYLE_H2_COLOR}", "#0069A8");
			htmlCode.replace("${STYLE_H3_COLOR}", "#00660F");
		} break;
	}

}
