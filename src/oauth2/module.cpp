/*
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#include "./module.h"

// oauth2.service
#include "./controllers/hello.h"


namespace oauth2
{

void ModuleConfig::urlpatterns()
{
	this->url<HelloController>(R"(oauth2/hello/?)", "oauth2::hello");
}

}
