#include "EntityListController.h"
#include "GravitiLib/PhysicalState.h"
#include "GravitiLib/Entity.h"
#include "GravitiLib/EntityManager.h"
#include <QStandardItem>
#include <QTreeView>
#include <QDirIterator>

static std::vector<std::string> textures;

void EntityListController::setupEntityList(QStandardItemModel* model)
{
    model->clear();
    model->setHorizontalHeaderLabels(QStringList() << "Property" << "Value");
}

std::vector<std::string> EntityListController::getTextureList()
{
    if (textures.size() == 0)
    {
        QDirIterator it1(":/Bodies/content/images", QDirIterator::Subdirectories);
        while (it1.hasNext()) {
            std::string s;
            s.append("qrc");
            s.append(it1.next().toStdString());
            textures.push_back(s);
        }
        QDirIterator it2(":/Objects/content/images", QDirIterator::Subdirectories);
        while (it2.hasNext()) {
            std::string s;
            s.append("qrc");
            s.append(it2.next().toStdString());
            textures.push_back(s);
        }
    }
    return textures;
}

void EntityListController::updateEntityList(QStandardItemModel* model)
{

    auto manager = EntityManager::getInstance();
    auto entities = manager->getAllEntities();

    model->clear();
    model->setHorizontalHeaderLabels(QStringList() << "Property" << "Value");

    if (!entities) {
        return;
    }


    // Add each entity to the tree view
    for (auto& entity : *entities) {
        QStandardItem* entityItem = new QStandardItem(QString::fromStdString(entity.getEntityName()));
        entityItem->setEditable(false);

        // Add entity properties as children
        QStandardItem* nameItem = new QStandardItem("Name");
        QStandardItem* nameValue = new QStandardItem(QString::fromStdString(entity.getEntityName()));
        nameValue->setEditable(false);

        QStandardItem* idItem = new QStandardItem("ID");
        QStandardItem* idValue = new QStandardItem(QString::number(entity.getEntityID()));
        idValue->setEditable(false);

        // Get physical state
        PhysicalState* state = entity.getPhysicalState();
        if (state) {
            QStandardItem* massItem = new QStandardItem("Mass");
            QStandardItem* massValue = new QStandardItem(QString::number(state->getMass(), 'f', 2));
            massValue->setEditable(false);

            QStandardItem* radiusItem = new QStandardItem("Radius");
            QStandardItem* radiusValue = new QStandardItem(QString::number(state->getRadius(), 'f', 2));
            radiusValue->setEditable(false);

            QStandardItem* posItem = new QStandardItem("Position");
            QStandardItem* posXItem = new QStandardItem("X");
            QStandardItem* posXValue = new QStandardItem(QString::number(state->getPosition(X), 'f', 2));
            posXValue->setEditable(false);
            QStandardItem* posYItem = new QStandardItem("Y");
            QStandardItem* posYValue = new QStandardItem(QString::number(state->getPosition(Y), 'f', 2));
            posYValue->setEditable(false);
            QStandardItem* posZItem = new QStandardItem("Z");
            QStandardItem* posZValue = new QStandardItem(QString::number(state->getPosition(Z), 'f', 2));
            posZValue->setEditable(false);

            QStandardItem* velItem = new QStandardItem("Velocity");
            QStandardItem* velXItem = new QStandardItem("X");
            QStandardItem* velXValue = new QStandardItem(QString::number(state->getVelocity(X), 'f', 2));
            velXValue->setEditable(false);
            QStandardItem* velYItem = new QStandardItem("Y");
            QStandardItem* velYValue = new QStandardItem(QString::number(state->getVelocity(Y), 'f', 2));
            velYValue->setEditable(false);
            QStandardItem* velZItem = new QStandardItem("Z");
            QStandardItem* velZValue = new QStandardItem(QString::number(state->getVelocity(Z), 'f', 2));
            velZValue->setEditable(false);

            // Add texture path if available
            QStandardItem* textureItem = new QStandardItem("Texture");
            QStandardItem* textureValue = new QStandardItem(QString::fromStdString(entity.getTexturePath()));
            textureValue->setEditable(false);

            // Build the tree structure
            entityItem->appendRow(QList<QStandardItem*>() << nameItem << nameValue);
            entityItem->appendRow(QList<QStandardItem*>() << idItem << idValue);
            entityItem->appendRow(QList<QStandardItem*>() << massItem << massValue);
            entityItem->appendRow(QList<QStandardItem*>() << radiusItem << radiusValue);

            posItem->appendRow(QList<QStandardItem*>() << posXItem << posXValue);
            posItem->appendRow(QList<QStandardItem*>() << posYItem << posYValue);
            posItem->appendRow(QList<QStandardItem*>() << posZItem << posZValue);
            entityItem->appendRow(QList<QStandardItem*>() << posItem << new QStandardItem(""));

            velItem->appendRow(QList<QStandardItem*>() << velXItem << velXValue);
            velItem->appendRow(QList<QStandardItem*>() << velYItem << velYValue);
            velItem->appendRow(QList<QStandardItem*>() << velZItem << velZValue);
            entityItem->appendRow(QList<QStandardItem*>() << velItem << new QStandardItem(""));

            entityItem->appendRow(QList<QStandardItem*>() << textureItem << textureValue);
        }

        model->appendRow(QList<QStandardItem*>() << entityItem << new QStandardItem(""));
    }
}