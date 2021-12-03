/*
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#include "./yaml.h"

// xalwart
#include <xalwart.crypto/hmac.h>


YAMLSignerComponent::YAMLSignerComponent(std::shared_ptr<xw::crypto::ISignatureAlgorithm>& signer) :
	_signer(signer)
{
	this->register_component(
		"algorithm",
		std::make_unique<xw::config::YAMLScalarComponent>(this->_algorithm)
	);
	this->register_component(
		"secret_key",
		std::make_unique<xw::config::YAMLScalarComponent>(this->_secret_key)
	);
}

void YAMLSignerComponent::initialize(const YAML::Node& node) const
{
	xw::config::YAMLMapComponent::initialize(node);
	this->_signer = xw::crypto::get_hs_signer(this->_algorithm, this->_secret_key);
}

YAMLTimedeltaComponent::YAMLTimedeltaComponent(xw::dt::Timedelta& timedelta) :
	xw::config::YAMLMapComponent(true), _timedelta(timedelta)
{
	this->register_component(
		"days", std::make_unique<xw::config::YAMLScalarComponent>(this->_config.days)
	);
	this->register_component(
		"seconds", std::make_unique<xw::config::YAMLScalarComponent>(this->_config.seconds)
	);
	this->register_component(
		"microseconds", std::make_unique<xw::config::YAMLScalarComponent>(this->_config.microseconds)
	);
	this->register_component(
		"milliseconds", std::make_unique<xw::config::YAMLScalarComponent>(this->_config.milliseconds)
	);
	this->register_component(
		"minutes", std::make_unique<xw::config::YAMLScalarComponent>(this->_config.minutes)
	);
	this->register_component(
		"hours", std::make_unique<xw::config::YAMLScalarComponent>(this->_config.hours)
	);
	this->register_component(
		"weeks", std::make_unique<xw::config::YAMLScalarComponent>(this->_config.weeks)
	);
}

YAMLOAuthComponent::YAMLOAuthComponent(OAuthConfig& config) : _config(config)
{
	auto jwt_component = std::make_unique<xw::config::YAMLMapComponent>();
	jwt_component->register_component(
		"subject",
		std::make_unique<xw::config::YAMLScalarComponent>(this->_config.JWT.SUBJECT)
	);
	jwt_component->register_component(
		"issuer",
		std::make_unique<xw::config::YAMLScalarComponent>(this->_config.JWT.ISSUER)
	);
	jwt_component->register_component(
		"period", std::make_unique<YAMLTimedeltaComponent>(this->_config.JWT.PERIOD)
	);
	jwt_component->register_component(
		"token_type",
		std::make_unique<xw::config::YAMLScalarComponent>(this->_config.JWT.TOKEN_TYPE)
	);
	this->register_component("jwt", std::move(jwt_component));

	this->register_component(
		"signer", std::make_unique<YAMLSignerComponent>(this->_config.SIGNER)
	);
}
