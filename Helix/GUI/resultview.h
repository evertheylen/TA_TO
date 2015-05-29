#ifndef RESULTVIEW_H
#define RESULTVIEW_H

#include <QMainWindow>
#include <QLabel>
#include "../engine_s/search.h"

namespace Ui {
	class ResultView;
}

class ResultView : public QMainWindow
{
	Q_OBJECT

public:
	explicit ResultView(QWidget *parent = 0);
	~ResultView();

	void setResult(Result* res);

	Result* res;

private:
	Ui::ResultView *ui;
};

#endif // RESULTVIEW_H
