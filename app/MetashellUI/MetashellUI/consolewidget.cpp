#include "consolewidget.h"
#include <QPainter>
#include <QKeyEvent>
#include <QKeySequence>
#include <QMouseEvent>
#include <QScrollBar>
#include <QApplication>

#include <QDebug>

ConsoleWidget::ConsoleWidget(QWidget* parent) : QWidget(parent)
{
  m_scrollbar = 0;
  setAutoFillBackground(true);
  setStyleSheet("background-color: black;");
  setFocusPolicy(Qt::StrongFocus);

  QFont t_font("Monospace");
  t_font.setBold(true);
  t_font.setPointSize(12);
  QFontMetricsF t_metrics(t_font);
  m_charWidth = t_metrics.averageCharWidth();
  m_charHeight = t_metrics.height();
  m_pressedAnythingSinceLastTab = true;

  m_lineEdit = "";
  m_linePos = 0;
  m_paintCursor = true;
  m_rebuildDisplayedText = true;

  m_promptTimer = std::make_shared<QTimer>();
  m_promptTimer->setInterval(600);
  m_promptTimer->start();

  m_clipBoard = QApplication::clipboard();

  m_showConsole = false;
  m_scrollDown = false;
  connect(
      m_promptTimer.get(), SIGNAL(timeout()), this, SLOT(promptTimerTicked()));
}

void ConsoleWidget::setScrollBar(QScrollArea* p_area)
{
  m_scrollbar = p_area;
  m_scrollbar->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
}

void ConsoleWidget::addText(QString p_text)
{
  Line t_new;
  t_new.content = p_text;
  t_new.type = LineType::Normal;
  m_content.push_back(t_new);
  m_rebuildDisplayedText = true;

  m_scrollDown = true;
  repaint();
}

void ConsoleWidget::addComment(QString p_text)
{
  Line t_new;
  t_new.content = p_text;
  t_new.type = LineType::Comment;
  m_content.push_back(t_new);
  m_rebuildDisplayedText = true;

  m_scrollDown = true;
  repaint();
}

void ConsoleWidget::addError(QString p_text)
{
  Line t_new;
  t_new.content = p_text;
  t_new.type = LineType::Error;
  m_content.push_back(t_new);
  m_rebuildDisplayedText = true;

  m_scrollDown = true;
  repaint();
}

void ConsoleWidget::clear() { m_content.clear(); }

void ConsoleWidget::codeCompletionArrived(QStringList p_list)
{
  if (p_list.size() == 1)
  {
    m_lineEdit = m_lineEdit.mid(0, m_linePos) + p_list.first() +
                 m_lineEdit.mid(m_linePos);
    m_linePos += p_list.first().size();
  }
  else if (!m_pressedAnythingSinceLastTab)
  {
    QString t_toComplete;
    int t_pos = m_linePos - 1;
    while (t_pos >= 0 && m_lineEdit[t_pos] != ' ')
      t_toComplete = m_lineEdit[t_pos--] + t_toComplete;

    for (QString& it : p_list)
    {
      it = t_toComplete + it;
    }
    addText(m_prompt + m_lineEdit);
    addText(p_list.join(' '));
    m_pressedAnythingSinceLastTab = true;
  }
  else
    m_pressedAnythingSinceLastTab = false;
}

void ConsoleWidget::promptArrived(QString p_prompt)
{
  m_showConsole = true;
  m_prompt = p_prompt;
  rebuildPromptText();
  repaint();
}

