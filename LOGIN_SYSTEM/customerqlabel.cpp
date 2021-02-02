#include<customerqlabel.h>

customerqlabel::customerqlabel(QWidget *parent, Qt::WindowFlags f):
    QLabel(parent,f)
{
}
customerqlabel::customerqlabel(const QString &text, QWidget *parent, Qt::WindowFlags f):
    QLabel(text,parent,f)
{
}
void customerqlabel::mouseReleaseEvent(QMouseEvent * ev)
{
    Q_UNUSED(ev)
    emit clicked();
}
