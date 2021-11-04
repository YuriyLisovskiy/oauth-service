/*
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#include "./token_controller.h"

// oauth-service
#include "../serializers/token_serializer.h"


std::unique_ptr<xw::http::IResponse> TokenController::post(xw::http::IRequest* request) const
{
	auto data = request->json();
	if (data.is_null())
	{
		throw xw::http::exc::BadRequest("request data is null", _ERROR_DETAILS_);
	}

	auto serializer = TokenSerializer(this->_client_service)
		.set_signature_algorithm(this->_signature_algorithm)
		.set_jwt_period(this->_jwt_period)
		.set_subject(this->_subject)
		.set_issuer(this->_issuer);
	auto created_token = serializer.save(data);
	return std::make_unique<xw::http::JsonResponse>(created_token.to_json(), 201);
}
