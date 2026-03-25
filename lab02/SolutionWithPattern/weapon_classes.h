#ifndef WEAPON_CLASSES_H
#define WEAPON_CLASSES_H

#include <QString>
#include <QPixmap>
#include <QPainter>
#include <memory>
#include <QDebug>
#include <QMap>
#include <QStringList>

class Weapon
{
public:
    virtual ~Weapon() = default;
    virtual int getDamage() = 0;
    virtual int getRange() = 0;
    virtual int getAccuracy() = 0;
    virtual int getStability() = 0;
    virtual int getAmmo() = 0;
    virtual QString getDescription() = 0;
    virtual QPixmap render() = 0;
    virtual QString getImagePath() = 0;
    virtual QString getWeaponType() = 0;
    virtual QStringList getAvailableMagazines() = 0;
    virtual QStringList getAvailableScopes() = 0;
    virtual QStringList getAvailableStocks() = 0;
    virtual std::shared_ptr<Weapon> getWrappedWeapon() { return nullptr; }
};

class ConcreteWeapon : public Weapon
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

public:
    ConcreteWeapon(const QString& name, const QString& weaponType, int damage, int range,
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
    }

    int getDamage() override { return damage; }
    int getRange() override { return range; }
    int getAccuracy() override { return accuracy; }
    int getStability() override { return stability; }
    int getAmmo() override { return ammo; }

    QPixmap render() override { return QPixmap(baseImagePath); }

    QString getWeaponType() override { return weaponType; }
    QString getDescription() override { return name; }
    QString getImagePath() override { return baseImagePath; }

    QStringList getAvailableMagazines() override { return availableMagazines; }
    QStringList getAvailableScopes() override { return availableScopes; }
    QStringList getAvailableStocks() override { return availableStocks; }

    std::shared_ptr<Weapon> getWrappedWeapon() override { return nullptr; }
};

class WeaponDecorator : public Weapon
{
protected:
    std::shared_ptr<Weapon> wrappedWeapon;
    QString overlayImagePath;

    std::shared_ptr<ConcreteWeapon> getBaseWeapon(std::shared_ptr<Weapon> weapon)
    {
        if (!weapon) return nullptr;
        if (auto concrete = std::dynamic_pointer_cast<ConcreteWeapon>(weapon)) {
            return concrete;
        }
        if (auto decorator = std::dynamic_pointer_cast<WeaponDecorator>(weapon)) {
            return getBaseWeapon(decorator->getWrappedWeapon());
        }
        return nullptr;
    }

public:
    WeaponDecorator(std::shared_ptr<Weapon> weapon) : wrappedWeapon(weapon) {}

    virtual ~WeaponDecorator() = default;

    int getDamage() override { return wrappedWeapon->getDamage(); }
    int getRange() override { return wrappedWeapon->getRange(); }
    int getAccuracy() override { return wrappedWeapon->getAccuracy(); }
    int getStability() override { return wrappedWeapon->getStability(); }
    int getAmmo() override { return wrappedWeapon->getAmmo(); }

    QPixmap render() override
    {
        QPixmap result = wrappedWeapon->render();
        QPainter painter(&result);
        QPixmap overlay(overlayImagePath);

        if (!overlay.isNull()) {
            painter.drawPixmap(0, 0, overlay);
        }
        painter.end();

        return result;
    }

    QString getDescription() override { return wrappedWeapon->getDescription(); }
    QString getImagePath() override { return overlayImagePath; }
    QString getWeaponType() override { return wrappedWeapon->getWeaponType(); }

    QStringList getAvailableMagazines() override { return wrappedWeapon->getAvailableMagazines(); }
    QStringList getAvailableScopes() override { return wrappedWeapon->getAvailableScopes(); }
    QStringList getAvailableStocks() override { return wrappedWeapon->getAvailableStocks(); }

    std::shared_ptr<Weapon> getWrappedWeapon() override { return wrappedWeapon; }
};

class Magazine : public WeaponDecorator
{
private:
    QString attachmentName;
    int accuracyDifference;
    int stabilityDifference;
    int ammoDifference;

