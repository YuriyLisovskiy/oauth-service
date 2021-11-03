/*
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#include <memory>

#include <gtest/gtest.h>

#include "../../../src/users/serializers/user_serializers.h"
#include "../user_service_mock.h"


class TestsUsersSerializers_CreateUserSerializer : public ::testing::Test
{
protected:
	std::shared_ptr<CreateUserSerializer> serializer = nullptr;
	std::shared_ptr<UserServiceMock> user_service_mock = nullptr;

	inline void SetUp() override
	{
		this->user_service_mock = std::make_shared<UserServiceMock>();
		this->serializer = std::make_shared<CreateUserSerializer>(this->user_service_mock);
	}
};

TEST_F(TestsUsersSerializers_CreateUserSerializer, process_Create)
{
	auto now = UserServiceMock::get_now();
	UserModel model("email@email.com", now, now);
	model.id = 1;
	std::string new_password = "new-super-password";
	model.set_password(new_password);
	this->user_service_mock->set_data(std::list<UserModel>{model});
	auto created_model = this->serializer->process(model.email, new_password);

	ASSERT_FALSE(created_model.is_null());
	ASSERT_EQ(created_model.id, model.id);
	ASSERT_EQ(created_model.email, model.email);
	ASSERT_TRUE(created_model.check_password(new_password));
}

class TestsUsersSerializers_UpdateUserSerializer : public ::testing::Test
{
protected:
	std::shared_ptr<UpdateUserSerializer> serializer = nullptr;
	std::shared_ptr<UserServiceMock> user_service_mock = nullptr;

	inline void SetUp() override
	{
		this->user_service_mock = std::make_shared<UserServiceMock>();
		this->serializer = std::make_shared<UpdateUserSerializer>(this->user_service_mock);
	}
};

TEST_F(TestsUsersSerializers_UpdateUserSerializer, process_UpdateEmailOnly)
{
	auto now = UserServiceMock::get_now();
	UserModel model("email@email.com", now, now);
	model.id = 1;
	model.set_password("hello-world");
	this->user_service_mock->set_data(std::list<UserModel>{model});
	std::string new_email = "my_email@email.com";
	auto updated_model = this->serializer->process(model.id, new_email, {});

	ASSERT_FALSE(updated_model.is_null());
	ASSERT_EQ(updated_model.id, model.id);
	ASSERT_EQ(updated_model.email, new_email);
}

TEST_F(TestsUsersSerializers_UpdateUserSerializer, process_UpdatePasswordOnly)
{
	auto now = UserServiceMock::get_now();
	UserModel model("email@email.com", now, now);
	model.id = 1;
	model.set_password("hello-world");
	this->user_service_mock->set_data(std::list<UserModel>{model});
	std::string new_password = "super-password-new";
	auto updated_model = this->serializer->process(model.id, "my_email@email.com", new_password);

	ASSERT_FALSE(updated_model.is_null());
	ASSERT_EQ(updated_model.id, model.id);
	ASSERT_TRUE(updated_model.check_password(new_password));
	ASSERT_FALSE(model.check_password(new_password));
}

TEST_F(TestsUsersSerializers_UpdateUserSerializer, process_UpdateAll)
{
	auto now = UserServiceMock::get_now();
	UserModel model("email@email.com", now, now);
	model.id = 1;
	this->user_service_mock->set_data(std::list<UserModel>{model});
	std::string new_email = "my_email@email.com";
	std::string new_password = "super-password-new";
	auto updated_model = this->serializer->process(model.id, new_email, new_password);

	ASSERT_FALSE(updated_model.is_null());
	ASSERT_EQ(updated_model.id, model.id);
	ASSERT_EQ(updated_model.email, new_email);
	ASSERT_TRUE(updated_model.check_password(new_password));
	ASSERT_FALSE(model.check_password(new_password));
}
