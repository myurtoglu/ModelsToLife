#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "addtetdialog.hpp"
#include "editobjdialog.hpp"
#include "editskeletondialog.hpp"
#include "simoptionsdialog.hpp"
#include "editsystemdialog.hpp"

#include <QMainWindow>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void openAddTetDialog();
    void saveToFile();
    void openFile();
    void openObjFile();
    void saveAllToFile();
    void openAllFromFile();
    void openEditObjectDialog();
    void openEditSkeletonDialog();
    void openSimOptionsDialog();
    void propertiesChanged();
    void openEditSystemDialog();
    void createTetFromPly();
    void editAccepted();

private:
    void keyPressEvent(QKeyEvent * event);
    void setUpMainWindow();
    void createDialogs();
    void connectVariables();
    void createConnections();
    void createShortcutKeys();

    Ui::MainWindow     *ui;
    AddTetDialog       *pAddTetDialog;
    EditObjDialog      *pEditObjDialog;
    EditSkeletonDialog *pEditSkeletonDialog;
    SimOptionsDialog   *pSimOptionsDialog;
    EditSystemDialog   *pEditSystemDialog;
};

#endif // MAINWINDOW_HPP
