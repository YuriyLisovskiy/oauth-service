/*
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#include "./user_controller.h"

// oauth-service
#include "../serializers/user_serializers.h"


std::unique_ptr<xw::http::IResponse> UserController::get(
	xw::http::IRequest* request, long long int id
) const
{
	auto user = this->_user_service->get_by_id(id);
	return std::make_unique<xw::http::JsonResponse>(user.to_json(), 200);
}

std::unique_ptr<xw::http::IResponse> UserController::put(
	xw::http::IRequest* request, long long int id
) const
{
	auto data = request->json();
	if (data.is_null())
	{
		throw xw::http::exc::BadRequest("request data is null", _ERROR_DETAILS_);
	}

	data["id"] = id;
	UpdateUserSerializer serializer(this->_user_service);
	auto updated_user = serializer.save(data);
	return std::make_unique<xw::http::JsonResponse>(updated_user.to_json(), 200);
}

std::unique_ptr<xw::http::IResponse> UserController::delete_(
	xw::http::IRequest* request, long long int id
) const
{
	auto user = this->_user_service->remove(id);
	return std::make_unique<xw::http::JsonResponse>(user.to_json(), 200);
}
