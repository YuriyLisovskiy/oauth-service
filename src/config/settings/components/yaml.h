/*
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#pragma once

// STL
#include <memory>

// xalwart
#include <xalwart.base/config/components/yaml/default.h>
#include <xalwart.crypto/interfaces.h>

// oauth-service
#include "../types.h"


class YAMLSignerComponent : public xw::config::YAMLMapComponent
{
public:
	explicit YAMLSignerComponent(std::shared_ptr<xw::crypto::ISignatureAlgorithm>& signer);

	void initialize(const YAML::Node& node) const override;

private:
	std::string _algorithm;
	std::string _secret_key;
	std::shared_ptr<xw::crypto::ISignatureAlgorithm>& _signer;
};

class YAMLTimedeltaComponent : public xw::config::YAMLMapComponent
{
public:
	explicit YAMLTimedeltaComponent(xw::dt::Timedelta& timedelta);

	inline void initialize(const YAML::Node& node) const override
	{
		xw::config::YAMLMapComponent::initialize(node);
		this->_timedelta = this->_config.to_timedelta();
	}

private:
	struct Timedelta
	{
		long days = 0;
		long seconds = 0;
		long microseconds = 0;
		long milliseconds = 0;
		long minutes = 0;
		long hours = 0;
		long weeks = 0;

		[[nodiscard]]
		inline xw::dt::Timedelta to_timedelta() const
		{
			return xw::dt::Timedelta(
				this->days, this->seconds, this->microseconds,
				this->milliseconds, this->minutes, this->hours, this->weeks
			);
		}
	};

	Timedelta _config;
	xw::dt::Timedelta& _timedelta;
};

class YAMLOAuthComponent : public xw::config::YAMLMapComponent
{
public:
	explicit YAMLOAuthComponent(OAuthConfig& config);

private:
	OAuthConfig& _config;
};
