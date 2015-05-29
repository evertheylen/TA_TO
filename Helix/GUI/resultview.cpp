#include "resultview.h"
#include "ui_resultview.h"

ResultView::ResultView(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::ResultView)
{
	ui->setupUi(this);

}

void ResultView::setResult(Result* res) {
	res = _res;

	// Set text labels:




}

ResultView::~ResultView()
{
	delete ui;
	delete this;
}
