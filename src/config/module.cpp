/*
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#include "./module.h"


void RootModuleConfig::urlpatterns()
{
	this->include("UsersModuleConfig", R"(users/)", "users");
}
