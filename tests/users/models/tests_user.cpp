/*
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#include <gtest/gtest.h>

#include "../../../src/users/models/user.h"


class TestsUsersModels_UserModel : public ::testing::Test
{
protected:
	xw::dt::Datetime now;
	UserModel user;

	void SetUp() override
	{
		this->now = xw::dt::Datetime::now(std::make_shared<xw::dt::Timezone>(xw::dt::Timezone::UTC));
		this->user = UserModel("email@gmail.com", this->now, this->now);
		this->user.id = 1;
	}
};

TEST_F(TestsUsersModels_UserModel, to_json)
{
	nlohmann::json expected = {
		{"id", this->user.id},
		{"email", this->user.email},
		{"created_at", this->user.created_at.str()},
		{"updated_at", this->user.updated_at.str()}
	};

	ASSERT_EQ(expected, this->user.to_json());
}

TEST_F(TestsUsersModels_UserModel, to_json_IsNull)
{
	UserModel model;
	model.mark_as_null();
	ASSERT_EQ(nullptr, model.to_json());
}

TEST_F(TestsUsersModels_UserModel, SetAndCheckPassword)
{
	std::string password = "super-secret-password";
	this->user.set_password(password);

	ASSERT_TRUE(this->user.check_password(password));
}