    void initModifiers(const QString& weaponType, const QString& magazineType)
    {
        accuracyDifference = 0;
        ammoDifference = 0;
        stabilityDifference = 0;
        attachmentName = "Магазин";

        if (weaponType == "AK74") {
            if (magazineType == "standard") {
                ammoDifference = 30;
                attachmentName = "Стандартный магазин";
            }
            else if (magazineType == "extended") {
                ammoDifference = 50;
                stabilityDifference = -10;
                accuracyDifference = -10;
                attachmentName = "Расширенный магазин";
            }
        }
        else if (weaponType == "pistol") {
            if (magazineType == "standard") {
                ammoDifference = 10;
                attachmentName = "Стандартный магазин";
            }
            else if (magazineType == "extended") {
                ammoDifference = 15;
                stabilityDifference = -5;
                accuracyDifference = -5;
                attachmentName = "Расширенный магазин";
            }
        }
        else if (weaponType == "SVD") {
            if (magazineType == "standard") {
                ammoDifference = 5;
                attachmentName = "Магазин";
            }
            else if (magazineType == "extended") {
                ammoDifference = 10;
                stabilityDifference = -10;
                accuracyDifference = -10;
                attachmentName = "Магазин";
            }
        }

        overlayImagePath = QString("../../../Assets/%1/%1%2magazine.png").arg(weaponType).arg(magazineType);
    }

public:
    Magazine(std::shared_ptr<Weapon> weapon, const QString& magazineType) : WeaponDecorator(weapon)
    {
        auto baseWeapon = getBaseWeapon(wrappedWeapon);
        QString weaponType = baseWeapon->getWeaponType();
        initModifiers(weaponType, magazineType);
    }

    int getAmmo() override { return wrappedWeapon->getAmmo() + ammoDifference; }
    int getStability() override { return wrappedWeapon->getStability() + stabilityDifference; }
    int getAccuracy() override { return wrappedWeapon->getAccuracy() + accuracyDifference; }

    QString getDescription() override { return wrappedWeapon->getDescription() + " + " + attachmentName; }
};

class Scope : public WeaponDecorator
{
private:
    QString attachmentName;
    int accuracyDifference;
    int rangeDifference;
    int stabilityDifference;

    void initModifiers(const QString& weaponType, const QString& scopeType)
    {
        accuracyDifference = 0;
        rangeDifference = 0;
        stabilityDifference = 0;
        attachmentName = "Прицел";

        if (weaponType == "AK74") {
            if (scopeType == "laser") {
                stabilityDifference = -5;
                accuracyDifference = 5;
                rangeDifference = 30;
                attachmentName = "Лазерный прицел";
            }
            else if (scopeType == "optical") {
                stabilityDifference = -20;
                accuracyDifference = 20;
                rangeDifference = 300;
                attachmentName = "Оптический прицел";
            }
        }
        else if (weaponType == "pistol") {
            if (scopeType == "laser") {
                accuracyDifference = 10;
                rangeDifference = 20;
                stabilityDifference = -10;
                attachmentName = "Лазерный прицел";
            }
            if (scopeType == "flashlight") {
                accuracyDifference = 5;
                stabilityDifference = -5;
                attachmentName = "Фонарик";
            }
        }
        else if (weaponType == "SVD") {
            if (scopeType == "optical") {
                accuracyDifference = 20;
                rangeDifference = 500;
                attachmentName = "Оптический прицел";
            }
        }
        else if (weaponType == "portalGun"){
            if (scopeType == "glados") {
                attachmentName = "Глэдос";
            }
        }
        overlayImagePath = QString("../../../Assets/%1/%1%2scope.png").arg(weaponType).arg(scopeType);
    }

public:
    Scope(std::shared_ptr<Weapon> weapon, const QString& scopeType) : WeaponDecorator(weapon)
    {
        auto baseWeapon = getBaseWeapon(wrappedWeapon);
        QString weaponType = baseWeapon->getWeaponType();
        initModifiers(weaponType, scopeType);
    }

    int getAccuracy() override { return wrappedWeapon->getAccuracy() + accuracyDifference; }
    int getRange() override { return wrappedWeapon->getRange() + rangeDifference; }
    int getStability() override { return wrappedWeapon->getStability() + stabilityDifference; }

    QString getDescription() override { return wrappedWeapon->getDescription() + " + " + attachmentName; }
};

class Stock : public WeaponDecorator
{
private:
    QString attachmentName;
    int accuracyDifference;
    int stabilityDifference;

    void initModifiers(const QString& weaponType, const QString& stockType)
    {
        accuracyDifference = 0;
        stabilityDifference = 0;
        attachmentName = "Приклад";

        if (weaponType == "AK74") {
            if (stockType == "standard") {
                accuracyDifference = 50;
                attachmentName = "Стандартный приклад";
            }
            else if (stockType == "tactical") {
                accuracyDifference = 60;
                stabilityDifference = -5;
                attachmentName = "Тактический приклад";
            }
        }

        overlayImagePath = QString("../../../Assets/%1/%1%2stock.png").arg(weaponType.toUpper()).arg(stockType);
    }

public:
    Stock(std::shared_ptr<Weapon> weapon, const QString& stockType) : WeaponDecorator(weapon)
    {
        auto baseWeapon = getBaseWeapon(wrappedWeapon);
        QString weaponType = baseWeapon->getWeaponType();
        initModifiers(weaponType, stockType);
    }

    int getAccuracy() override { return wrappedWeapon->getAccuracy() + accuracyDifference; }
    int getStability() override { return wrappedWeapon->getStability() + stabilityDifference; }

    QString getDescription() override { return wrappedWeapon->getDescription() + " + " + attachmentName; }
};

#endif