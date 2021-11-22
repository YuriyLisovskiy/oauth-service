/*
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#include "./token_controller.h"

// xalwart
#include <xalwart/http/headers.h>

// oauth-service
#include "../serializers/token_serializers.h"


std::unique_ptr<xw::http::IResponse> TokenController::post(xw::http::IRequest* request) const
{
	auto data = request->json();
	if (data.is_null())
	{
		throw xw::http::exc::BadRequest("request data is null", _ERROR_DETAILS_);
	}

	auto serializer = CreateTokenSerializer(this->_client_service)
		.set_signature_algorithm(this->_signature_algorithm)
		.set_jwt_period(this->_jwt_period)
		.set_subject(this->_subject)
		.set_issuer(this->_issuer);
	auto created_token = serializer.save(data);
	auto response = std::make_unique<xw::http::JsonResponse>(created_token.to_json(), 201);
	this->_set_required_headers(response.get());
	return response;
}

std::unique_ptr<xw::http::IResponse> TokenController::dispatch(xw::http::IRequest* request) const
{
	try
	{
		return xw::ctrl::Controller<>::dispatch(request);
	}
	catch (const OAuthError& error)
	{
		return std::make_unique<xw::http::JsonResponse>(
			nlohmann::json{
				{"error", error.get_error_value().to_string()},
				{"error_description", error.what()},
				{
					"error_uri",
					"See the full API docs at https://github.com/YuriyLisovskiy/oauth-service/blob/master/README.md"
				}
			},
			error.status_code()
		);
	}
}

void TokenController::_set_required_headers(xw::http::IResponse* response) const
{
	response->set_header(xw::http::CACHE_CONTROL, "no-store");
	response->set_header("Pragma", "no-cache");
}
