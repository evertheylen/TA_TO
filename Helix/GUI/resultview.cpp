#include "resultview.h"
#include "ui_resultview.h"

#include <QFontDatabase>

void HTMLDelegate::paint(QPainter* painter, const QStyleOptionViewItem & option, const QModelIndex &index) const
{
	QStyleOptionViewItemV4 options = option;
	initStyleOption(&options, index);

	painter->save();

	QTextDocument doc;
	doc.setHtml(options.text);
	QFont font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
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

#define set(row, col, text) item = new QTableWidgetItem(); \
	item->setText(QString::fromStdString(text)); \
	ui->tableWidget->setItem(row, col, item);


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

	int row = 0;
	for (Match& m: res->matches) {
		QTableWidgetItem* item; // see define above
		ui->tableWidget->insertRow(row);

		// Set formatted html
		QString richStr = QString::fromStdString(m.format(*res->file));

		item = new QTableWidgetItem();
		item->setText(richStr);
		//QFont font("Monospace");
		//font.setStyleHint(QFont::TypeWriter);
		QFont font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
		item->setFont(font);
		ui->tableWidget->setItem(row, 0, item);

		set(row, 1, std::to_string(m.locations.size()));

		// Set locations
		std::string loc = std::to_string(m.locations.at(0));
		for (auto it=(m.locations.begin()+1); it != m.locations.end(); ++it) {
			loc += ", ";
			loc += std::to_string(*it);
		}
		set(row, 2, loc);

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
