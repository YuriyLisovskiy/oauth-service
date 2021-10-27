/*
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#pragma once

// xalwart
#include <xalwart/conf/module.h>


class RootModuleConfig : public xw::conf::ModuleConfig
{
public:
	explicit inline RootModuleConfig(
		const std::string& registration_name, xw::conf::Settings* settings
	) : xw::conf::ModuleConfig(registration_name, settings)
	{
	}

	void urlpatterns() override;
};
