#ifndef HELPFORM_H
#define HELPFORM_H

#include <QDialog>

namespace Ui {
    class HelpForm;
}

class HelpForm : public QDialog {
    Q_OBJECT
public:
    HelpForm(const QString &text, QWidget *parent = 0);
    ~HelpForm();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::HelpForm *ui;
};

#endif // HELPFORM_H
