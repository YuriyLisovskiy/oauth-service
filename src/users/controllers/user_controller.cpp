/*
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#include "./user_controller.h"

// oauth-service
#include "../serializers/user_serializers.h"


std::unique_ptr<xw::http::HttpResponse> UserController::get(
	xw::http::Request* request, long long int id
) const
{
	// TODO:
	return nullptr;
}

std::unique_ptr<xw::http::HttpResponse> UserController::put(
	xw::http::Request* request, long long int id
) const
{
	auto data = request->json();
	if (!data.is_null())
	{
		data["id"] = id;
		UpdateUserSerializer serializer(this->_user_service);
		auto updated_user = serializer.save(data);
		if (updated_user.is_null())
		{
			throw xw::http::exc::NotFound(
				"user with id '" + std::to_string(id) + "' is not found", _ERROR_DETAILS_
			);
		}

		return std::make_unique<xw::http::JsonResponse>(updated_user.to_json(), 200);
	}

	return std::make_unique<xw::http::JsonResponse>(
		nlohmann::json(nlohmann::json::value_t::object), 400
	);
}

std::unique_ptr<xw::http::HttpResponse> UserController::delete_(
	xw::http::Request* request, long long int id
) const
{
	// TODO:
	return nullptr;
}
