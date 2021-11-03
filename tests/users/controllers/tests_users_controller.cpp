/*
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#include <gtest/gtest.h>

#include "./request_mock.h"
#include "../user_service_mock.h"
#include "../../../src/users/controllers/users_controller.h"


class TestsUsersControllers_UsersController : public ::testing::Test
{
protected:
	std::shared_ptr<UserServiceMock> user_service_mock = nullptr;
	HttpRequestMock request_mock;

	void SetUp() override
	{
		this->user_service_mock = std::make_shared<UserServiceMock>();
	}
};

TEST_F(TestsUsersControllers_UsersController, get_EmptyUsersList)
{
	UsersController controller(nullptr, this->user_service_mock);
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
	UsersController controller(nullptr, this->user_service_mock);
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
	UsersController controller(nullptr, this->user_service_mock);
	this->request_mock.set_json(nullptr);

	ASSERT_THROW(auto _ = controller.post(&this->request_mock), xw::http::exc::BadRequest);
}

TEST_F(TestsUsersControllers_UsersController, post_Success)
{
	auto now = xw::dt::Datetime::now(std::make_shared<xw::dt::Timezone>(xw::dt::Timezone::UTC));
	auto user = UserModel("email@gmail.com", now, now);
	this->user_service_mock->set_data(std::list{user});
	UsersController controller(nullptr, this->user_service_mock);
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
