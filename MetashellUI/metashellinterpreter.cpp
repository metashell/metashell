#include "metashellinterpreter.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

MetashellInterpreter::MetashellInterpreter(QObject *parent) :
    QObject(parent)
{
    m_metashell = std::make_shared<QProcess>();
    m_metashell->start("metashell", QStringList() << "--console=json");
    connect(m_metashell.get(), SIGNAL(readyRead()), this, SLOT(jsonArrived()));
    connect(m_metashell.get(), SIGNAL(finished(int)), this, SLOT(metashellQuit()));
}

MetashellInterpreter::~MetashellInterpreter()
{
    disconnect(m_metashell.get(), SIGNAL(finished(int)), this, SLOT(metashellQuit()));
    m_metashell->close();
}

void MetashellInterpreter::sendCommand(QString p_command)
{
    QString t_send = "{\"type\":\"cmd\", \"cmd\":\"" + p_command + "\"}\n";
    m_metashell->write(t_send.toUtf8());
    m_metashell->waitForBytesWritten();
}

void MetashellInterpreter::sendCodeCompletion(QString p_cc)
{
    QString t_send = "{\"type\":\"code_completion\", \"code\":\"" + p_cc + "\"}\n";
    m_metashell->write(t_send.toUtf8());
    m_metashell->waitForBytesWritten();
}

void MetashellInterpreter::processJson(QByteArray &p_input)
{
    QJsonDocument t_json = QJsonDocument::fromJson(p_input);
    if(t_json.isObject())
    {
        QJsonObject t_object = t_json.object();
        QString t_type = t_object["type"].toString();
        if(t_type == "comment")
        {
            QStringList t_comment;
            QJsonArray t_array = t_object["paragraphs"].toArray();
            for(QJsonValue it : t_array)
            {
                QJsonObject t_paragraph = it.toObject();
                QString t_firsline = t_paragraph["first_line_indentation"].toString();
                QString t_restline = t_paragraph["rest_of_lines_indentation"].toString();
                QStringList t_splitted = t_paragraph["content"].toString().split('\n');

                t_splitted[0] = t_firsline + t_splitted[0];
                for(int i = 1; i < t_splitted.size(); i++)
                {
                    t_splitted[i] = t_restline + t_splitted[i];
                }
                t_comment.push_back(t_splitted.join('\n'));
            }
            commentArrived(t_comment.join('\n'));
        }
        else if(t_type == "prompt")
        {
            QString t_prompt = t_object.value("prompt").toString();
            promptArrived(t_prompt);
        }
        else if(t_type == "backtrace")
        {
            QVector<JsonData> t_data;
            for(QJsonValue it : t_object["frames"].toArray())
            {
                QJsonObject t_frameObject = it.toObject();
                JsonData t_frame;
                t_frame.type = t_type;
                t_frame.name = t_frameObject["name"].toString();
                t_frame.kind = t_frameObject["kind"].toString();
                QStringList t_fileinfo = t_frameObject["source_location"].toString().split(':');
                if(t_fileinfo.size() == 3)
                {
                    t_frame.sourceFileName = t_fileinfo[0];
                    t_frame.sourceRow = t_fileinfo[1].toInt();
                    t_frame.sourceColumn = t_fileinfo[2].toInt();
                }
                t_fileinfo = t_frameObject["point_of_instantiation"].toString().split(':');
                if(t_fileinfo.size() == 3)
                {
                    t_frame.instantiationFileName = t_fileinfo[0];
                    t_frame.instantiationRow = t_fileinfo[1].toInt();
                    t_frame.instantiationColumn = t_fileinfo[2].toInt();
                }
                t_data.push_back(t_frame);
            }
            backTraceArrived(t_data);
        }
        else if(t_type == "call_graph")
        {
            QVector<JsonData> t_data;
            for(QJsonValue it : t_object["nodes"].toArray())
            {
                QJsonObject t_frameObject = it.toObject();
                JsonData t_frame;
                t_frame.type = t_type;
                t_frame.name = t_frameObject["name"].toString();
                t_frame.kind = t_frameObject["kind"].toString();
                QStringList t_fileinfo = t_frameObject["source_location"].toString().split(':');
                if(t_fileinfo.size() == 3)
                {
                    t_frame.sourceFileName = t_fileinfo[0];
                    t_frame.sourceRow = t_fileinfo[1].toInt();
                    t_frame.sourceColumn = t_fileinfo[2].toInt();
                }
                t_fileinfo = t_frameObject["point_of_instantiation"].toString().split(':');
                if(t_fileinfo.size() == 3)
                {
                    t_frame.instantiationFileName = t_fileinfo[0];
                    t_frame.instantiationRow = t_fileinfo[1].toInt();
                    t_frame.instantiationColumn = t_fileinfo[2].toInt();
                }
                t_frame.depth = t_frameObject["depth"].toInt();
                t_frame.children = t_frameObject["children"].toInt();
                t_data.push_back(t_frame);
            }
            callGraphArrived(t_data);
        }
        else if(t_type == "code_completion_result")
        {
            QStringList t_list;
            for(QJsonValue it : t_object["completions"].toArray())
            {
                t_list.push_back(it.toString());
            }
            codeCompletionArrived(t_list);
        }
        else if(t_type == "cpp_code")
        {
            QString t_code = t_object["cpp_code"].toString();
            cppCodeArrived(t_code);
        }
        else if(t_type == "error")
        {
            QString t_error = t_object["msg"].toString();
            errorArrived(t_error);
        }
        else if(t_type == "frame")
        {
            JsonData t_data;
            t_data.type = t_type;
            t_data.name = t_object["name"].toString();
            QStringList t_fileinfo = t_object["source_location"].toString().split(':');
            if(t_fileinfo.size() == 3)
            {
                t_data.sourceFileName = t_fileinfo[0];
                t_data.sourceRow = t_fileinfo[1].toInt();
                t_data.sourceColumn = t_fileinfo[2].toInt();
            }
            if(t_object.keys().contains("point_of_instantiation"))
            {
                t_fileinfo = t_object["point_of_instantiation"].toString().split(':');
                if(t_fileinfo.size() == 3)
                {
                    t_data.instantiationFileName = t_fileinfo[0];
                    t_data.instantiationRow = t_fileinfo[1].toInt();
                    t_data.instantiationColumn = t_fileinfo[2].toInt();
                }
            }
            if(t_object.keys().contains("kind"))
                t_data.kind = t_object["kind"].toString();
            if(t_object.keys().contains("time_taken"))
                t_data.timeTaken = t_object["time_taken"].toDouble();
            if(t_object.keys().contains("time_taken_ratio"))
                t_data.timeTakenRatio = t_object["time_taken_ratio"].toDouble();
            frameArrived(t_data);
        }
        else if(t_type == "raw_text")
        {
            QString t_rawtext = t_object["value"].toString();
            rawTextArrived(t_rawtext);
        }
        else if(t_type == "type")
        {
            QString t_name = t_object["name"].toString();
            typeArrived(t_name);
        }
    }
}

void MetashellInterpreter::jsonArrived()
{
    QList<QByteArray> t_input = m_metashell->readAll().split('\n');
    for(QByteArray it : t_input)
        if(!it.isEmpty())
            processJson(it);
}

void MetashellInterpreter::metashellQuit()
{
    clear();
    m_metashell->start("metashell", QStringList() << "--console=json");
}
