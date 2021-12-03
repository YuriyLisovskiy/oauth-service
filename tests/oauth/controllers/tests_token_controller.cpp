/*
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#include <gtest/gtest.h>

#include "../../request_mock.h"
#include "../client_service_mock.h"
#include "../../../src/oauth/controllers/token_controller.h"
#include "../../../src/oauth/exceptions.h"


class TestsOAuthControllers_TokenController : public ::testing::Test
{
protected:
	std::shared_ptr<ClientServiceMock> client_service_mock = nullptr;
	HttpRequestMock request_mock;
	std::shared_ptr<TokenController> controller;
	ClientModel client;
	xw::dt::Timedelta exp_period = xw::dt::Timedelta(1);

	void SetUp() override
	{
		this->client_service_mock = std::make_shared<ClientServiceMock>();
		auto now = xw::dt::Datetime::now(std::make_shared<xw::dt::Timezone>(xw::dt::Timezone::UTC));
		this->client = make_client(now);
		this->client_service_mock->set_data(std::list{this->client});
		this->controller = std::make_shared<TokenController>(nullptr);
		controller->set_issuer("test-issuer")
			.set_subject("test-subject")
			.set_token_type("Bearer")
			.set_jwt_period(this->exp_period)
			.set_client_service(this->client_service_mock)
			.set_signature_algorithm(std::make_shared<xw::crypto::HS256>("secret-key"));
	}

	static inline ClientModel make_client(const xw::dt::Datetime& dt)
	{
		return ClientModel(_generate_uuid4(), _generate_random_alphanum_string(64), dt, dt);
	}
};

TEST_F(TestsOAuthControllers_TokenController, post_RequestJsonIsNullptr)
{
	TokenController controller(nullptr);
	this->request_mock.set_json(nullptr);

	ASSERT_THROW(auto _ = controller.post(&this->request_mock), xw::http::exc::BadRequest);
}

TEST_F(TestsOAuthControllers_TokenController, post_Success)
{
	this->request_mock.set_json({
		{"grant_type", "client_credentials"},
		{"client_id", this->client.client_id},
		{"client_secret", this->client.client_secret}
	});
	auto response = this->controller->post(&this->request_mock);

	ASSERT_EQ(response->get_header(xw::http::CACHE_CONTROL, ""), "no-store");
	ASSERT_EQ(response->get_header("Pragma", ""), "no-cache");

	auto json_content = nlohmann::json::parse(response->get_content());

	ASSERT_TRUE(json_content.contains("expires_in"));
	ASSERT_EQ(json_content["expires_in"].get<time_t>(), (time_t)this->exp_period.total_seconds());
	ASSERT_TRUE(json_content.contains("access_token"));
	ASSERT_TRUE(json_content.contains("token_type"));
	ASSERT_EQ(json_content["token_type"].get<std::string>(), "Bearer");
}

TEST_F(TestsOAuthControllers_TokenController, dispatch_Success)
{
	this->request_mock.set_json({
		{"grant_type", "client_credentials"},
		{"client_id", this->client.client_id},
		{"client_secret", this->client.client_secret}
	});
	this->request_mock.set_method("POST");
	auto response = this->controller->dispatch(&this->request_mock);

	ASSERT_EQ(response->get_header(xw::http::CACHE_CONTROL, ""), "no-store");
	ASSERT_EQ(response->get_header("Pragma", ""), "no-cache");

	auto json_content = nlohmann::json::parse(response->get_content());

	ASSERT_TRUE(json_content.contains("expires_in"));
	ASSERT_EQ(json_content["expires_in"].get<time_t>(), (time_t)this->exp_period.total_seconds());
	ASSERT_TRUE(json_content.contains("access_token"));
	ASSERT_TRUE(json_content.contains("token_type"));
	ASSERT_EQ(json_content["token_type"].get<std::string>(), "Bearer");
}

TEST_F(TestsOAuthControllers_TokenController, dispatch_ReturnsErrorResponseWhenSerializerThrowsOAuthError)
{
	this->request_mock.set_json({
		{"client_id", this->client.client_id},
		{"client_secret", this->client.client_secret}
	});
	this->request_mock.set_method("POST");
	auto response = this->controller->dispatch(&this->request_mock);
	auto json_content = nlohmann::json::parse(response->get_content());

	ASSERT_TRUE(json_content.contains("error"));
	ASSERT_TRUE(json_content.contains("error_description"));
	ASSERT_TRUE(json_content.contains("error_uri"));
}
