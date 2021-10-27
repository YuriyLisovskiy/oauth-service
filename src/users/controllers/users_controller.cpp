/*
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#include "./users_controller.h"


std::unique_ptr<xw::http::HttpResponse> UsersController::get(xw::http::Request* request) const
{
	auto user_models = this->_user_service->get_all();
	auto users = nlohmann::json::array();
	for (const auto& user : user_models)
	{
		users.push_back(user.to_json());
	}

	return std::make_unique<xw::http::JsonResponse>(nlohmann::json{{"users", users}});
}

std::unique_ptr<xw::http::HttpResponse> UsersController::post(xw::http::Request* request) const
{
	// TODO:
	return nullptr;
}
