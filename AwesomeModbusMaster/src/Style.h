#ifndef StyleH
#define StyleH

#include <QString>
#include <QColor>

class QPlainTextEdit;
class QWidget;
class QTableView;
class QTreeView;
class QListView;
class QLineEdit;


/*! Central implementation and configuration of visual appearance of the software.
	This class provides several member functions that can be used to tune the appearance of the software.
*/
class Style {
public:

	/*! Virtual D'tor */
	virtual ~Style() {}

	/*! Resizes column to contents, yet honors the enlargeOnly flag.
		Useful if user can adjust column width manually, but data changes may require enlarging
		column.
	*/
	static void resizeTableColumnToContents(QTableView * v, int column, bool enlargeOnly);

	enum ThemeType {
		TT_Normal,
		TT_Dark
	};

	/*! Toggles view theme and application style sheet. */
	virtual void setStyle(ThemeType theme);

	void formatPlainTextEdit(QPlainTextEdit * textEdit) const;
	void formatDatabaseTableView(QTableView * v, bool readOnly=false) const;
	void formatDatabaseTreeView(QTreeView * v) const;
	void formatListView(QListView * v) const;
	void formatReadOnlyLineEdit(QLineEdit * le) const;

	/*! Modifies line edit to get into error state.
		If toolTip is not an empty string, this error-info-tool tip is set in case of error.
		Mind to reset the tooltip when correct.
	*/
	void setErrorStateLineEdit(QLineEdit * lineEdit, const QString & toolTip) const;
	/*! Clears tooltip and resets palette to default. */
	void resetErrorStateLineEdit(QLineEdit * lineEdit, bool clearToolTip=true) const;

	/*! Returns column width to be used at least for a given string. */
	unsigned int columnWidth(const QString & s) const;

	void setHtmlColors(QString & htmlCode);

	QString				m_styleSheet;

	QColor				m_colorLogProgressText;
	QColor				m_colorLogErrorText;
	QColor				m_colorLogWarningText;
	QColor				m_colorLogDebugText;

	QColor				m_alternativeBackgroundBright;
	QColor				m_alternativeBackgroundDark;
	QColor				m_alternativeBackgroundText;

	QColor				m_summationRowColorDarker;
	QColor				m_summationRowColor;
	QColor				m_summationRowColorLighter;

	QColor				m_editFieldErrorColor;
	QColor				m_readOnlyTableBackground;

	QColor				m_finishedProcessColor;

	QColor				m_bookingOkColor;
	QColor				m_bookingNeedsUpdateColor;

	QColor				m_lessonsLearnedEffortColor;
	QColor				m_lessonsLearnedCorrectionColor;
	QColor				m_lessonsLearnedResultsColor;
	QColor				m_lessonsLearnedCommentColor;

	/*! Table/Cell height, computed based on font-properties, hereby honoring font scaling. */
	unsigned int		m_viewRowHeight;
	unsigned int		m_viewFontPointSize;
	unsigned int		m_dayBookingTableColumnWidth;

	ThemeType			m_theme = TT_Normal;
};


#endif // StyleH
