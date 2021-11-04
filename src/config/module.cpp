/*
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#include "./module.h"


void RootModuleConfig::urlpatterns()
{
	this->include("OAuthModuleConfig", R"(oauth/)", "oauth");
	this->include("UsersModuleConfig", R"(users/)", "users");
}