void ConsoleWidget::keyPressEvent(QKeyEvent* p_event)
{
  if (p_event->key() != Qt::Key_Tab)
    m_pressedAnythingSinceLastTab = true;

  QKeySequence t_seq(p_event->key());
  if (p_event->modifiers() == Qt::ControlModifier)
  {
    if (p_event->key() == Qt::Key_C)
    {
      QString t_toCopy;
      for (int i = m_selection.fromRow; i <= m_selection.toRow; i++)
      {
        int t_fromChar = 0;
        if (m_selection.fromRow == i)
          t_fromChar = m_selection.fromChar;
        if (m_selection.toRow == i)
          t_toCopy += joinLineText(m_displayedText[i])
                          .mid(t_fromChar, m_selection.toChar - t_fromChar) +
                      "\n";
        else
          t_toCopy += joinLineText(m_displayedText[i]).mid(t_fromChar) + "\n";
      }
      m_clipBoard->setText(t_toCopy);
    }
    else if (p_event->key() == Qt::Key_V)
    {
      QString t_clipBoard = m_clipBoard->text();
      m_lineEdit.replace("\\\n", "\n");
      m_lineEdit = m_lineEdit.mid(0, m_linePos) + t_clipBoard +
                   m_lineEdit.mid(m_linePos);
      m_lineEdit.replace('\n', "\\\n");
      m_linePos += t_clipBoard.size();
      m_scrollDown = true;
    }
  }
  else if (p_event->key() == Qt::Key_Left)
  {
    if (m_linePos > 0)
      m_linePos--;
  }
  else if (p_event->key() == Qt::Key_Right)
  {
    if (m_linePos < m_lineEdit.size())
      m_linePos++;
  }
  else if (p_event->key() == Qt::Key_Backspace)
  {
    if (m_linePos > 0)
    {
      m_lineEdit = m_lineEdit.mid(0, m_linePos - 1) + m_lineEdit.mid(m_linePos);
      m_linePos--;
      repaint();
    }
  }
  else if (p_event->key() == Qt::Key_Return)
  {
    if (m_lineEdit.endsWith('\\'))
    {
      m_lineEdit += "\n";
    }
    else
    {
      sendCommand(m_lineEdit);
      addText(m_prompt + m_lineEdit);
      appendToHistory(m_lineEdit);
      m_lineEdit.clear();
      m_linePos = 0;
    }
  }
  else if (p_event->key() == Qt::Key_Up)
  {
    if (!m_history.isEmpty())
    {
      if (m_history.size() > 1 && !m_future.isEmpty())
      {
        m_future.push_back(m_history.last());
        m_history.pop_back();
      }
      else if (m_future.isEmpty())
      {
        m_future.push_back(m_lineEdit);
      }
      m_lineEdit = m_history.last();
      m_linePos = m_lineEdit.size();
    }
  }
  else if (p_event->key() == Qt::Key_Down)
  {
    if (!m_future.isEmpty())
    {
      QString t_next = m_future.last();
      m_future.pop_back();
      if (!m_future.isEmpty())
      {
        m_history.push_back(t_next);
      }
      m_lineEdit = t_next;
      m_linePos = m_lineEdit.size();
    }
  }
  else if (p_event->key() == Qt::Key_Tab)
  {
    QString t_toComplete;
    int t_pos = m_linePos - 1;
    while (t_pos >= 0 && m_lineEdit[t_pos] != ' ')
      t_toComplete = m_lineEdit[t_pos--] + t_toComplete;

    sendCodeCompletion(t_toComplete);
  }
  else if (t_seq.toString().size() == 1 || p_event->key() == Qt::Key_Space)
  {
    m_lineEdit.replace("\\\n", "\n");
    m_lineEdit = m_lineEdit.mid(0, m_linePos) + p_event->text() +
                 m_lineEdit.mid(m_linePos);
    m_lineEdit.replace('\n', "\\\n");
    m_linePos++;
    m_scrollDown = true;
  }
  rebuildPromptText();
  repaint();
}

void ConsoleWidget::mousePressEvent(QMouseEvent* p_event)
{
  int t_presumedRow = p_event->pos().y() / m_charHeight;
  int t_presumedColumn = p_event->pos().x() / m_charWidth;
  m_selection.startingRow = m_selection.fromRow = m_selection.toRow =
      t_presumedRow;
  m_selection.startingChar = m_selection.fromChar = m_selection.toChar =
      t_presumedColumn;
}

