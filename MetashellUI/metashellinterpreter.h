#ifndef METASHELLINTERPRETER_H
#define METASHELLINTERPRETER_H

#include <QObject>
#include <QVector>
#include <QProcess>
#include <memory>

struct JsonData
{
    JsonData()
    {
        sourceRow = sourceColumn = instantiationRow = instantiationColumn =
                    timeTaken = timeTakenRatio = depth = children = -1;
    }

    QString     type;

    //all
    QString     name;
    QString     sourceFileName;
    int         sourceRow;
    int         sourceColumn;
    QString     kind;
    QString     instantiationFileName;
    int         instantiationRow;
    int         instantiationColumn;

    //frame
    double      timeTaken;
    double      timeTakenRatio;

    //call_graph
    int         depth;
    int         children;
};

class MetashellInterpreter : public QObject
{
    Q_OBJECT
public:
    explicit MetashellInterpreter(QObject *parent = 0);
    ~MetashellInterpreter();

    void                                sendCommand(QString p_command);
    void                                sendCodeCompletion(QString p_cc);
private:
    void                                processJson(QByteArray &p_input);
    std::shared_ptr<QProcess>           m_metashell;
signals:
    void                                backTraceArrived(QVector<JsonData> p_backtrace);
    void                                callGraphArrived(QVector<JsonData> p_callGraph);
    void                                codeCompletionArrived(QStringList p_results);
    void                                commentArrived(QString p_comment);
    void                                cppCodeArrived(QString p_cppCode);
    void                                errorArrived(QString p_error);
    void                                frameArrived(JsonData p_frame);
    void                                promptArrived(QString p_prompt);
    void                                rawTextArrived(QString p_rawText);
    void                                typeArrived(QString p_type);
    void                                clear();
private slots:
    void                                jsonArrived();
    void                                metashellQuit();
};

#endif // METASHELLINTERPRETER_H
