/*
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#pragma once

// xalwart
#include <xalwart/conf/module.h>


class ModuleConfig : public xw::conf::ModuleConfig
{
public:
	explicit inline ModuleConfig(
		const std::string& registration_name, xw::conf::Settings* settings
	) : xw::conf::ModuleConfig(registration_name, settings)
	{
	}

	void commands() override;
};
