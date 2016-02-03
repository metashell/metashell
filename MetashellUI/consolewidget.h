#ifndef CONSOLEWIDGET_H
#define CONSOLEWIDGET_H

#include <QWidget>
#include <QScrollArea>
#include <memory>

class ConsoleWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ConsoleWidget(QWidget *parent = 0);
    void                            setScrollBar(QScrollArea* p_area);
    void                            addText(QString p_text);
    void                            addComment(QString p_text);
    void                            addError(QString p_text);
    void                            clear();
protected:
    void                            paintEvent(QPaintEvent *p_event);
    void                            scrollDown();
private:
    enum LineType
    {
        Normal, Comment, Error
    };

    struct Line
    {
        QString content;
        LineType type;
    };

    QVector<Line>                   m_content;
    QString                         m_text;
    QScrollArea*                    m_scrollbar;
signals:

public slots:

};

#endif // CONSOLEWIDGET_H
