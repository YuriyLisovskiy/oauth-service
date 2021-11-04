/*
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#pragma once

// xalwart
#include <xalwart/conf/module.h>


class ModuleConfig : public xw::conf::ModuleConfig
{
public:
	explicit inline ModuleConfig(const std::string& name, xw::conf::Settings* settings) :
		xw::conf::ModuleConfig(name, settings)
	{
	}

	inline void urlpatterns() override
	{
	}

	void commands() override;
};
