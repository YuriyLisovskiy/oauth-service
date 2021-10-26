/*
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#include "./hello.h"


std::unique_ptr<xw::http::HttpResponse> HelloController::get(xw::http::Request* request) const
{
	auto table_names = this->settings->DB->get_table_names();
	auto tables = nlohmann::json::array();
	for (const auto& table_name : table_names)
	{
		tables.push_back(table_name);
	}

	return std::make_unique<xw::http::JsonResponse>(nlohmann::json{
		{"tables", tables}
	});
}
