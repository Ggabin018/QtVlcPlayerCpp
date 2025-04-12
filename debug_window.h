#ifndef DEBUGWINDOW_H
#define DEBUGWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <functional>
#include <vector>

class DebugWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit DebugWindow(QWidget *parent = nullptr);
    ~DebugWindow();

    void addDebugMessage(const QString &message);

    // Add a function to set up debug buttons from a vector of functions
    void setupDebugFunctions(const std::vector<std::pair<QString, std::function<QString()>>> &debugFunctions);

private:
    QTextEdit *debugTextEdit;
    QWidget *buttonsWidget;
    QVBoxLayout *mainLayout;
    QHBoxLayout *buttonsLayout;
    QScrollArea *buttonScrollArea;
};

#endif // DEBUGWINDOW_H
