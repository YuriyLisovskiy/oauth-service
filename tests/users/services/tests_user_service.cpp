/*
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#include <memory>
#include <filesystem>

#include <gtest/gtest.h>

#include <xalwart.orm/sqlite3/backend.h>
#include <xalwart.orm/db/executor.h>

#include "../../../src/users/services/user_service.h"
#include "../../../src/users/services/exceptions.h"
#include "../../../src/config/migrations/001_create_clients.h"
#include "../../../src/config/migrations/002_create_users.h"


class TestsUsersServices_UserService : public ::testing::Test
{
protected:
	std::shared_ptr<UserService> user_service = nullptr;
	std::string db_file = "/tmp/test.db";

	void SetUp() override
	{
		this->_backend = std::make_shared<xw::orm::sqlite3::Backend>(1, this->db_file.c_str());
		this->_backend->create_pool();
		std::list<std::shared_ptr<xw::orm::db::Migration>> migrations = {
			std::make_shared<Migration001_CreateClients>(this->_backend.get()),
			std::make_shared<Migration002_CreateUsers>(this->_backend.get())
		};
		auto executor = xw::orm::db::MigrationExecutor(
			this->_backend.get(), migrations,
			[](auto msg, auto end) { /* logging */ }
		);
		executor.apply(this->_backend->schema_editor());
		this->user_service = std::make_shared<UserService>(
			std::make_shared<xw::orm::Repository>(this->_backend.get()),
			std::make_shared<xw::dt::Timezone>(xw::dt::Timezone::UTC)
		);
	}

	void TearDown() override
	{
		if (std::filesystem::exists(this->db_file))
		{
			std::filesystem::remove(this->db_file);
		}
	}

	void set_data(const std::vector<UserModel>& data)
	{
		auto connection = this->_backend->get_connection();
		auto query = xw::orm::q::Insert<UserModel>(connection.get(), this->_backend->sql_builder());
		for (auto& model : data)
		{
			connection->begin_transaction();
			query = query.model(model);
			connection->end_transaction();
		}

		query.commit_batch();
		this->_backend->release_connection(connection);
	}

	long long int set_model(const UserModel& model)
	{
		auto connection = this->_backend->get_connection();
		auto query = xw::orm::q::Insert<UserModel>(connection.get(), this->_backend->sql_builder());
		query = query.model(model);
		long long int id;
		query.commit_one(id);
		this->_backend->release_connection(connection);
		return id;
	}

private:
	std::shared_ptr<xw::orm::sqlite3::Backend> _backend;
};

TEST_F(TestsUsersServices_UserService, get_all)
{
	auto now = xw::dt::Datetime::now();
	auto now_str = now.str();

	auto user1 = UserModel("email1@gmail.com", now, now);
	user1.set_password("super-password");
	auto user2 = UserModel("email2@gmail.com", now, now);
	user2.set_password("super-password-2");
	std::vector<UserModel> expected_result = {user1, user2};
	this->set_data(expected_result);
	auto actual_result = this->user_service->get_all();

	ASSERT_EQ(expected_result.size(), actual_result.size());

	size_t i = 0;
	for (const auto& actual_model : actual_result)
	{
		auto expected_model = expected_result[i++];
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
	auto model = UserModel("email@gmail.com", now, now);
	model.set_password("super-pass");
	auto id = this->set_model(model);
	auto model_got = this->user_service->get_by_id(id);

	ASSERT_EQ(model_got.email, model.email);
}

TEST_F(TestsUsersServices_UserService, get_by_id_NonExistent)
{
	ASSERT_THROW(auto _ = this->user_service->get_by_id(777), UserNotFoundException);
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
	ASSERT_THROW(auto _ = this->user_service->remove(777), UserNotFoundException);
}

TEST_F(TestsUsersServices_UserService, remove)
{
	auto now = xw::dt::Datetime::now();
	auto now_str = now.str();
	auto model = UserModel("email@gmail.com", now, now);
	model.set_password("super-pass");
	auto id = this->set_model(model);
	auto removed_model = this->user_service->remove(id);

	ASSERT_EQ(model.email, removed_model.email);
}

TEST_F(TestsUsersServices_UserService, update_NonExistent)
{
	ASSERT_THROW(auto _ = this->user_service->update(777, "", ""), UserNotFoundException);
}

TEST_F(TestsUsersServices_UserService, update)
{
	auto now = xw::dt::Datetime::now(std::make_shared<xw::dt::Timezone>(xw::dt::Timezone::UTC));
	auto now_str = now.str();
	auto model = UserModel("email@gmail.com", now, now);
	auto id = this->set_model(model);
	std::string new_email = "new_email@gmail.com";
	auto updated_model = this->user_service->update(id, new_email, {});

	ASSERT_FALSE(updated_model.is_null());
	ASSERT_EQ(updated_model.email, new_email);
	ASSERT_LE(updated_model.updated_at, model.updated_at);
}
