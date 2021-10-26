/*
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#pragma once

// xalwart
#include <xalwart/conf/settings.h>
#include <xalwart/conf/loaders/yaml_loader.h>


class Settings final : public xw::conf::Settings
{
public:
	inline Settings() : xw::conf::Settings()
	{
	}

	void register_modules() override;

	void register_migrations() override;

	std::unique_ptr<xw::server::IServer> build_server(const std::function<xw::net::StatusCode(
		xw::net::RequestContext*, const std::map<std::string, std::string>& /* environment */
	)>& handler, const xw::Options& options) override;

	static std::unique_ptr<Settings> load();
};
