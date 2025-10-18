#pragma once

#include <QStandardItemModel>

class EntityListController
{
public:
	static void setupEntityList(QStandardItemModel* model);
	static void updateEntityList(QStandardItemModel* model);
};