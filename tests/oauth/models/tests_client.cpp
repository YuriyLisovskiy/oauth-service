/**
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#include <gtest/gtest.h>

#include "../../../src/oauth/models/client.h"


class TestsOAuthModels_ClientModel : public ::testing::Test
{
protected:
	std::string client_id = "super-id";
	std::string client_secret = "super-key";
	xw::dt::Datetime now = xw::dt::Datetime::now(
		std::make_shared<xw::dt::Timezone>(xw::dt::Timezone::UTC)
	);
};


TEST_F(TestsOAuthModels_ClientModel, to_string)
{
	std::string expected_result = "Client:\n  id: " + this->client_id +
		"\n  secret: " + this->client_secret +
		"\n  created at: " + this->now.str() +
		"\n  updated at: " + this->now.str();
	auto actual_result = ClientModel(this->client_id, this->client_secret, this->now, this->now).to_string();

	ASSERT_EQ(expected_result, actual_result);
}

TEST_F(TestsOAuthModels_ClientModel, to_json)
{
	nlohmann::json expected_result = {
		{"client_id", this->client_id},
		{"client_secret", this->client_secret},
		{"created_at", this->now.str()},
		{"updated_at", this->now.str()}
	};
	auto actual_result = ClientModel(this->client_id, this->client_secret, this->now, this->now).to_json();

	ASSERT_EQ(expected_result, actual_result);
}
