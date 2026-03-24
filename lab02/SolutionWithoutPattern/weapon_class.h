#ifndef WEAPON_CLASSES_H
#define WEAPON_CLASSES_H

#include <QString>
#include <QPixmap>
#include <QPainter>
#include <QDebug>
#include <QMap>
#include <QStringList>

struct AttachmentModifiers {
    int damage = 0;
    int range = 0;
    int accuracy = 0;
    int stability = 0;
    int ammo = 0;
    QString name;
    QString overlayImagePath;
};

class Weapon
{
private:
    QString name;
    QString weaponType;
    int damage;
    int range;
    int accuracy;
    int stability;
    int ammo;
    QString baseImagePath;
    QStringList availableMagazines;
    QStringList availableScopes;
    QStringList availableStocks;

    QString currentMagazine;
    QString currentScope;
    QString currentStock;

    QMap<QString, AttachmentModifiers> magazineModifiers;
    QMap<QString, AttachmentModifiers> scopeModifiers;
    QMap<QString, AttachmentModifiers> stockModifiers;

    void initModifiers()
    {
        if (weaponType == "AK74") {
            AttachmentModifiers standardMag;
            standardMag.ammo = 30;
            standardMag.name = "Стандартный магазин";
            standardMag.overlayImagePath = QString("../../../Assets/%1/%1standardmagazine.png").arg(weaponType);
            magazineModifiers["standard"] = standardMag;

            AttachmentModifiers extendedMag;
            extendedMag.ammo = 50;
            extendedMag.stability = -10;
            extendedMag.accuracy = -10;
            extendedMag.name = "Расширенный магазин";
            extendedMag.overlayImagePath = QString("../../../Assets/%1/%1extendedmagazine.png").arg(weaponType);
            magazineModifiers["extended"] = extendedMag;
        }
        else if (weaponType == "pistol") {
            AttachmentModifiers standardMag;
            standardMag.ammo = 10;
            standardMag.name = "Стандартный магазин";
            standardMag.overlayImagePath = QString("../../../Assets/%1/%1standardmagazine.png").arg(weaponType);
            magazineModifiers["standard"] = standardMag;

            AttachmentModifiers extendedMag;
            extendedMag.ammo = 15;
            extendedMag.stability = -5;
            extendedMag.accuracy = -5;
            extendedMag.name = "Расширенный магазин";
            extendedMag.overlayImagePath = QString("../../../Assets/%1/%1extendedmagazine.png").arg(weaponType);
            magazineModifiers["extended"] = extendedMag;
        }
        else if (weaponType == "SVD") {
            AttachmentModifiers standardMag;
            standardMag.ammo = 5;
            standardMag.name = "Магазин";
            standardMag.overlayImagePath = QString("../../../Assets/%1/%1standardmagazine.png").arg(weaponType);
            magazineModifiers["standard"] = standardMag;

            AttachmentModifiers extendedMag;
            extendedMag.ammo = 10;
            extendedMag.stability = -10;
            extendedMag.accuracy = -10;
            extendedMag.name = "Магазин";
            extendedMag.overlayImagePath = QString("../../../Assets/%1/%1extendedmagazine.png").arg(weaponType);
            magazineModifiers["extended"] = extendedMag;
        }

        if (weaponType == "AK74") {
            AttachmentModifiers laser;
            laser.stability = -5;
            laser.accuracy = 5;
            laser.range = 30;
            laser.name = "Лазерный прицел";
            laser.overlayImagePath = QString("../../../Assets/%1/%1laserscope.png").arg(weaponType);
            scopeModifiers["laser"] = laser;

            AttachmentModifiers optical;
            optical.stability = -20;
            optical.accuracy = 20;
            optical.range = 300;
            optical.name = "Оптический прицел";
            optical.overlayImagePath = QString("../../../Assets/%1/%1opticalscope.png").arg(weaponType);
            scopeModifiers["optical"] = optical;
        }
        else if (weaponType == "pistol") {
            AttachmentModifiers laser;
            laser.accuracy = 10;
            laser.range = 20;
            laser.stability = -10;
            laser.name = "Лазерный прицел";
            laser.overlayImagePath = QString("../../../Assets/%1/%1laserscope.png").arg(weaponType);
            scopeModifiers["laser"] = laser;

            AttachmentModifiers flashlight;
            flashlight.accuracy = 5;
            flashlight.stability = -5;
            flashlight.name = "Фонарик";
            flashlight.overlayImagePath = QString("../../../Assets/%1/%1flashlightscope.png").arg(weaponType);
            scopeModifiers["flashlight"] = flashlight;
        }
        else if (weaponType == "SVD") {
            AttachmentModifiers optical;
            optical.accuracy = 20;
            optical.range = 500;
            optical.name = "Оптический прицел";
            optical.overlayImagePath = QString("../../../Assets/%1/%1opticalscope.png").arg(weaponType);
            scopeModifiers["optical"] = optical;
        }
        else if (weaponType == "portalGun") {
            AttachmentModifiers glados;
            glados.name = "Глэдос";
            glados.overlayImagePath = QString("../../../Assets/%1/%1gladosscope.png").arg(weaponType);
            scopeModifiers["glados"] = glados;
        }

        if (weaponType == "AK74") {
            AttachmentModifiers standardStock;
            standardStock.accuracy = 50;
            standardStock.name = "Стандартный приклад";
            standardStock.overlayImagePath = QString("../../../Assets/%1/%1standardstock.png").arg(weaponType.toUpper());
            stockModifiers["standard"] = standardStock;

            AttachmentModifiers tacticalStock;
            tacticalStock.accuracy = 60;
            tacticalStock.stability = -5;
            tacticalStock.name = "Тактический приклад";
            tacticalStock.overlayImagePath = QString("../../../Assets/%1/%1tacticalstock.png").arg(weaponType.toUpper());
            stockModifiers["tactical"] = tacticalStock;
        }
    }

