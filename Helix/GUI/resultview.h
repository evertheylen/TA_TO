#ifndef RESULTVIEW_H
#define RESULTVIEW_H

#include <QMainWindow>
#include <QLabel>
#include "../engine_s/search.h"
#include <QStyledItemDelegate>
#include <QPainter>
#include <QTextDocument>
#include <QFont>
#include <QTableWidgetItem>

// http://stackoverflow.com/questions/1956542/how-to-make-item-view-render-rich-html-text-in-qt
class HTMLDelegate : public QStyledItemDelegate
{
protected:
	void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
	QSize sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const;
};



namespace Ui {
	class ResultView;
}

class NumericTableWidgetItem : public QTableWidgetItem {
    public:
        bool operator <(const QTableWidgetItem &right) const
        {
            return text().toInt() < right.text().toInt();
        }
};

class ResultView : public QMainWindow
{
	Q_OBJECT

public:
	explicit ResultView(QWidget *parent = 0);
	~ResultView();

	void setResult(Result* res);

	Result* res;

private slots:
    void on_help_button_clicked();

private:
	Ui::ResultView *ui;
};

#endif // RESULTVIEW_H
