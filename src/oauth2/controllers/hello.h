/*
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#pragma once

// Xalwart framework
#include <xalwart/controllers/controller.h>


class HelloController : public xw::ctrl::Controller<>
{
public:
	explicit inline HelloController(const xw::conf::Settings* settings) :
		xw::ctrl::Controller<>({"get"}, settings)
	{
	}

	std::unique_ptr<xw::http::abc::HttpResponse> get(xw::http::Request* request) const override;
};
