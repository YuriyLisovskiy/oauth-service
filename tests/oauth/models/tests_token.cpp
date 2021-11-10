/**
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#include <gtest/gtest.h>

#include "../../../src/oauth/models/token.h"


class TestsOAuthModels_Token : public ::testing::Test
{
protected:
	std::string access_token = "access-token-1.access-token-2.access-token-3";
	std::string token_type = "Bearer";
	time_t expires_in = 3600;
};

TEST_F(TestsOAuthModels_Token, to_json)
{
	nlohmann::json expected_result = {
		{"access_token", this->access_token},
		{"token_type", this->token_type},
		{"expires_in", this->expires_in}
	};
	auto actual_result = Token(this->access_token, this->token_type, this->expires_in).to_json();

	ASSERT_EQ(expected_result, actual_result);
}