void ConsoleWidget::mouseMoveEvent(QMouseEvent* p_event)
{
  int t_presumedRow = p_event->pos().y() / m_charHeight;
  int t_presumedColumn = p_event->pos().x() / m_charWidth;
  if (m_selection.startingRow > t_presumedRow)
  {
    m_selection.fromRow = t_presumedRow;
    m_selection.toRow = m_selection.startingRow;
    m_selection.fromChar = t_presumedColumn;
    m_selection.toChar = m_selection.startingChar;
  }
  else if (m_selection.startingRow == t_presumedRow &&
           m_selection.startingChar > t_presumedColumn)
  {
    m_selection.fromChar = t_presumedColumn;
    m_selection.toChar = m_selection.startingChar;
  }
  else
  {
    m_selection.fromRow = m_selection.startingRow;
    m_selection.fromChar = m_selection.startingChar;
    m_selection.toRow = t_presumedRow;
    m_selection.toChar = t_presumedColumn;
  }
  if (m_selection.fromRow < 0)
    m_selection.fromRow = 0;
  else if (m_selection.fromRow >=
           m_displayedText.size() -
               1) // It does need the equal, because of fromChar
  {
    m_selection.fromRow = m_displayedText.size() - 1;
    if (m_displayedText.size() != 0 &&
        m_selection.fromChar >= lineLength(m_displayedText.last()) - 1)
      m_selection.fromChar = lineLength(m_displayedText.last()) - 1;
  }
  if (m_selection.fromChar < 0)
    m_selection.fromChar = 0;
  if (m_selection.toRow < 0)
    m_selection.toRow = 0;
  else if (m_selection.toRow >= m_displayedText.size() - 1)
  {
    m_selection.toRow = m_displayedText.size() - 1;
    if (m_displayedText.size() != 0 &&
        m_selection.toChar >= lineLength(m_displayedText.last()) - 1)
      m_selection.toChar = lineLength(m_displayedText.last()) - 1;
  }
  if (m_selection.toChar < 0)
    m_selection.toChar = 0;

  repaint();
}

void ConsoleWidget::mouseReleaseEvent(QMouseEvent* p_event)
{
  Q_UNUSED(p_event)
}

