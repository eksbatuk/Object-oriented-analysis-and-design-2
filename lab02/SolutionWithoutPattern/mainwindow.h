#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>
#include <QMap>
#include "weapon_class.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onWeaponChanged(int index);
    void onMagazineChanged(int index);
    void onScopeChanged(int index);
    void onStockChanged(int index);
    void updateDisplay();

private:
    Ui::MainWindow *ui;
    std::shared_ptr<Weapon> currentWeapon;
    QMap<QString, std::shared_ptr<Weapon>> weapons;

    void initWeapons();
    void updateWeaponAttachments();
    void updateStats();
    void updateImage();
    void updateAttachmentOptions();
};

#endif // MAINWINDOW_H