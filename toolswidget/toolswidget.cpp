#include "toolswidget.h"
#include "mainwindow.h"
#include "toolswidget/ui_toolswidget.h"

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

void ToolsWidget::setTool(const Tool::Tools tool)
{
    switch (tool) {
    case Tool::Select:
        selectToolButtonClicked();
        break;
    case Tool::Move:
        moveToolButtonClicked();
        break;
    default:
        break;
    }
}

Select &ToolsWidget::select()
{
    return _select;
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
    emit toolSelected(Tool::Tools::Select);
}

void ToolsWidget::moveToolButtonClicked()
{
    _tool = &_move;
    toolButtonClicked();
    emit toolSelected(Tool::Tools::Move);
}

void ToolsWidget::toolReleased()
{
    ui->selectToolButton->click();
}

void ToolsWidget::moveMotion()
{
    _window->app()->undoStack().push(new TransformNodeCommand(_window, _scene.focusNode(), _move.transform()));
}
