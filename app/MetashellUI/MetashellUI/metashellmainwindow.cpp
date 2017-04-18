#include "metashellmainwindow.h"
#include "ui_metashellmainwindow.h"
#include <QKeyEvent>

MetashellMainWindow::MetashellMainWindow(QWidget* parent)
  : QMainWindow(parent), ui(new Ui::MetashellMainWindow)
{
  ui->setupUi(this);
  m_interpeter = std::make_shared<MetashellInterpreter>();
  m_lastKey = -1;

  m_cw = std::make_shared<ConsoleWidget>();
  ui->consoleScrollBar->setWidget(m_cw.get());
  m_cw->setScrollBar(ui->consoleScrollBar);

  connect(m_cw.get(), SIGNAL(sendCommand(QString)), this,
          SLOT(sendCommand(QString)));
  connect(m_cw.get(), SIGNAL(sendCodeCompletion(QString)), this,
          SLOT(sendCodeCompletion(QString)));

  connect(m_interpeter.get(), SIGNAL(backTraceArrived(QVector<JsonData>)), this,
          SLOT(backTraceArrived(QVector<JsonData>)));
  connect(m_interpeter.get(), SIGNAL(callGraphArrived(QVector<JsonData>)), this,
          SLOT(callGraphArrived(QVector<JsonData>)));
  connect(m_interpeter.get(), SIGNAL(codeCompletionArrived(QStringList)), this,
          SLOT(codeCompletionArrived(QStringList)));
  connect(m_interpeter.get(), SIGNAL(commentArrived(QString)), this,
          SLOT(commentArrived(QString)));
  connect(m_interpeter.get(), SIGNAL(cppCodeArrived(QString)), this,
          SLOT(cppCodeArrived(QString)));
  connect(m_interpeter.get(), SIGNAL(errorArrived(QString)), this,
          SLOT(errorArrived(QString)));
  connect(m_interpeter.get(), SIGNAL(frameArrived(JsonData)), this,
          SLOT(frameArrived(JsonData)));
  connect(m_interpeter.get(), SIGNAL(promptArrived(QString)), this,
          SLOT(promptArrived(QString)));
  connect(m_interpeter.get(), SIGNAL(rawTextArrived(QString)), this,
          SLOT(rawTextArrived(QString)));
  connect(m_interpeter.get(), SIGNAL(typeArrived(QString)), this,
          SLOT(typeArrived(QString)));
  connect(m_interpeter.get(), SIGNAL(clear()), this, SLOT(clear()));
  // connect(ui->le_input, SIGNAL(returnPressed()), this, SLOT(sendCommand()));
  // connect(ui->le_input, SIGNAL(textEdited(QString)), this,
  // SLOT(textChanged(QString)));
}

MetashellMainWindow::~MetashellMainWindow() { delete ui; }

void MetashellMainWindow::sendCodeCompletion(QString p_cc)
{
  m_interpeter->sendCodeCompletion(p_cc);
}

void MetashellMainWindow::clear() { m_cw->clear(); }

void MetashellMainWindow::sendCommand(QString p_command)
{
  m_interpeter->sendCommand(p_command);
}

void MetashellMainWindow::backTraceArrived(QVector<JsonData> p_backtrace)
{
  int t_row = 0;
  for (JsonData it : p_backtrace)
  {
    QString t_append = "#" + QString::number(t_row) + " " + it.name + " at " +
                       it.sourceFileName + ":" + QString::number(it.sourceRow) +
                       QString::number(it.sourceColumn) + " (" + it.kind +
                       " from " + it.instantiationFileName + ":" +
                       QString::number(it.instantiationRow) + ":" +
                       QString::number(it.instantiationColumn) + ")";
    m_cw->addText(t_append);
    t_row++;
  }
}

void MetashellMainWindow::callGraphArrived(QVector<JsonData> p_callGraph)
{
  for (JsonData it : p_callGraph)
  {
    QString t_append;
    for (int i = 0; i < it.depth; i++)
      t_append.append("|");
    t_append += it.name + "(" + it.kind + ")";
    m_cw->addText(t_append);
  }
}

void MetashellMainWindow::codeCompletionArrived(QStringList p_results)
{
  m_cw->codeCompletionArrived(p_results);
}

void MetashellMainWindow::commentArrived(QString p_comment)
{
  m_cw->addComment(p_comment);
}

void MetashellMainWindow::cppCodeArrived(QString p_cppCode)
{
  m_cw->addText(p_cppCode);
}

void MetashellMainWindow::errorArrived(QString p_error)
{
  m_cw->addError(p_error);
}

void MetashellMainWindow::frameArrived(JsonData p_frame)
{
  QString t_append = p_frame.name + " at " + p_frame.sourceFileName + ":" +
                     QString::number(p_frame.sourceRow) +
                     QString::number(p_frame.sourceColumn);
  if (p_frame.kind != "")
  {
    t_append += " (" + p_frame.kind + " from " + p_frame.instantiationFileName +
                ":" + QString::number(p_frame.instantiationRow) + ":" +
                QString::number(p_frame.instantiationColumn) + ")";
  }
  m_cw->addText(t_append);
}

void MetashellMainWindow::promptArrived(QString p_prompt)
{
  m_cw->promptArrived(p_prompt);
}

void MetashellMainWindow::rawTextArrived(QString p_rawText)
{
  m_cw->addText(p_rawText);
}

void MetashellMainWindow::typeArrived(QString p_type) { m_cw->addText(p_type); }

void MetashellMainWindow::textChanged(QString p_text)
{
  if (p_text.split('\n').size() != 1)
  {
    QStringList t_list = p_text.split('\n');
    QString t_lastline = t_list.last();
    // ui->le_input->setText(t_lastline);
    t_list.removeOne(t_lastline);

    for (QString it : t_list)
    {
      // m_cw->addText(ui->lbl_prompt->text() + it);
      m_interpeter->sendCommand(it);
      // appendToHistory(it);
    }
  }
}
