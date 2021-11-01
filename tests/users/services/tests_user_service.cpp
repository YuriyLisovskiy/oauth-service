/*
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#include <gtest/gtest.h>

#include "../../../src/users/services/user_service.h"
#include "./sql_backend_mock.h"


class TestsUsersServices_UserService : public ::testing::Test
{
protected:
	std::shared_ptr<DatabaseConnectionMock> connection = nullptr;
	std::shared_ptr<UserService> user_service = nullptr;

	void SetUp() override
	{
		this->connection = std::make_shared<DatabaseConnectionMock>();
		this->_backend.set_connection(this->connection);
		this->user_service = std::make_shared<UserService>(
			std::make_shared<xw::orm::Repository>(&this->_backend),
			std::make_shared<xw::dt::Timezone>(xw::dt::Timezone::UTC)
		);
	}

private:
	SQLBackendMock _backend;
};

std::vector<std::map<std::string, char*>> create_map_data(
	std::map<std::string, UserModel>& data, std::string& dt_str
)
{
	std::vector<std::map<std::string, char*>> map_data;
	map_data.reserve(data.size());
	for (auto& model : data)
	{
		map_data.push_back({
			std::map<std::string, char*>{
				{"id", (char*)model.first.data()},
				{"email", model.second.email.data()},
				{"created_at", dt_str.data()},
				{"updated_at", dt_str.data()}
			}
		});
	}

	return map_data;
}

UserModel make_user(long long id, const std::string& email, const xw::dt::Datetime& dt)
{
	UserModel user(email, dt, dt);
	user.id = id;
	return user;
}

TEST_F(TestsUsersServices_UserService, get_all)
{
	auto now = xw::dt::Datetime::now();
	auto now_str = now.str();
	std::map<std::string, UserModel> expected_result = {
		{"1", make_user(1, "email1@gmail.com", now)},
		{"2", make_user(2, "email2@gmail.com", now)},
		{"3", make_user(3, "email3@gmail.com", now)}
	};
	this->connection->set_map_rows_data(create_map_data(expected_result, now_str));
	auto actual_result = this->user_service->get_all();

	ASSERT_EQ(expected_result.size(), actual_result.size());

	for (const auto& actual_model : actual_result)
	{
		auto expected_model = expected_result[std::to_string(actual_model.id)];
		ASSERT_EQ(expected_model.id, actual_model.id);
		ASSERT_EQ(expected_model.email, actual_model.email);
	}
}

TEST_F(TestsUsersServices_UserService, list_Empty)
{
	ASSERT_TRUE(this->user_service->get_all().empty());
}

TEST_F(TestsUsersServices_UserService, get_by_id)
{
	auto now = xw::dt::Datetime::now();
	auto now_str = now.str();
	auto model = make_user(1, "email@gmail.com", now);
	std::map<std::string, UserModel> models = {{"1", model}};
	this->connection->set_map_rows_data(create_map_data(models, now_str));
	auto model_got = this->user_service->get_by_id(model.id);

	ASSERT_EQ(model.id, model_got.id);
	ASSERT_EQ(model.email, model_got.email);
}

TEST_F(TestsUsersServices_UserService, get_by_id_NonExistent)
{
	auto model = this->user_service->get_by_id(777);

	ASSERT_TRUE(model.is_null());
}

TEST_F(TestsUsersServices_UserService, create)
{
	std::string email = "email@gmail.com";
	auto created_model = this->user_service->create(email, "super-password");

	ASSERT_FALSE(created_model.is_null());
	ASSERT_EQ(created_model.email, email);
}

TEST_F(TestsUsersServices_UserService, remove_NonExistent)
{
	auto removed_model = this->user_service->remove(777);

	ASSERT_TRUE(removed_model.is_null());
}

TEST_F(TestsUsersServices_UserService, remove)
{
	auto now = xw::dt::Datetime::now();
	auto now_str = now.str();
	auto model = make_user(1, "email@gmail.com", now);
	std::map<std::string, UserModel> models = {{"1", model}};
	this->connection->set_map_rows_data(create_map_data(models, now_str));
	auto removed_model = this->user_service->remove(model.id);

	ASSERT_EQ(model.id, removed_model.id);
	ASSERT_EQ(model.email, removed_model.email);
}

TEST_F(TestsUsersServices_UserService, update_NonExistent)
{
	auto updated_model = this->user_service->update(777, "", "");

	ASSERT_TRUE(updated_model.is_null());
}

TEST_F(TestsUsersServices_UserService, update)
{
	auto now = xw::dt::Datetime::now(std::make_shared<xw::dt::Timezone>(xw::dt::Timezone::UTC));
	auto now_str = now.str();
	auto model = make_user(1, "email@gmail.com", now);
	std::map<std::string, UserModel> models = {{"1", model}};
	this->connection->set_map_rows_data(create_map_data(models, now_str));
	auto updated_model = this->user_service->update(model.id, "new_email@gmail.com", {});

	ASSERT_FALSE(updated_model.is_null());
	ASSERT_EQ(model.id, updated_model.id);
	ASSERT_NE(model.email, updated_model.email);
	ASSERT_LE(model.updated_at, updated_model.updated_at);
}
