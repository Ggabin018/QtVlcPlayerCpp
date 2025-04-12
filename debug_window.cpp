#include "debug_window.h"

DebugWindow::DebugWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Debug Window");
    resize(800, 600);

           // Create a central widget
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

           // Create main layout
    mainLayout = new QVBoxLayout(centralWidget);

           // Create a scroll area for buttons
    buttonScrollArea = new QScrollArea(this);
    buttonScrollArea->setWidgetResizable(true);
    buttonScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    buttonScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    buttonScrollArea->setMaximumHeight(100); // Adjust as needed

    // Create a widget to hold the buttons
    buttonsWidget = new QWidget(buttonScrollArea);
    buttonsLayout = new QHBoxLayout(buttonsWidget);
    buttonScrollArea->setWidget(buttonsWidget);

    mainLayout->addWidget(buttonScrollArea);

           // Create the text edit for debug messages
    debugTextEdit = new QTextEdit(this);
    debugTextEdit->setReadOnly(true);
    mainLayout->addWidget(debugTextEdit);
}

DebugWindow::~DebugWindow()
{
}

void DebugWindow::addDebugMessage(const QString &message)
{
    debugTextEdit->append(message);
}

void DebugWindow::setupDebugFunctions(const std::vector<std::pair<QString, std::function<QString()>>> &debugFunctions)
{
    // Clear any existing buttons
    QLayoutItem* item;
    while ((item = buttonsLayout->takeAt(0)) != nullptr)
    {
        delete item->widget();
        delete item;
    }

           // Create buttons for each debug function
    for (const auto &funcPair : debugFunctions)
    {
        QPushButton *button = new QPushButton(funcPair.first, buttonsWidget);

        // Connect button to lambda that calls the function and adds results to debug text
        connect(button, &QPushButton::clicked, this, [this, funcPair]() {
            QString result = funcPair.second(); // Call the function
            this->addDebugMessage("--- " + funcPair.first + " ---");
            this->addDebugMessage(result);
            this->addDebugMessage(""); // Add an empty line for readability
        });

        buttonsLayout->addWidget(button);
    }

    // Add a stretch at the end to keep buttons aligned to the left
    buttonsLayout->addStretch();
}
