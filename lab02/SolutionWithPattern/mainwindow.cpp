#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFont>
#include <climits>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initWeapons();

    for (const QString& key : weapons.keys()) {
        ui->weaponCombo->addItem(weapons[key]->getDescription(), key);
    }

    connect(ui->weaponCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onWeaponChanged);
    connect(ui->magazineCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onMagazineChanged);
    connect(ui->scopeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onScopeChanged);
    connect(ui->stockCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onStockChanged);

    onWeaponChanged(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initWeapons()
{
    weapons["AK74"] = std::make_shared<ConcreteWeapon>(
        "AK74", "AK74", 45, 200, 20, 70, 0,
        "../../../Assets/AK74/AK74.png",
        QStringList{"standard", "extended"},
        QStringList{"laser", "optical"},
        QStringList{"standard", "tactical"}
        );

    weapons["pistol"] = std::make_shared<ConcreteWeapon>(
        "Пистолет", "pistol", 20, 50, 40, 50, 0,
        "../../../Assets/Pistol/Pistol.png",
        QStringList{"standard", "extended"},
        QStringList{"laser", "flashlight"},
        QStringList{""}
        );

    weapons["SVD"] = std::make_shared<ConcreteWeapon>(
        "СВД", "SVD", 90, 500, 80, 70, 0,
        "../../../Assets/SVD/SVD.png",
        QStringList{"standard", "extended"},
        QStringList{"optical"},
        QStringList{""}
        );

    weapons["portalGun"] = std::make_shared<ConcreteWeapon>(
        "Портальная пушка", "portalGun", 0, INT_MAX, 100, 100, INT_MAX,
        "../../../Assets/PortalGun/PortalGun.png",
        QStringList{""},
        QStringList{"glados"},
        QStringList{""}
        );
}

void MainWindow::updateAttachmentOptions()
{
    QString currentWeaponKey = ui->weaponCombo->currentData().toString();
    auto weapon = weapons[currentWeaponKey];

    if (!weapon) return;

    ui->magazineCombo->clear();
    ui->magazineCombo->addItem("Нет");
    for (const QString& magazine : weapon->getAvailableMagazines()) {
        QString displayName;
        if (magazine == "standard") displayName = "Стандартный магазин";
        else if (magazine == "extended") displayName = "Расширенный магазин";
        if (!magazine.isEmpty()) {
            ui->magazineCombo->addItem(displayName, magazine);
        }
    }

    ui->scopeCombo->clear();
    ui->scopeCombo->addItem("Нет");
    for (const QString& scope : weapon->getAvailableScopes()) {
        QString displayName;
        if (scope == "optical") displayName = "Оптический прицел";
        else if (scope == "laser") displayName = "Лазерный прицел";
        else if (scope == "flashlight") displayName = "Фонарик";
        else if (scope == "glados") displayName = "Глэдос";
        if (!scope.isEmpty()) {
            ui->scopeCombo->addItem(displayName, scope);
        }
    }

    ui->stockCombo->clear();
    ui->stockCombo->addItem("Нет");
    for (const QString& stock : weapon->getAvailableStocks()) {
        QString displayName;
        if (stock == "tactical") displayName = "Тактический приклад";
        else if (stock == "standard") displayName = "Стандартный приклад";
        if (!stock.isEmpty()) {
            ui->stockCombo->addItem(displayName, stock);
        }
    }
}

void MainWindow::onWeaponChanged(int index)
{
    Q_UNUSED(index);

    ui->magazineCombo->setCurrentIndex(0);
    ui->scopeCombo->setCurrentIndex(0);
    ui->stockCombo->setCurrentIndex(0);

    updateAttachmentOptions();

    rebuildWeapon();

    updateDisplay();
}

void MainWindow::onMagazineChanged(int index)
{
    Q_UNUSED(index);
    rebuildWeapon();
    updateDisplay();
}

void MainWindow::onScopeChanged(int index)
{
    Q_UNUSED(index);
    rebuildWeapon();
    updateDisplay();
}

void MainWindow::onStockChanged(int index)
{
    Q_UNUSED(index);
    rebuildWeapon();
    updateDisplay();
}

void MainWindow::rebuildWeapon()
{
    QString currentWeaponKey = ui->weaponCombo->currentData().toString();
    auto baseWeapon = weapons[currentWeaponKey];

    if (!baseWeapon) return;

    currentWeapon = baseWeapon;

    QString magazineType = ui->magazineCombo->currentData().toString();
    if (!magazineType.isEmpty() && magazineType != "") {
        currentWeapon = std::make_shared<Magazine>(currentWeapon, magazineType);
    }

    QString scopeType = ui->scopeCombo->currentData().toString();
    if (!scopeType.isEmpty() && scopeType != "") {
        currentWeapon = std::make_shared<Scope>(currentWeapon, scopeType);
    }

    QString stockType = ui->stockCombo->currentData().toString();
    if (!stockType.isEmpty() && stockType != "") {
        currentWeapon = std::make_shared<Stock>(currentWeapon, stockType);
    }
}

void MainWindow::updateDisplay()
{
    updateStats();
    updateImage();
}

void MainWindow::updateStats()
{
    if (!currentWeapon) return;

    QString stats = QString(
                        "═══════════════════════════════════\n"
                        "  %1\n"
                        "═══════════════════════════════════\n"
                        "  Урон:          %2\n"
                        "  Дальность:     %3\n"
                        "  Точность:      %4\n"
                        "  Стабильность:  %5\n"
                        "  Боезапас:      %6\n"
                        "═══════════════════════════════════"
                        ).arg(currentWeapon->getDescription())
                        .arg(currentWeapon->getDamage())
                        .arg(currentWeapon->getRange())
                        .arg(currentWeapon->getAccuracy())
                        .arg(currentWeapon->getStability())
                        .arg(currentWeapon->getAmmo());

    ui->statsLabel->setText(stats);
    ui->statsLabel->setFont(QFont("Courier New", 10));
}

void MainWindow::updateImage()
{
    if (!currentWeapon) return;

    QPixmap weaponImage = currentWeapon->render();

    if (!weaponImage.isNull()) {
        QPixmap scaled = weaponImage.scaled(ui->imageLabel->size(),
                                            Qt::KeepAspectRatio,
                                            Qt::SmoothTransformation);
        ui->imageLabel->setPixmap(scaled);
    }
}