#include "selectwidget.h"
#include "ui_selectwidget.h"

SelectWidget::SelectWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SelectWidget)
{
    ui->setupUi(this);

    connect(ui->selectionModeComboBox, &QComboBox::currentIndexChanged, this, &SelectWidget::selectionModeDidChange);
}

SelectWidget::~SelectWidget()
{
    delete ui;
}

void SelectWidget::selectionModeDidChange(const int idx)
{
    switch (idx) {
    case 0:
        emit selectionModeChanged(Select::Mode::OBJECTS);
        break;
    case 1:
        emit selectionModeChanged(Select::Mode::SURFACES);
        break;
    default:
        break;
    }
}
