/*
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#include "./users_controller.h"

// oauth-service
#include "../serializers/user_serializers.h"


std::unique_ptr<xw::http::IResponse> UsersController::get(xw::http::IRequest* request) const
{
	auto user_models = this->user_service->get_all();
	auto users = nlohmann::json::array();
	for (const auto& user : user_models)
	{
		users.push_back(user.to_json());
	}

	return std::make_unique<xw::http::JsonResponse>(nlohmann::json{{"users", users}});
}

std::unique_ptr<xw::http::IResponse> UsersController::post(xw::http::IRequest* request) const
{
	auto data = request->json();
	if (data.is_null())
	{
		throw xw::http::exc::BadRequest("request data is null", _ERROR_DETAILS_);
	}

	CreateUserSerializer serializer(this->user_service);
	auto created_user = serializer.save(data);
	return std::make_unique<xw::http::JsonResponse>(created_user.to_json(), 201);
}
