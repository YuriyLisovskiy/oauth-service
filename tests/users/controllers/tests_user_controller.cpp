/*
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#include <gtest/gtest.h>

#include "./request_mock.h"
#include "../user_service_mock.h"
#include "../../../src/users/controllers/user_controller.h"
#include "../../../src/users/services/exceptions.h"


class TestsUsersControllers_UserController : public ::testing::Test
{
protected:
	std::shared_ptr<UserServiceMock> user_service_mock = nullptr;
	HttpRequestMock request_mock;

	void SetUp() override
	{
		this->user_service_mock = std::make_shared<UserServiceMock>();
	}
};

TEST_F(TestsUsersControllers_UserController, contructor_ThrowsNullPointerExceptionOnNullptrService)
{
	ASSERT_THROW(UserController(nullptr, nullptr), xw::NullPointerException);
}

TEST_F(TestsUsersControllers_UserController, get_Success)
{
	auto now = xw::dt::Datetime::now(std::make_shared<xw::dt::Timezone>(xw::dt::Timezone::UTC));
	UserModel user("email@gmail.com", now, now);
	user.id = 1;
	this->user_service_mock->set_data(std::list{user});
	UserController controller(nullptr, this->user_service_mock);
	auto response = controller.get(&this->request_mock, user.id);

	ASSERT_EQ(response->get_status(), 200);

	auto json_content = nlohmann::json::parse(response->get_content());

	ASSERT_TRUE(json_content.contains("email"));
	ASSERT_EQ(user.email, json_content["email"].get<std::string>());
	ASSERT_TRUE(json_content.contains("created_at"));
	ASSERT_EQ(user.created_at.str(), json_content["created_at"].get<std::string>());
	ASSERT_TRUE(json_content.contains("updated_at"));
	ASSERT_EQ(user.updated_at.str(), json_content["updated_at"].get<std::string>());
	ASSERT_FALSE(json_content.contains("raw_password"));
}

TEST_F(TestsUsersControllers_UserController, get_UserNotFound)
{
	UserController controller(nullptr, this->user_service_mock);

	ASSERT_THROW(controller.get(&this->request_mock, 1), UserNotFoundException);
}

TEST_F(TestsUsersControllers_UserController, put_RequestDataIsNull)
{
	UserController controller(nullptr, this->user_service_mock);
	this->request_mock.set_json(nullptr);

	ASSERT_THROW(controller.put(&this->request_mock, 1), xw::http::exc::BadRequest);
}

TEST_F(TestsUsersControllers_UserController, put_Success)
{
	auto now = xw::dt::Datetime::now(std::make_shared<xw::dt::Timezone>(xw::dt::Timezone::UTC));
	UserModel user("email@gmail.com", now, now);
	user.id = 1;
	user.set_password("super-password");
	this->user_service_mock->set_data(std::list{user});
	UserController controller(nullptr, this->user_service_mock);

	auto new_email = "another-email@gmail.com";
	this->request_mock.set_json({
		{"email", new_email},
		{"raw_password", "another-super-password"}
	});
	auto response = controller.put(&this->request_mock, user.id);

	ASSERT_EQ(response->get_status(), 200);

	auto json_content = nlohmann::json::parse(response->get_content());

	ASSERT_TRUE(json_content.contains("email"));
	ASSERT_EQ(new_email, json_content["email"].get<std::string>());
	ASSERT_TRUE(json_content.contains("created_at"));
	ASSERT_EQ(user.created_at.str(), json_content["created_at"].get<std::string>());
	ASSERT_TRUE(json_content.contains("updated_at"));
}

TEST_F(TestsUsersControllers_UserController, put_UserNotFound)
{
	UserController controller(nullptr, this->user_service_mock);
	this->request_mock.set_json({
		{"email", "email@gmail.com"}
	});

	ASSERT_THROW(controller.put(&this->request_mock, 1), UserNotFoundException);
}

TEST_F(TestsUsersControllers_UserController, delete_Success)
{
	auto now = xw::dt::Datetime::now(std::make_shared<xw::dt::Timezone>(xw::dt::Timezone::UTC));
	UserModel user("email@gmail.com", now, now);
	user.id = 1;
	user.set_password("super-password");
	this->user_service_mock->set_data(std::list{user});
	UserController controller(nullptr, this->user_service_mock);
	auto response = controller.delete_(&this->request_mock, user.id);

	ASSERT_EQ(response->get_status(), 200);
}

TEST_F(TestsUsersControllers_UserController, delete_UserNotFound)
{
	UserController controller(nullptr, this->user_service_mock);

	ASSERT_THROW(controller.delete_(&this->request_mock, 1), UserNotFoundException);
}
