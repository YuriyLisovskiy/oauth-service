/*
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

// STL
#include <iostream>

// xalwart
#include <xalwart/conf/application.h>

// oauth-service
#include "./config/settings.h"


int main(int argc, char** argv)
{
	xw::conf::initialize_signal_handlers();
	std::unique_ptr<Settings> settings;
	try
	{
		settings = Settings::load();
		xw::conf::Application(settings.get())
			.configure()
			.execute(argc, argv);
	}
	catch (const xw::ImproperlyConfigured& exc)
	{
		if (settings && settings->LOGGER)
		{
			settings->LOGGER->error(exc);
		}
		else
		{
			std::cerr << exc.get_message() << std::endl;
		}
	}

	return 0;
}
