/*
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#include <gtest/gtest.h>

#include "../../request_mock.h"
#include "../user_service_mock.h"
#include "../../../src/users/controllers/users_controller.h"


class UsersControllerToTest : public UsersController
{
public:
	explicit inline UsersControllerToTest(const xw::ILogger* logger, const OAuthConfig& oauth_config) :
		UsersController(logger, oauth_config)
	{
	}

protected:
	[[nodiscard]]
	inline bool client_exists(const std::string&) const override
	{
		return true;
	}
};

class TestsUsersControllers_UsersController : public ::testing::Test
{
protected:
	std::shared_ptr<UserServiceMock> user_service_mock = nullptr;
	HttpRequestMock request_mock;
	std::string token;
	OAuthConfig oauth_config;

	void SetUp() override
	{
		this->user_service_mock = std::make_shared<UserServiceMock>();

		this->oauth_config.JWT.TOKEN_TYPE = "Bearer";
		this->oauth_config.JWT.ISSUER = "test";
		this->oauth_config.JWT.SUBJECT = "test";
		this->oauth_config.JWT.PERIOD = xw::dt::Timedelta(0, 30);
		this->oauth_config.SIGNER = xw::crypto::get_hs_signer("HS256", "test-secret");

		auto now = xw::dt::Datetime::utc_now();
		nlohmann::json claims = {
			{xw::crypto::jwt::iat, now.timestamp()},
			{xw::crypto::jwt::exp, (time_t)(now + this->oauth_config.JWT.PERIOD).timestamp()},
			{xw::crypto::jwt::iss, this->oauth_config.JWT.ISSUER},
			{xw::crypto::jwt::sub, this->oauth_config.JWT.SUBJECT},
			{xw::crypto::jwt::aud, "some-id"}
		};
		this->token = xw::crypto::jwt::sign(this->oauth_config.SIGNER.get(), claims);
	}
};

TEST_F(TestsUsersControllers_UsersController, get_EmptyUsersList)
{
	UsersControllerToTest controller(nullptr, this->oauth_config);
	controller.set_user_service(this->user_service_mock);
	this->request_mock.set_header("Authorization", this->oauth_config.JWT.TOKEN_TYPE + " " + this->token);
	auto response = controller.get(&this->request_mock);
	auto json_content = nlohmann::json::parse(response->get_content());

	nlohmann::json expected_json_content = {
		{"users", nlohmann::json(nlohmann::json::value_t::array)}
	};
	ASSERT_EQ(json_content, expected_json_content);
}

TEST_F(TestsUsersControllers_UsersController, get_NonEmptyUsersList)
{
	auto now = xw::dt::Datetime::now(std::make_shared<xw::dt::Timezone>(xw::dt::Timezone::UTC));
	auto users_data = std::vector<UserModel>{
		UserModel("email1@gmail.com", now, now),
		UserModel("email2@gmail.com", now, now)
	};
	this->user_service_mock->set_data(users_data);
	UsersControllerToTest controller(nullptr, this->oauth_config);
	controller.set_user_service(this->user_service_mock);
	this->request_mock.set_header("Authorization", this->oauth_config.JWT.TOKEN_TYPE + " " + this->token);
	auto response = controller.get(&this->request_mock);
	auto json_content = nlohmann::json::parse(response->get_content());

	ASSERT_TRUE(json_content.contains("users"));

	auto json_users = json_content["users"];

	ASSERT_EQ(users_data.size(), json_users.size());

	size_t i = 0;
	for (const auto& json_user : json_users)
	{
		auto user_model = users_data[i++];

		ASSERT_EQ(json_user["email"].get<std::string>(), user_model.email);
		ASSERT_EQ(json_user["created_at"].get<std::string>(), user_model.created_at.str());
		ASSERT_EQ(json_user["updated_at"].get<std::string>(), user_model.updated_at.str());
	}
}

TEST_F(TestsUsersControllers_UsersController, post_RequestJsonIsNullptr)
{
	UsersControllerToTest controller(nullptr, this->oauth_config);
	controller.set_user_service(this->user_service_mock);
	this->request_mock.set_header("Authorization", this->oauth_config.JWT.TOKEN_TYPE + " " + this->token);
	this->request_mock.set_json(nullptr);

	ASSERT_THROW(auto _ = controller.post(&this->request_mock), xw::http::exc::BadRequest);
}

TEST_F(TestsUsersControllers_UsersController, post_Success)
{
	auto now = xw::dt::Datetime::now(std::make_shared<xw::dt::Timezone>(xw::dt::Timezone::UTC));
	auto user = UserModel("email@gmail.com", now, now);
	this->user_service_mock->set_data(std::list{user});
	UsersControllerToTest controller(nullptr, this->oauth_config);
	controller.set_user_service(this->user_service_mock);
	this->request_mock.set_header("Authorization", this->oauth_config.JWT.TOKEN_TYPE + " " + this->token);
	this->request_mock.set_json({
		{"email", user.email},
		{"raw_password", "super-password"}
	});
	auto response = controller.post(&this->request_mock);
	auto json_content = nlohmann::json::parse(response->get_content());

	ASSERT_TRUE(json_content.contains("email"));
	ASSERT_EQ(json_content["email"].get<std::string>(), user.email);
	ASSERT_FALSE(json_content.contains("raw_password"));
	ASSERT_TRUE(json_content.contains("created_at"));
	ASSERT_EQ(json_content["created_at"].get<std::string>(), user.created_at.str());
	ASSERT_TRUE(json_content.contains("updated_at"));
	ASSERT_EQ(json_content["updated_at"].get<std::string>(), user.updated_at.str());
}
