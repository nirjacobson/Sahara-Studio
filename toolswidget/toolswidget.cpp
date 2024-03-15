#include "toolswidget.h"
#include "ui_toolswidget.h"
#include "mainwindow.h"

ToolsWidget::ToolsWidget(MainWindow* window, Sahara::Scene& scene, QWidget *parent) :
    QWidget(parent),
    _window(window),
    ui(new Ui::ToolsWidget),
    _select(scene),
    _move(scene),
    _scene(scene)
{
    ui->setupUi(this);

    connect(&_select, &Tool::released, this, &ToolsWidget::toolReleased);
    connect(&_select, &Select::updatedScene, this, &ToolsWidget::updatedScene);
    connect(&_move, &Tool::released, this, &ToolsWidget::toolReleased);
    connect(&_move, &Move::mouseMotion, this, &ToolsWidget::moveMotion);

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

void ToolsWidget::moveMotion()
{
    _window->app()->undoStack().push(new TransformNodeCommand(_window, _scene.focusNode(), _move.transform()));
}
