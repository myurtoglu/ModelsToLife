#ifndef SIMOPTIONSDIALOG_HPP
#define SIMOPTIONSDIALOG_HPP

#include <QDialog>

namespace Ui {
class SimOptionsDialog;
}

class SimOptionsDialog: public QDialog {
    Q_OBJECT

public:
    explicit SimOptionsDialog(QWidget *parent = 0);
    ~SimOptionsDialog();

    float *mpDeltaT;
    float *mpGravity;
    float *mpRestitution;
    float *mpStiffness;
    float *mpDamping;
    float *mpMass;

private:
    void setUpSimOptionsDialog();
    void createConnections();

    Ui::SimOptionsDialog *ui;

private slots:
    void changeDeltaT(int state);
    void changeGravity(int state);
    void changeRestitution(int state);
    void changeStiffness(int state);
    void changeDamping(int state);
    void changeMass(int state);

signals:
    void propertiesChanged();
};

#endif // SIMOPTIONSDIALOG_HPP
