#include "consolewidget.h"
#include <QPainter>
#include <QScrollBar>

ConsoleWidget::ConsoleWidget(QWidget *parent) :
    QWidget(parent)
{
    m_scrollbar = 0;
    setAutoFillBackground(true);
    setStyleSheet("background-color: black;");
}

void ConsoleWidget::setScrollBar(QScrollArea *p_area)
{
    m_scrollbar = p_area;
}

void ConsoleWidget::addText(QString p_text)
{
    Line t_new;
    t_new.content = p_text;
    t_new.type = LineType::Normal;
    m_content.push_back(t_new);

    repaint();
    scrollDown();
}

void ConsoleWidget::addComment(QString p_text)
{
    Line t_new;
    t_new.content = p_text;
    t_new.type = LineType::Comment;
    m_content.push_back(t_new);

    repaint();
    scrollDown();
}

void ConsoleWidget::addError(QString p_text)
{
    Line t_new;
    t_new.content = p_text;
    t_new.type = LineType::Error;
    m_content.push_back(t_new);

    repaint();
    scrollDown();
}

void ConsoleWidget::clear()
{
    m_content.clear();
}

void ConsoleWidget::paintEvent(QPaintEvent *p_event)
{
    Q_UNUSED(p_event)
    QPainter t_painter(this);
    QPen t_pen(Qt::white);
    QFont t_font("Monospace");
    t_font.setBold(true);
    t_font.setPointSize( 12 );

    t_painter.setFont(t_font);
    t_painter.fillRect(rect(), Qt::black);

    QFontMetrics t_metrics(t_font);
    int t_charperline = width() / t_metrics.averageCharWidth();
    int t_fontheight = t_metrics.height();
    int t_actualheight = 0;

    for(Line it_line : m_content)
    {
        if(it_line.type == LineType::Normal)
            t_pen.setColor(Qt::white);
        if(it_line.type == LineType::Error)
            t_pen.setColor(Qt::red);
        if(it_line.type == LineType::Comment)
            t_pen.setColor(Qt::darkGreen);
        t_painter.setPen(t_pen);
        for(QString it : it_line.content.split('\n'))
        {
            int t_pos = 0;
            while(t_pos < it.size())
            {
                QString t_part = it.mid(t_pos, t_charperline);
                t_painter.drawText(QRectF(0, t_actualheight, width(), t_actualheight + t_fontheight), t_part);
                t_actualheight += t_fontheight;
                t_pos += t_charperline;
            }
        }
    }
    setMinimumHeight(t_actualheight);
}

void ConsoleWidget::scrollDown()
{
    if(m_scrollbar != 0)
        m_scrollbar->verticalScrollBar()->setSliderPosition(m_scrollbar->verticalScrollBar()->maximum());
}
