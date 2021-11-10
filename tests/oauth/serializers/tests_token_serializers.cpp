/*
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#include <memory>

#include <gtest/gtest.h>

#include <xalwart.crypto/hmac.h>

#include "../../../src/oauth/serializers/token_serializers.h"
#include "../client_service_mock.h"


class TestsOAuthSerializers_CreateTokenSerializer : public ::testing::Test
{
protected:
	std::shared_ptr<CreateTokenSerializer> serializer = nullptr;
	std::shared_ptr<ClientServiceMock> client_service_mock = nullptr;
	std::string issuer = "test-issuer";
	std::string subject = "test-subject";
	xw::dt::Timedelta period = xw::dt::Timedelta(1);
	std::shared_ptr<xw::crypto::ISignatureAlgorithm> signer;
	std::string grant_type = "client_credentials";
	std::string invalid_grant_type = "non-client_credentials";
	std::string token_type = "Bearer";

	inline void SetUp() override
	{
		this->client_service_mock = std::make_shared<ClientServiceMock>();
		this->signer = std::make_shared<xw::crypto::HS256>("secret_key");
		this->serializer = std::make_shared<CreateTokenSerializer>(this->client_service_mock);
		this->serializer->set_issuer(this->issuer)
			.set_subject(this->subject)
			.set_jwt_period(this->period)
			.set_signature_algorithm(this->signer);
	}

	static inline ClientModel make_client(const xw::dt::Datetime& dt)
	{
		return ClientModel(_generate_uuid4(), _generate_random_alphanum_string(64), dt, dt);
	}
};

TEST_F(TestsOAuthSerializers_CreateTokenSerializer, process_Success)
{
	auto now = xw::dt::Datetime::now();
	auto model = make_client(now);
	this->client_service_mock->set_data(std::list<ClientModel>{model});
	auto created_model = this->serializer->process(
		this->grant_type, model.client_id, model.client_secret
	);

	ASSERT_EQ(created_model.expires_in, (time_t)this->period.total_seconds());
	ASSERT_EQ(created_model.token_type, this->token_type);
}

TEST_F(TestsOAuthSerializers_CreateTokenSerializer, process_InvalidGrantType)
{
	auto now = xw::dt::Datetime::now();
	auto model = make_client(now);
	this->client_service_mock->set_data(std::list<ClientModel>{model});

	ASSERT_THROW(auto _ = this->serializer->process(
		this->invalid_grant_type, model.client_id, model.client_secret
	), OAuthError);
}

TEST_F(TestsOAuthSerializers_CreateTokenSerializer, process_ClientNoFound)
{
	ASSERT_THROW(auto _ = this->serializer->process(
		this->grant_type, "unknown-client-id", "client-secret-key"
	), ClientNotFoundError);
}

TEST_F(TestsOAuthSerializers_CreateTokenSerializer, process_InvalidClientSecret)
{
	auto now = xw::dt::Datetime::now();
	auto model = make_client(now);
	this->client_service_mock->set_data(std::list<ClientModel>{model});

	ASSERT_THROW(auto _ = this->serializer->process(
		this->grant_type, model.client_id, "non-existent-key"
	), OAuthError);
}

TEST(TestsOAuthSerializers_CreateTokenSerializer_Constructor, ThrowsOnNullptrService)
{
	ASSERT_THROW(CreateTokenSerializer(nullptr), xw::NullPointerException);
}
