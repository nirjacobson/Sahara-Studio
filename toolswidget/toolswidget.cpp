#include "toolswidget.h"
#include "ui_toolswidget.h"

ToolsWidget::ToolsWidget(Sahara::Scene& scene, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ToolsWidget),
    _select(scene),
    _move(scene)
{
    ui->setupUi(this);

    connect(&_select, &Tool::released, this, &ToolsWidget::toolReleased);
    connect(&_select, &Tool::updatedNode, this, &ToolsWidget::updatedNode);
    connect(&_select, &Tool::updatedScene, this, &ToolsWidget::updatedScene);
    connect(&_move, &Tool::released, this, &ToolsWidget::toolReleased);
    connect(&_move, &Tool::updatedNode, this, &ToolsWidget::updatedNode);
    connect(&_move, &Tool::updatedScene, this, &ToolsWidget::updatedScene);

    _tool = &_select;

    ui->selectToolButton->setChecked(true);
    ui->selectToolButton->setIcon(QIcon(":/icons/cursor.png"));
    ui->moveToolButton->setIcon(QIcon(":/icons/move.png"));

    connect(ui->selectToolButton, &QToolButton::clicked, this, &ToolsWidget::selectToolButtonClicked);
    connect(ui->moveToolButton, &QToolButton::clicked, this, &ToolsWidget::moveToolButtonClicked);
}

ToolsWidget::~ToolsWidget()
{
    delete ui;
}

void ToolsWidget::setScene(Sahara::Scene& scene)
{
    _select.setScene(scene);
    _move.setScene(scene);
}

void ToolsWidget::mouseMoved(const QVector2D& ndc)
{
    _tool->mouseMoved(ndc);
}

void ToolsWidget::mousePressed(const QVector2D& ndc)
{
    _tool->mousePressed(ndc);
}

void ToolsWidget::toolButtonClicked()
{
    ui->selectToolButton->setChecked(_tool == &_select);
    ui->moveToolButton->setChecked(_tool == &_move);
}

void ToolsWidget::selectToolButtonClicked()
{
    _tool = &_select;
    toolButtonClicked();
}

void ToolsWidget::moveToolButtonClicked()
{
    _tool = &_move;
    toolButtonClicked();
}

void ToolsWidget::toolReleased()
{
    ui->selectToolButton->click();
}
