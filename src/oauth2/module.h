/*
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#pragma once

// Xalwart framework.
#include <xalwart/conf/module.h>


namespace oauth2
{

class ModuleConfig : public xw::conf::ModuleConfig
{
public:
	inline explicit ModuleConfig(xw::conf::Settings* settings) : xw::conf::ModuleConfig(settings)
	{
	}

	void urlpatterns() override;
};

}
