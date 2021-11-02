/*
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#include "./users_controller.h"

// oauth-service
#include "../serializers/user_serializers.h"


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
	auto data = request->json();
	if (data.is_null())
	{
		return std::make_unique<xw::http::JsonResponse>(
			nlohmann::json(nlohmann::json::value_t::object), 400
		);
	}

	CreateUserSerializer serializer(this->_user_service);
	auto created_user = serializer.save(data);
	return std::make_unique<xw::http::JsonResponse>(created_user.to_json(), 201);
}