void ConsoleWidget::paintEvent(QPaintEvent* p_event)
{
  Q_UNUSED(p_event)
  QPainter t_painter(this);
  QPen t_pen(Qt::white);
  QPen t_highlightpen(Qt::black);
  QBrush t_backgroundBrush(Qt::white);
  QFont t_font("Monospace");
  t_font.setBold(true);
  t_font.setPointSize(12);

  t_painter.setFont(t_font);
  t_painter.fillRect(rect(), Qt::black);

  t_painter.setBackground(t_backgroundBrush);

  if (m_rebuildDisplayedText)
    rebuildDisplayedText();

  int t_charperline = width() / m_charWidth - 1;
  float t_actualheight = 0;
  int t_rownum = 0;
  int t_promptPos = 0;
  int t_correctedLinePos = m_linePos + m_prompt.size();
  for (QVector<TextPart> it_textparts : m_displayedText)
  {
    int t_inlinePos = 0;
    for (TextPart it : it_textparts)
    {
      t_pen.setColor(it.color);
      t_painter.setPen(t_pen);
      if (t_rownum >= m_selection.fromRow && t_rownum <= m_selection.toRow)
      {
        int t_inlinePosFrom = t_inlinePos + m_selection.fromChar;
        if (t_rownum > m_selection.fromRow)
        {
          t_inlinePosFrom = t_inlinePos;
        }
        int t_inlineLength = m_selection.toChar - t_inlinePosFrom;
        if (t_rownum < m_selection.toRow)
        {
          t_inlineLength = t_charperline;
        }

        if (t_rownum >= m_contentLines && m_paintCursor)
        {
          /// FROM 0 TO t_inlinePosFrom
          if (t_correctedLinePos - t_promptPos < t_inlinePosFrom)
          {
            t_painter.drawText(
                QRectF(
                    0, t_actualheight, width(), t_actualheight + m_charHeight),
                it.text.mid(0, t_correctedLinePos - t_promptPos));
            t_painter.setPen(t_highlightpen);
            t_painter.setPen(t_highlightpen);
            t_backgroundBrush.setColor(Qt::white);
            t_painter.setBackground(t_backgroundBrush);
            t_painter.setBackgroundMode(Qt::OpaqueMode);
            t_painter.drawText(
                QRectF((t_correctedLinePos - t_promptPos) * m_charWidth,
                       t_actualheight, width(), t_actualheight + m_charHeight),
                it.text.mid(t_correctedLinePos - t_promptPos, 1));
            t_painter.setPen(t_pen);
            t_painter.setBackgroundMode(Qt::TransparentMode);
            t_painter.drawText(
                QRectF((t_correctedLinePos - t_promptPos + 1) * m_charWidth,
                       t_actualheight, width(), t_actualheight + m_charHeight),
                it.text.mid(
                    t_correctedLinePos - t_promptPos + 1, t_inlinePosFrom));
          }
          else
            t_painter.drawText(QRectF(0, t_actualheight, width(),
                                      t_actualheight + m_charHeight),
                               it.text.mid(0, t_inlinePosFrom));

          /// FROM t_inlinePosFrom to selection end
          if (t_correctedLinePos - t_promptPos >= t_inlinePosFrom &&
              t_correctedLinePos - t_promptPos <
                  t_inlinePosFrom + t_inlineLength)
          {
            t_painter.setPen(t_highlightpen);
            t_backgroundBrush.setColor(t_pen.color());
            t_painter.setBackground(t_backgroundBrush);
            t_painter.setBackgroundMode(Qt::OpaqueMode);
            t_painter.drawText(
                QRectF(t_inlinePosFrom * m_charWidth, t_actualheight, width(),
                       t_actualheight + m_charHeight),
                it.text.mid(t_inlinePosFrom, t_correctedLinePos - t_promptPos -
                                                 t_inlinePosFrom));
            t_painter.setPen(t_pen);
            t_painter.setBackgroundMode(Qt::TransparentMode);
            t_painter.drawText(
                QRectF((t_correctedLinePos - t_promptPos) * m_charWidth,
                       t_actualheight, width(), t_actualheight + m_charHeight),
                it.text.mid(t_correctedLinePos - t_promptPos, 1));
            t_painter.setPen(t_highlightpen);
            t_painter.setBackgroundMode(Qt::OpaqueMode);
            t_painter.drawText(
                QRectF((t_correctedLinePos - t_promptPos + 1) * m_charWidth,
                       t_actualheight, width(), t_actualheight + m_charHeight),
                it.text.mid(t_correctedLinePos - t_promptPos + 1,
                            t_inlinePosFrom + t_inlineLength -
                                (t_correctedLinePos - t_promptPos + 1)));
          }
          else
          {
            t_painter.setPen(t_highlightpen);
            t_backgroundBrush.setColor(t_pen.color());
            t_painter.setBackground(t_backgroundBrush);
            t_painter.setBackgroundMode(Qt::OpaqueMode);
            t_painter.drawText(
                QRectF(t_inlinePosFrom * m_charWidth, t_actualheight, width(),
                       t_actualheight + m_charHeight),
                it.text.mid(t_inlinePosFrom, t_inlineLength));
          }

          /// FROM selection end to end of line
          if (t_correctedLinePos - t_promptPos >=
              t_inlinePosFrom + t_inlineLength)
          {
            t_painter.setPen(t_pen);
            t_painter.setBackgroundMode(Qt::TransparentMode);
            t_painter.drawText(
                QRectF((t_inlinePosFrom + t_inlineLength) * m_charWidth,
                       t_actualheight, width(), t_actualheight + m_charHeight),
                it.text.mid(
                    t_inlinePosFrom + t_inlineLength,
                    t_correctedLinePos - (t_inlinePosFrom + t_inlineLength)));
            t_painter.setPen(t_highlightpen);
            t_backgroundBrush.setColor(Qt::white);
            t_painter.setBackground(t_backgroundBrush);
            t_painter.setBackgroundMode(Qt::OpaqueMode);
            t_painter.drawText(
                QRectF((t_correctedLinePos - t_promptPos) * m_charWidth,
                       t_actualheight, width(), t_actualheight + m_charHeight),
                it.text.mid(t_correctedLinePos - t_promptPos, 1));
            t_painter.setPen(t_pen);
            t_painter.setBackgroundMode(Qt::TransparentMode);
            t_painter.drawText(
                QRectF((t_correctedLinePos - t_promptPos + 1) * m_charWidth,
                       t_actualheight, width(), t_actualheight + m_charHeight),
                it.text.mid(t_correctedLinePos - t_promptPos + 1));
          }
          else
          {
            t_painter.setPen(t_pen);
            t_painter.setBackgroundMode(Qt::TransparentMode);
            t_painter.drawText(
                QRectF((t_inlinePosFrom + t_inlineLength) * m_charWidth,
                       t_actualheight, width(), t_actualheight + m_charHeight),
                it.text.mid(t_inlinePosFrom + t_inlineLength));
          }
        }
        else
        {
          t_painter.drawText(
              QRectF(0, t_actualheight, width(), t_actualheight + m_charHeight),
              it.text.mid(0, t_inlinePosFrom));
          t_painter.setPen(t_highlightpen);
          t_backgroundBrush.setColor(t_pen.color());
          t_painter.setBackground(t_backgroundBrush);
          t_painter.setBackgroundMode(Qt::OpaqueMode);
          t_painter.drawText(
              QRectF(t_inlinePosFrom * m_charWidth, t_actualheight, width(),
                     t_actualheight + m_charHeight),
              it.text.mid(t_inlinePosFrom, t_inlineLength));
          t_painter.setPen(t_pen);
          t_painter.setBackgroundMode(Qt::TransparentMode);
          t_painter.drawText(
              QRectF((t_inlinePosFrom + t_inlineLength) * m_charWidth,
                     t_actualheight, width(), t_actualheight + m_charHeight),
              it.text.mid(t_inlinePosFrom + t_inlineLength));
        }
      }
      else
      {
        if (t_rownum >= m_contentLines && m_paintCursor &&
            m_linePos + m_prompt.size() >= t_promptPos &&
            m_linePos + m_prompt.size() < t_promptPos + t_charperline)
        {
          t_painter.drawText(
              QRectF(0, t_actualheight, width(), t_actualheight + m_charHeight),
              it.text.mid(0, t_correctedLinePos - t_promptPos));
          t_painter.setPen(t_highlightpen);
          t_backgroundBrush.setColor(Qt::white);
          t_painter.setBackground(t_backgroundBrush);
          t_painter.setBackgroundMode(Qt::OpaqueMode);
          t_painter.drawText(
              QRectF((t_correctedLinePos - t_promptPos) * m_charWidth,
                     t_actualheight, width(), t_actualheight + m_charHeight),
              it.text.mid(t_correctedLinePos - t_promptPos, 1));
          t_painter.setPen(t_pen);
          t_painter.setBackgroundMode(Qt::TransparentMode);
          t_painter.drawText(
              QRectF((t_correctedLinePos - t_promptPos + 1) * m_charWidth,
                     t_actualheight, width(), t_actualheight + m_charHeight),
              it.text.mid(t_correctedLinePos - t_promptPos + 1));
        }
        else
          t_painter.drawText(QRectF(t_inlinePos * m_charWidth, t_actualheight,
                                    width(), t_actualheight + m_charHeight),
                             it.text);
      }
      if (t_rownum >= m_contentLines)
        t_promptPos += it.text.size();
      t_inlinePos += it.text.size();
    }
    t_actualheight += m_charHeight;
    t_rownum++;
  }

  m_rebuildDisplayedText = false;
  setMinimumHeight(t_actualheight);
  if (m_scrollDown)
  {
    scrollDown();
    m_scrollDown = false;
  }
}