    int getModifierValue(const QMap<QString, AttachmentModifiers>& modifiers,
                         const QString& attachment,
                         int modifierField) const
    {
        if (attachment.isEmpty()) return 0;
        if (modifiers.contains(attachment)) {
            const AttachmentModifiers& mod = modifiers[attachment];
            switch (modifierField) {
            case 0: return mod.damage;
            case 1: return mod.range;
            case 2: return mod.accuracy;
            case 3: return mod.stability;
            case 4: return mod.ammo;
            default: return 0;
            }
        }
        return 0;
    }

public:
    Weapon(const QString& name, const QString& weaponType, int damage, int range,
           int accuracy, int stability, int ammo, const QString& baseImagePath,
           const QStringList& magazines = {}, const QStringList& scopes = {},
           const QStringList& stocks = {})
        : name(name)
        , weaponType(weaponType)
        , damage(damage)
        , range(range)
        , accuracy(accuracy)
        , stability(stability)
        , ammo(ammo)
        , baseImagePath(baseImagePath)
        , availableMagazines(magazines)
        , availableScopes(scopes)
        , availableStocks(stocks)
    {
        initModifiers();
    }

    void setMagazine(const QString& magazineType) { currentMagazine = magazineType; }
    void setScope(const QString& scopeType) { currentScope = scopeType; }
    void setStock(const QString& stockType) { currentStock = stockType; }

    void clearAttachments()
    {
        currentMagazine.clear();
        currentScope.clear();
        currentStock.clear();
    }

    QString getCurrentMagazine() const { return currentMagazine; }
    QString getCurrentScope() const { return currentScope; }
    QString getCurrentStock() const { return currentStock; }

    int getDamage() const
    {
        int total = damage;
        total += getModifierValue(magazineModifiers, currentMagazine, 0);
        total += getModifierValue(scopeModifiers, currentScope, 0);
        total += getModifierValue(stockModifiers, currentStock, 0);
        return total;
    }

    int getRange() const
    {
        int total = range;
        total += getModifierValue(magazineModifiers, currentMagazine, 1);
        total += getModifierValue(scopeModifiers, currentScope, 1);
        total += getModifierValue(stockModifiers, currentStock, 1);
        return total;
    }

    int getAccuracy() const
    {
        int total = accuracy;
        total += getModifierValue(magazineModifiers, currentMagazine, 2);
        total += getModifierValue(scopeModifiers, currentScope, 2);
        total += getModifierValue(stockModifiers, currentStock, 2);
        return total;
    }

    int getStability() const
    {
        int total = stability;
        total += getModifierValue(magazineModifiers, currentMagazine, 3);
        total += getModifierValue(scopeModifiers, currentScope, 3);
        total += getModifierValue(stockModifiers, currentStock, 3);
        return total;
    }

    int getAmmo() const
    {
        int total = ammo;
        total += getModifierValue(magazineModifiers, currentMagazine, 4);
        total += getModifierValue(scopeModifiers, currentScope, 4);
        total += getModifierValue(stockModifiers, currentStock, 4);
        return total;
    }

    QPixmap render() const
    {
        QPixmap result(baseImagePath);
        QPainter painter(&result);

        if (!currentMagazine.isEmpty() && magazineModifiers.contains(currentMagazine)) {
            QPixmap overlay(magazineModifiers[currentMagazine].overlayImagePath);
            if (!overlay.isNull()) painter.drawPixmap(0, 0, overlay);
        }

        if (!currentScope.isEmpty() && scopeModifiers.contains(currentScope)) {
            QPixmap overlay(scopeModifiers[currentScope].overlayImagePath);
            if (!overlay.isNull()) painter.drawPixmap(0, 0, overlay);
        }

        if (!currentStock.isEmpty() && stockModifiers.contains(currentStock)) {
            QPixmap overlay(stockModifiers[currentStock].overlayImagePath);
            if (!overlay.isNull()) painter.drawPixmap(0, 0, overlay);
        }

        painter.end();
        return result;
    }

    QString getWeaponType() const { return weaponType; }

    QString getDescription() const
    {
        QString desc = name;

        if (!currentMagazine.isEmpty() && magazineModifiers.contains(currentMagazine)) {
            desc += " + " + magazineModifiers[currentMagazine].name;
        }
        if (!currentScope.isEmpty() && scopeModifiers.contains(currentScope)) {
            desc += " + " + scopeModifiers[currentScope].name;
        }
        if (!currentStock.isEmpty() && stockModifiers.contains(currentStock)) {
            desc += " + " + stockModifiers[currentStock].name;
        }

        return desc;
    }

    QString getImagePath() const { return baseImagePath; }

    QStringList getAvailableMagazines() const { return availableMagazines; }
    QStringList getAvailableScopes() const { return availableScopes; }
    QStringList getAvailableStocks() const { return availableStocks; }
};

#endif