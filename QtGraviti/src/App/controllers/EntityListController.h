#pragma once

#include <QStandardItemModel>
#include <vector>
#include <string>

class EntityListController
{
public:
	static void setupEntityList(QStandardItemModel* model);
	static void updateEntityList(QStandardItemModel* model);
	static std::vector<std::string> getTextureList();
};