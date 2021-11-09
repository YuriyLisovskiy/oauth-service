/*
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#pragma once

// STL
#include <string>
#include <memory>

// xalwart
#include <xalwart.base/datetime.h>
#include <xalwart.crypto/interfaces.h>


struct OAuthConfig final
{
	struct JWTConfig final
	{
		std::string SUBJECT;
		std::string ISSUER;
		xw::dt::Timedelta PERIOD = xw::dt::Timedelta();
	};

	JWTConfig JWT;
	std::shared_ptr<xw::crypto::ISignatureAlgorithm> SIGNER = nullptr;
};
