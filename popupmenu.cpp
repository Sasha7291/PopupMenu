#include "popupmenu.h"

#include <QTimerEvent>
#include <QToolBar>

PopupMenu::PopupMenu(QToolBar* parent)
    : QWidget(parent)
    , toolButtons_(new QButtonGroup(this))
    , mainToolButton_(new QToolButton(parent))
    , toolBar_(parent)
    , slots_()
    , buttonSize_(30, 30)
    , pressedTimer_(0)
    , _PRESSED_TIMER_DELAY_(1500)
    , _SPACING_(0)
{
    setWindowFlags(Qt::Popup);

    toolButtons_->setExclusive(true);
    createLayout();
    createMainToolButton();
    setSize(buttonSize_);

    connect(toolBar_, &QToolBar::orientationChanged, this, &PopupMenu::changeOrientation);
}

void PopupMenu::addToolButton(QToolButton *toolButton, std::function<void()> slot)
{
    if (!toolButtons_->buttons().contains(toolButton))
    {
        layout()->addWidget(toolButton);
        toolButtons_->addButton(toolButton);
        setButtonSize(toolButton, buttonSize_);
        toolButton->setCheckable(true);

        slots_[toolButton] = [slot](bool checked){
            if (checked) slot();
        };

        if (toolButtons_->buttons().empty())
            setMainToolButton(toolButton);

        connect(toolButton, &QToolButton::clicked, this, &PopupMenu::hide);
        connect(toolButton, &QToolButton::toggled, this, slots_[toolButton]);
    }
}

void PopupMenu::setButtonSize(const QSize &size)
{
    buttonSize_ = size;
    setSize(size);

    for (auto &button : toolButtons_->buttons())
        setButtonSize(static_cast<QToolButton*>(button), size);
}

void PopupMenu::setMainToolButton(QToolButton *toolButton)
{
    disconnect(mainToolButton_, &QToolButton::triggered, nullptr, nullptr);
    connect(mainToolButton_, &QToolButton::triggered, this, slots_[toolButton]);

    mainToolButton_->setIcon(toolButton->icon());
}

void PopupMenu::hideEvent(QHideEvent*)
{
    if (auto *toolButton = static_cast<QToolButton*>(QObject::sender()))
        if (toolButton != mainToolButton_)
            setMainToolButton(toolButton);
}

void PopupMenu::showEvent(QShowEvent*)
{
    if (toolBar_->orientation() == Qt::Horizontal)
        move(mainToolButton_->mapToGlobal(QPoint(
            mainToolButton_->geometry().bottomLeft().x() - 0.3 * width(), // почему-то!!! виджет смещён на треть ширины
            mainToolButton_->geometry().bottomLeft().y()
        )));
    else
        move(mainToolButton_->mapToGlobal(QPoint(
            mainToolButton_->geometry().topRight().x(),
            mainToolButton_->geometry().topRight().y() - height() / 3   // аналогично с высотой
        )));
}

void PopupMenu::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == pressedTimer_)
    {
        show();
        killTimer(pressedTimer_);
        pressedTimer_ = 0;
    }
}

void PopupMenu::changeOrientation()
{
    createLayout();
    setSize(buttonSize_);
}

void PopupMenu::createLayout()
{
    if (layout() != nullptr)
    {
        for (auto &button : toolButtons_->buttons())
            layout()->removeWidget(button);

        delete layout();
    }

    if (toolBar_->orientation() == Qt::Horizontal)
        setLayout(new QVBoxLayout(this));
    else
        setLayout(new QHBoxLayout(this));

    layout()->setContentsMargins(_SPACING_, _SPACING_, _SPACING_, _SPACING_);
    layout()->setSpacing(_SPACING_);

    for (auto &button : toolButtons_->buttons())
        layout()->addWidget(button);
}

void PopupMenu::createMainToolButton()
{
    toolBar_->addWidget(mainToolButton_);
    mainToolButton_->setCheckable(true);
    connect(mainToolButton_, &QToolButton::pressed, this, [this](){
        pressedTimer_ = startTimer(_PRESSED_TIMER_DELAY_);
    });
    connect(mainToolButton_, &QToolButton::released, this, [this](){
        if (pressedTimer_ != 0)
        {
            killTimer(pressedTimer_);
            pressedTimer_ = 0;
        }
    });
}

void PopupMenu::setButtonSize(QToolButton *button, const QSize &size)
{
    button->setIconSize(size);
    button->setMaximumSize(size);
}

void PopupMenu::setSize(const QSize &size)
{
    if (toolBar_->orientation() == Qt::Horizontal)
        setFixedSize(size.width(), size.height() * toolButtons_->buttons().size());
    else
        setFixedSize(size.width() * toolButtons_->buttons().size(), size.height());
}
