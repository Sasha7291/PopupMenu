#pragma once

#include <QButtonGroup>
#include <QPointer>
#include <QToolButton>
#include <QLayout>
#include <QWidget>

class QToolBar;

class PopupMenu : public QWidget
{
    Q_OBJECT

public:
    explicit PopupMenu(QToolBar* parent);
    PopupMenu(const PopupMenu&) = delete;
    PopupMenu(PopupMenu&&) = delete;
    ~PopupMenu() = default;

    PopupMenu &operator=(const PopupMenu&) = delete;
    PopupMenu &operator=(PopupMenu&&) = delete;

    void addToolButton(QToolButton *toolButton, std::function<void()> slot = [](){});
    [[nodiscard]] inline QSize buttonSize() const { return this->buttonSize_; }
    [[nodiscard]] inline QToolButton *mainToolButton() const { return this->mainToolButton_; }
    void setButtonSize(const QSize &size);
    inline void setButtonSize(const unsigned int width, const unsigned int height)
    { this->setButtonSize(QSize(width, height)); }
    void setMainToolButton(QToolButton *toolButton);
    [[nodiscard]] inline QToolBar *toolBar() const { return this->toolBar_; }

protected:
    virtual void hideEvent(QHideEvent*) override;
    virtual void showEvent(QShowEvent*) override;
    virtual void timerEvent(QTimerEvent *event) override;

private slots:
    void changeOrientation();

private:
    void createLayout();
    void createMainToolButton();
    void setButtonSize(QToolButton *button, const QSize &size);
    void setSize(const QSize &size);

    QPointer<QButtonGroup> toolButtons_;
    QPointer<QToolButton> mainToolButton_;
    QToolBar *toolBar_;

    QMap<QToolButton*, std::function<void(bool)>> slots_;
    QSize buttonSize_;
    int pressedTimer_;

    const int _PRESSED_TIMER_DELAY_;
    const int _SPACING_;

};
