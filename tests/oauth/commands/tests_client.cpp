/**
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#include <memory>
#include <vector>
#include <list>
#include <initializer_list>

#include <gtest/gtest.h>

#include "../../../src/oauth/commands/client.h"

#include "../../logger_mock.h"
#include "../client_service_mock.h"


template <class CommandType>
class ClientCommandsTestBase : public ::testing::Test
{
protected:
	std::shared_ptr<LoggerMock> logger_mock;
	std::shared_ptr<ClientServiceMock> client_service_mock;
	CommandType* command = nullptr;

	void SetUp() override
	{
		this->logger_mock = std::make_shared<LoggerMock>();
		this->client_service_mock = std::make_shared<ClientServiceMock>();
		this->command = new CommandType(this->logger_mock, this->client_service_mock);
	}

	void TearDown() override
	{
		delete this->command;
	}

	void run_command(const std::initializer_list<std::string>& args)
	{
		std::vector<char*> argv;
		argv.reserve(args.size());
		for (const auto& arg : args)
		{
			argv.push_back((char*)arg.data());
		}

		this->command->run_from_argv(argv.size(), argv.data(), 0);
	}
};

class TestsClientCommands_NewCommand : public ClientCommandsTestBase<NewClientCommand>
{
};

TEST_F(TestsClientCommands_NewCommand, create)
{
	std::string client_id = "5f3a74e4-d25b-434f-9018-93cf10738099";
	this->run_command({"--id", client_id});
	ASSERT_TRUE(this->client_service_mock->has_client(client_id));
}

class TestsClientCommands_ListCommand : public ClientCommandsTestBase<ListClientsCommand>
{
};

TEST_F(TestsClientCommands_ListCommand, list_EmptyResult)
{
	this->logger_mock->set_info_assertion_function([](const auto& msg, auto, auto, auto)
	{
		ASSERT_EQ(msg, "No clients.");
	});
	this->run_command({});
}

TEST_F(TestsClientCommands_ListCommand, list_NotEmptyResult)
{
	auto now = xw::dt::Datetime::now(std::make_shared<xw::dt::Timezone>(xw::dt::Timezone::UTC));
	std::list<ClientModel> clients = {
		ClientModel(_generate_uuid4(), _generate_random_alphanum_string(64), now, now),
		ClientModel(_generate_uuid4(), _generate_random_alphanum_string(64), now, now),
		ClientModel(_generate_uuid4(), _generate_random_alphanum_string(64), now, now),
		ClientModel(_generate_uuid4(), _generate_random_alphanum_string(64), now, now),
		ClientModel(_generate_uuid4(), _generate_random_alphanum_string(64), now, now),
		ClientModel(_generate_uuid4(), _generate_random_alphanum_string(64), now, now)
	};
	this->client_service_mock->set_data(clients);
	std::string message = "List:\n\n" +
		xw::str::join("\n\n", clients.begin(), clients.end(), [](const auto& client) -> std::string
		{
			return client.to_string();
		});
	this->logger_mock->set_info_assertion_function([message](const auto& msg, auto, auto, auto)
	{
		ASSERT_EQ(msg, message);
	});
	this->run_command({});
}

class TestsClientCommands_UpdateCommand : public ClientCommandsTestBase<UpdateClientCommand>
{
};

TEST_F(TestsClientCommands_UpdateCommand, update_ExistentClient)
{
	auto now = xw::dt::Datetime::now(std::make_shared<xw::dt::Timezone>(xw::dt::Timezone::UTC));
	ClientModel model(_generate_uuid4(), _generate_random_alphanum_string(64), now, now);
	this->client_service_mock->set_data({model});
	this->run_command({"--id", model.client_id});
	auto updated_model = this->client_service_mock->get_client(model.client_id);

	ASSERT_FALSE(updated_model.is_null());
	ASSERT_TRUE(updated_model.client_secret != model.client_secret);
	ASSERT_EQ(updated_model.created_at, model.created_at);
	ASSERT_TRUE(updated_model.updated_at >= model.updated_at);
}

TEST_F(TestsClientCommands_UpdateCommand, update_NonExistentClient)
{
	std::string client_id = "5f3a74e4-d25b-434f-9018-93cf10738099";
	this->run_command({"--id", client_id});
	this->logger_mock->set_error_assertion_function([client_id](const auto& msg, auto, auto, auto)
	{
		ASSERT_EQ(msg, "Client with id '" + client_id + "' is not found.");
	});
}

TEST_F(TestsClientCommands_UpdateCommand, update_NonExistentClientWithEmptyId)
{
	this->run_command({"--id"});
	this->logger_mock->set_error_assertion_function([](const auto& msg, auto, auto, auto)
	{
		ASSERT_EQ(msg, "Client with empty id is not found.");
	});
}

class TestsClientCommands_DeleteCommand : public ClientCommandsTestBase<DeleteClientCommand>
{
};

TEST_F(TestsClientCommands_DeleteCommand, delete_ExistentClient)
{
	auto now = xw::dt::Datetime::now(std::make_shared<xw::dt::Timezone>(xw::dt::Timezone::UTC));
	ClientModel model(_generate_uuid4(), _generate_random_alphanum_string(64), now, now);
	this->client_service_mock->set_data({model});
	this->logger_mock->set_error_assertion_function([model](const auto& msg, auto, auto, auto)
	{
		ASSERT_EQ(msg, "Deleted:\n" + model.to_string());
	});
	this->run_command({"--id", model.client_id});
}

TEST_F(TestsClientCommands_DeleteCommand, delete_NonExistentClient)
{
	std::string client_id = "5f3a74e4-d25b-434f-9018-93cf10738099";
	this->run_command({"--id", client_id});
	this->logger_mock->set_error_assertion_function([client_id](const auto& msg, auto, auto, auto)
	{
		ASSERT_EQ(msg, "Client with id '" + client_id + "' is not found.");
	});
}

TEST_F(TestsClientCommands_DeleteCommand, delete_NonExistentClientWithEmptyId)
{
	this->run_command({"--id"});
	this->logger_mock->set_error_assertion_function([](const auto& msg, auto, auto, auto)
	{
		ASSERT_EQ(msg, "Client with empty id is not found.");
	});
}
