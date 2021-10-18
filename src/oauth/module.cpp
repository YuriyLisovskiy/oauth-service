/*
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#include "./module.h"

// oauth2.service
#include "./controllers/hello.h"


void ModuleConfig::urlpatterns()
{
	this->url<HelloController>(R"(oauth2/hello/?)", "hello");
}
