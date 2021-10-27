/*
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#include "./settings.h"

// xalwart
#include <xalwart.base/workers/threaded_worker.h>
#include <xalwart.server/http_server.h>
#include <xalwart.orm/config/yaml.h>

// oauth-service
#include "../oauth/module.h"
#include "../users/module.h"
#include "./migrations/001_create_client.h"
#include "./migrations/002_create_user.h"


void Settings::register_modules()
{
	this->module<ModuleConfig>();
	this->module<UsersModuleConfig>();
}

void Settings::register_migrations()
{
	this->migration<Migration001_CreateClient>();
	this->migration<Migration002_CreateUser>();
}

std::unique_ptr<xw::server::IServer> Settings::build_server(const std::function<xw::net::StatusCode(
	xw::net::RequestContext*, const std::map<std::string, std::string>& /* environment */
)>& handler, const xw::Options& options)
{
	return std::make_unique<xw::server::DevelopmentHTTPServer>(xw::server::Context{
		.logger = this->LOGGER.get(),
		.timezone = this->TIMEZONE,
		.max_headers_count = this->LIMITS.MAX_HEADERS_COUNT,
		.max_header_length = this->LIMITS.MAX_HEADER_LENGTH,
		.timeout_seconds = options.get<time_t>("timeout_seconds"),
		.timeout_microseconds = options.get<time_t>("timeout_microseconds"),
		.socket_creation_retries_count = options.get<size_t>("retries"),
		.worker = std::make_unique<xw::ThreadedWorker>(options.get<size_t>("workers")),
		.handler = handler
	});
}

std::unique_ptr<Settings> Settings::load()
{
	return xw::conf::YAMLSettingsLoader<Settings>({"settings", R"(settings\.local)"})
		.with_components([](auto* loader, auto* settings)
		{
			// Registration of logger should be done first
			// to log errors during the next components' setup.
			loader->register_default_logger(settings);

			// Setup standard components which are present in
			// xw::conf::Settings excluding template engine
			// and databases.
			loader->register_standard_components(settings);

			// Other custom components' setup.
			loader->register_component("databases", std::make_unique<xw::orm::config::YAMLDatabasesComponent>(
				settings->BASE_DIR, settings->DATABASES
			));
		})
		.load();
}
