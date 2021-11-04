/**
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#include <memory>
#include <filesystem>

#include <gtest/gtest.h>

#include <xalwart.orm/sqlite3/backend.h>
#include <xalwart.orm/db/migration.h>
#include <xalwart.orm/db/executor.h>

#include "../../../src/oauth/exceptions.h"
#include "../../../src/oauth/services/client_service.h"
#include "../../../src/config/migrations/001_create_client.h"


class TestsOAuthServices_ClientService : public ::testing::Test
{
protected:
	std::shared_ptr<ClientService> client_service = nullptr;
	std::string db_file = "/tmp/test.db";

	void SetUp() override
	{
		this->_backend = std::make_shared<xw::orm::sqlite3::Backend>(1, this->db_file.c_str());
		this->_backend->create_pool();
		std::list<std::shared_ptr<xw::orm::db::Migration>> migrations = {
			std::make_shared<Migration001_CreateClient>(this->_backend.get())
		};
		auto executor = xw::orm::db::MigrationExecutor(
			this->_backend.get(), migrations,
			[](auto msg, auto end) { /* logging */ }
		);
		executor.apply(this->_backend->schema_editor());
		this->client_service = std::make_shared<ClientService>(
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

	void set_data(const std::vector<ClientModel>& data)
	{
		auto connection = this->_backend->get_connection();
		auto query = xw::orm::q::Insert<ClientModel>(connection.get(), this->_backend->sql_builder());
		for (auto& model : data)
		{
			connection->begin_transaction();
			query = query.model(model);
			connection->end_transaction();
		}

		query.commit_batch();
		this->_backend->release_connection(connection);
	}

	std::string set_model(const ClientModel& model)
	{
		auto connection = this->_backend->get_connection();
		auto query = xw::orm::q::Insert<ClientModel>(connection.get(), this->_backend->sql_builder());
		query = query.model(model);
		std::string id;
		query.commit_one(id);
		this->_backend->release_connection(connection);
		return id;
	}

private:
	std::shared_ptr<xw::orm::sqlite3::Backend> _backend;
};

ClientModel make_client(const xw::dt::Datetime& dt)
{
	return ClientModel(_generate_uuid4(), _generate_random_alphanum_string(64), dt, dt);
}

std::vector<std::map<std::string, char*>> create_map_data(
	std::vector<ClientModel>& data, std::string& dt_str
)
{
	std::vector<std::map<std::string, char*>> map_data;
	map_data.reserve(data.size());
	for (auto& model : data)
	{
		map_data.push_back({
			std::map<std::string, char*>{
				{"client_id", (char*)model.client_id.c_str()},
				{"client_secret", (char*)model.client_secret.c_str()},
				{"created_at", dt_str.data()},
				{"updated_at", dt_str.data()}
			}
		});
	}

	return map_data;
}

TEST_F(TestsOAuthServices_ClientService, list)
{
	auto now = xw::dt::Datetime::now();
	auto now_str = now.str();
	std::vector<ClientModel> expected_result = {
		make_client(now), make_client(now), make_client(now)
	};
	this->set_data(expected_result);
	auto actual_result = this->client_service->list();

	ASSERT_EQ(expected_result.size(), actual_result.size());

	size_t i = 0;
	for (const auto& actual_model : actual_result)
	{
		auto expected_model = expected_result[i++];
		ASSERT_EQ(expected_model.client_id, actual_model.client_id);
		ASSERT_EQ(expected_model.client_secret, actual_model.client_secret);
	}
}

TEST_F(TestsOAuthServices_ClientService, list_Empty)
{
	ASSERT_TRUE(this->client_service->list().empty());
}

TEST_F(TestsOAuthServices_ClientService, create_WithDefaultId)
{
	auto created_model = this->client_service->create("");

	ASSERT_FALSE(created_model.is_null());
	ASSERT_EQ(created_model.client_id.size(), 36);
	ASSERT_EQ(created_model.client_secret.size(), 64);
}

TEST_F(TestsOAuthServices_ClientService, create_WithCustomId)
{
	auto client_id = _generate_uuid4();
	auto created_model = this->client_service->create(client_id);

	ASSERT_FALSE(created_model.is_null());
	ASSERT_EQ(created_model.client_id, client_id);
}

TEST_F(TestsOAuthServices_ClientService, remove_NonExistent)
{
	ASSERT_THROW(auto _ = this->client_service->remove("super-id"), ClientNotFoundError);
}

TEST_F(TestsOAuthServices_ClientService, remove)
{
	auto now = xw::dt::Datetime::now();
	auto now_str = now.str();
	auto model = make_client(now);
	this->set_model(model);
	auto removed_model = this->client_service->remove(model.client_id);

	ASSERT_FALSE(model.is_null());
	ASSERT_EQ(model.client_id, removed_model.client_id);
	ASSERT_EQ(model.client_secret, removed_model.client_secret);
}

TEST_F(TestsOAuthServices_ClientService, update_NonExistent)
{
	ASSERT_THROW(auto _ = this->client_service->update("super-id"), ClientNotFoundError);
}

TEST_F(TestsOAuthServices_ClientService, update)
{
	auto now = xw::dt::Datetime::now(std::make_shared<xw::dt::Timezone>(xw::dt::Timezone::UTC));
	auto now_str = now.str();
	auto model = make_client(now);
	std::vector<ClientModel> models = {model};
	this->set_model(model);
	auto updated_model = this->client_service->update(model.client_id);

	ASSERT_FALSE(updated_model.is_null());
	ASSERT_EQ(model.client_id, updated_model.client_id);
	ASSERT_NE(model.client_secret, updated_model.client_secret);
}
