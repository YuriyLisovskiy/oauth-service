/*
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#pragma once

// Xalwart framework.
#include <xalwart/conf/module.h>


class ModuleConfig : public xw::conf::ModuleConfig
{
public:
	explicit inline ModuleConfig(xw::conf::Settings* settings) : xw::conf::ModuleConfig(settings)
	{
	}

	void urlpatterns() override;
};