void ConsoleWidget::resizeEvent(QResizeEvent* p_event)
{
  // Removing selection on resize
  if (p_event->oldSize().width() != p_event->size().width())
  {
    m_rebuildDisplayedText = true;
    m_selection.fromRow = m_selection.fromChar = m_selection.toRow =
        m_selection.toChar = m_selection.startingRow = m_selection.startingChar;
  }
}

void ConsoleWidget::scrollDown()
{
  if (m_scrollbar != 0)
    m_scrollbar->verticalScrollBar()->setSliderPosition(
        m_scrollbar->verticalScrollBar()->maximum());
}

void ConsoleWidget::appendToHistory(QString p_text)
{
  while (m_future.size() > 1)
  {
    m_history.push_back(m_future.last());
    m_future.pop_back();
  }
  m_future.clear();
  if (p_text.trimmed() != "" &&
      (m_history.isEmpty() ||
       m_history.last().toLower().trimmed() != p_text.toLower().trimmed()))
    m_history.push_back(p_text);
}

QString ConsoleWidget::joinLineText(QVector<TextPart> p_vector)
{
  QString t_return;
  for (TextPart it : p_vector)
  {
    t_return += it.text;
  }
  return t_return;
}

int ConsoleWidget::lineLength(QVector<ConsoleWidget::TextPart> p_vector)
{
  int t_return = 0;
  for (TextPart it : p_vector)
  {
    t_return += it.text.size();
  }
  return t_return;
}

