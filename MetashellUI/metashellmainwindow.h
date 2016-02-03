#ifndef METASHELLMAINWINDOW_H
#define METASHELLMAINWINDOW_H

#include <QMainWindow>
#include "metashellinterpreter.h"

#include "consolewidget.h"

namespace Ui {
class MetashellMainWindow;
}

class MetashellMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MetashellMainWindow(QWidget *parent = 0);
    ~MetashellMainWindow();
    void                                        keyPressEvent(QKeyEvent* p_event);
private:
    void                                        appendToHistory(QString p_text);
    void                                        sendCodeCompletion();


    Ui::MetashellMainWindow*                    ui;
    std::shared_ptr<MetashellInterpreter>       m_interpeter;
    std::shared_ptr<ConsoleWidget>              m_cw;
    QVector<QString>                            m_history;
    QVector<QString>                            m_future;
    int                                         m_lastKey;
private slots:
    void                                        clear();
    void                                        sendCommand();
    void                                        backTraceArrived(QVector<JsonData> p_backtrace);
    void                                        callGraphArrived(QVector<JsonData> p_callGraph);
    void                                        codeCompletionArrived(QStringList p_results);
    void                                        commentArrived(QString p_comment);
    void                                        cppCodeArrived(QString p_cppCode);
    void                                        errorArrived(QString p_error);
    void                                        frameArrived(JsonData p_frame);
    void                                        promptArrived(QString p_prompt);
    void                                        rawTextArrived(QString p_rawText);
    void                                        typeArrived(QString p_type);
    void                                        textChanged(QString p_text);
};

#endif // METASHELLMAINWINDOW_H
