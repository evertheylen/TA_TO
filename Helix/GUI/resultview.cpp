#include "resultview.h"
#include "ui_resultview.h"

#include <QFontDatabase>
#include <QMessageBox>
#include <QString>

void HTMLDelegate::paint(QPainter* painter, const QStyleOptionViewItem & option, const QModelIndex &index) const
{
	QStyleOptionViewItemV4 options = option;
	initStyleOption(&options, index);

	painter->save();

	QTextDocument doc;
	doc.setHtml(options.text);
	//QFont font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
	QFont font("monospace");	
	font.setPointSize(14);
	doc.setDefaultFont(font);

	options.text = "";
	options.widget->style()->drawControl(QStyle::CE_ItemViewItem, &options, painter);

	painter->translate(options.rect.left(), options.rect.top());
	QRect clip(0, 0, options.rect.width(), options.rect.height());
	doc.drawContents(painter, clip);

	painter->restore();
}

QSize HTMLDelegate::sizeHint ( const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
	QStyleOptionViewItemV4 options = option;
	initStyleOption(&options, index);

	QTextDocument doc;
	doc.setHtml(options.text);
	doc.setTextWidth(options.rect.width());
	return QSize(doc.idealWidth(), doc.size().height());
}





ResultView::ResultView(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::ResultView)
{
	ui->setupUi(this);

	HTMLDelegate* delegate = new HTMLDelegate();
	// set model and delegate to the treeview object
	//ui->treeView->setModel(model);
	ui->tableWidget->setItemDelegate(delegate);
	ui->tableWidget->verticalHeader()->setDefaultSectionSize(55);
}

#define set(row, col, text) num_item = new NumericTableWidgetItem(); \
    num_item->setText(QString::fromStdString(text)); \
    ui->tableWidget->setItem(row, col, num_item);


void ResultView::setResult(Result* _res) {
	res = _res;

	// Set text labels:
	// Filename
	std::cout << "file address: " << res->file << "\n";
	std::cout << res->file->name << " <-- filename\n";
	ui->label_file->setText(QString::fromStdString(res->file->name));
	// Query
	ui->label_query->setText(QString::fromStdString(res->query->input));
	// Summary
	std::cout << res->summary() << "\n";
	ui->label_summary->setText(QString::fromStdString(res->summary()));

    ui->tableWidget->horizontalHeaderItem(3)->setText("Total errors | Max: " + QString::number(res->query->max_total));
    ui->tableWidget->horizontalHeaderItem(4)->setText("Fakes | Max: " + QString::number(res->query->max_fakes));
    ui->tableWidget->horizontalHeaderItem(5)->setText("Skips | Max: " + QString::number(res->query->max_skips));
    ui->tableWidget->horizontalHeaderItem(6)->setText("Repetitions | Max: " + QString::number(res->query->max_reps));
    ui->tableWidget->horizontalHeaderItem(7)->setText("Ignores | Max: " + QString::number(res->query->max_ignores));


	int row = 0;
	for (Match& m: res->matches) {
        NumericTableWidgetItem* num_item; // see define above
		ui->tableWidget->insertRow(row);

		// Set formatted html
		QString richStr = QString::fromStdString(m.format(res->file));

        QTableWidgetItem* item = new QTableWidgetItem();
		item->setText(richStr);
		//QFont font("Monospace");
		//font.setStyleHint(QFont::TypeWriter);
		//QFont font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
		QFont font("monospace");
		item->setFont(font);
		ui->tableWidget->setItem(row, 0, item);

		set(row, 1, std::to_string(m.locations.size()));

		// Set locations
		std::string loc = std::to_string(m.locations.at(0));
		for (auto it=(m.locations.begin()+1); it != m.locations.end(); ++it) {
			loc += ", ";
			loc += std::to_string(*it);
		}
        QTableWidgetItem* item2 = new QTableWidgetItem();
        item2->setText(QString::fromStdString(loc));
        item2->setFont(font);
        ui->tableWidget->setItem(row, 2, item2);

		// Set nen hele hoop ints (total, fakes, skips, reps, ignores)
		set(row, 3, std::to_string(m.get_total_errors()));
		set(row, 4, std::to_string(m.fakes));
		set(row, 5, std::to_string(m.skips));
		set(row, 6, std::to_string(m.reps));
		set(row, 7, std::to_string(m.ignores));

		row++;
	}

	ui->tableWidget->resizeColumnsToContents();

}

ResultView::~ResultView()
{
	delete ui;
	delete this;
}

void ResultView::on_help_button_clicked()
{
    QString help = "The resultview is splitted into different areas. In the first area the matches are explained in detail.\n";
    help += "The first line is the match that is actually found in the suffixtree, the second line is the corresponding match found in the DFA.\n";
    help += "This line has color coding:\n\n";
    help += "Orange: Fake error -- A letter from the suffixtree is interpreted in a different way by the DFA\n\n";
    help += "Blue: Skip error -- The DFA has advanced to the next state without new input from the suffixtree\n\n";
    help += "Green: Repetition error -- The input of the suffixtree is the same as the previous input, the DFA remains in the same state\n\n";
    help += "Red: Ignore (or FakeRepetition) error -- The input of the suffixtree differs from the previous input, the DFA remains in the same state and ignores the suffixtree\n\n";
    help += "The same match can be found in multiple ways, with different kinds of errors.\n";
   // help += "\n\nBelow a scheme of all the different errors\n\n";
   // help += "-------+        |                    D F A                    |\nSUFFIX  \       |         stay         |       advance        |\n+------+----------------------+----------------------+\nstay          | /  /  /  /  /  /  /  |                      |\n";
   // help += "(can't be OK    |/  /  /  /  /  /  /  /|        Skip          |\nor not OK)  |  /  /  /  /  /  /  / |                      |\n----------------+----------------------+----------------------+\n|                      |                      |\n";
   // help += "OK   |      Repetition      |       Perfect!       |\n|                      |                      |\nadvance - - - -+- - - - - - - - - - - + - - - - - - - - - - -+\n|                      |                      |\nNOT OK |   Ignore / FakeRep   |         Fake         |\n|                      |                      |\n---------------+----------------------+----------------------+\n";

    QMessageBox::information(this, tr("Detailed results --- Help"), help);
}
