/*
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

// STL
#include <iostream>

// Xalwart framework
#include <xalwart/conf/application.h>

// oauth2.service
#include "./config/settings.h"


int main(int argc, char** argv)
{
	xw::conf::initialize_signal_handlers();
	auto settings = service::Settings::load();
	try
	{
		xw::conf::Application(settings.get())
			.configure()
			.execute(argc, argv);
	}
	catch (const xw::BaseException& exc)
	{
		if (settings->LOGGER)
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