void ConsoleWidget::rebuildDisplayedText()
{
  m_displayedText.clear();
  m_lastLineType = LineType::Normal;
  int t_charperline = width() / m_charWidth - 1;
  for (Line& it_line : m_content)
  {
    QColor t_color;
    it_line.displayedContent.clear();
    if (it_line.type != LineType::Comment &&
        m_lastLineType == LineType::Comment)
    {
      m_displayedText.push_back(QVector<TextPart>()
                                << TextPart(" */", Qt::darkGreen));
    }

    if (it_line.type == LineType::Normal)
      t_color = Qt::white;
    if (it_line.type == LineType::Error)
      t_color = Qt::red;
    if (it_line.type == LineType::Comment)
      t_color = Qt::darkGreen;

    if (it_line.type == LineType::Comment &&
        m_lastLineType != LineType::Comment)
    {
      m_displayedText.push_back(QVector<TextPart>()
                                << TextPart("/* ", Qt::darkGreen));
    }
    m_lastLineType = it_line.type;
    for (QString it : it_line.content.split('\n'))
    {
      int t_pos = 0;
      while (t_pos < it.size())
      {
        QString t_part;
        int t_charsWritten = 0;
        if (it_line.type == LineType::Comment)
        {
          t_part = " * " + it.mid(t_pos, t_charperline - 3);
          t_charsWritten = t_charperline - 3;
        }
        else
        {
          t_part = it.mid(t_pos, t_charperline);
          t_charsWritten = t_charperline;
        }
        m_displayedText.push_back(QVector<TextPart>()
                                  << TextPart(t_part, t_color));
        t_pos += t_charsWritten;
      }
    }
  }

  if (m_lastLineType == LineType::Comment)
  {
    m_displayedText.push_back(QVector<TextPart>()
                              << TextPart(" */", Qt::darkGreen));
  }
  m_contentLines = m_displayedText.size();
  if (m_showConsole)
    rebuildPromptText();
}

void ConsoleWidget::rebuildPromptText()
{
  m_displayedText = m_displayedText.mid(0, m_contentLines);
  if (m_showConsole)
  {
    int t_pos = 0;
    int t_charperline = width() / m_charWidth - 1;
    QString t_prompt = m_prompt + m_lineEdit + " ";
    while (t_pos < t_prompt.size())
    {
      QString t_part = t_prompt.mid(t_pos, t_charperline).split('\n').first();
      m_displayedText.push_back(QVector<TextPart>()
                                << TextPart(t_part, Qt::white));
      t_pos += t_part.length();
      if (t_part.size() != t_charperline)
        t_pos += 1;
    }
    m_scrollDown = true;
  }
}

void ConsoleWidget::promptTimerTicked()
{
  m_paintCursor = !m_paintCursor;
  repaint();
}
