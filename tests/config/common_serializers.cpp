/**
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#include <gtest/gtest.h>

#include "../../src/config/common_serializers.h"


TEST(TestsConfig_SerializerField, is_valid_True)
{
	SerializerField<int> field{.name="test_int"};

	ASSERT_TRUE(field.is_valid());
}

TEST(TestsConfig_SerializerField, is_valid_False)
{
	SerializerField<int> field{};
	ASSERT_FALSE(field.is_valid());
}

TEST(TestsConfig_Serializer, validate_ThrowsInvalidData)
{
	Serializer<int> serializer({.name="test_int"});

	ASSERT_THROW(serializer.validate({{}}), ValidationError);
}

TEST(TestsConfig_Serializer, validate_ThrowsInvalidField)
{
	Serializer<int> serializer({.name=""});

	ASSERT_THROW(
		serializer.validate(nlohmann::json(nlohmann::json::value_t::object)),
		xw::ArgumentError
	);
}

TEST(TestsConfig_Serializer, validate_ThrowsOnMissingRequiredField)
{
	Serializer<int> serializer({.name="test_int", .required=true});

	ASSERT_THROW(
		serializer.validate(nlohmann::json(nlohmann::json::value_t::object)),
		ValidationError
	);
}

TEST(TestsConfig_Serializer, validate_NoThrowOnMissingOptionalField)
{
	Serializer<int> serializer({.name="test_int", .required=false});

	ASSERT_NO_THROW(serializer.validate(nlohmann::json(nlohmann::json::value_t::object)));
}

TEST(TestsConfig_Serializer, validate_SuccessWithRequiredField)
{
	Serializer<int> serializer({.name="test_int", .required=true});
	int expected = 777;
	auto actual = serializer.validate({{"test_int", expected}});

	ASSERT_EQ(std::tuple_size<decltype(actual)>::value, 1);
	ASSERT_EQ(std::get<0>(actual).value(), expected);
}

TEST(TestsConfig_Serializer, validate_SuccessWithOptionalFieldWithValue)
{
	Serializer<int> serializer({.name="test_int", .required=false});
	int expected = 777;
	auto actual = serializer.validate({{"test_int", expected}});

	ASSERT_EQ(std::tuple_size<decltype(actual)>::value, 1);
	ASSERT_TRUE(std::get<0>(actual).has_value());
	ASSERT_EQ(std::get<0>(actual).value(), expected);
}

TEST(TestsConfig_Serializer, validate_SuccessWithOptionalFieldWithoutValue)
{
	Serializer<int> serializer({.name="test_int", .required=false});
	auto actual = serializer.validate(nlohmann::json(nlohmann::json::value_t::object));

	ASSERT_EQ(std::tuple_size<decltype(actual)>::value, 1);
	ASSERT_FALSE(std::get<0>(actual).has_value());
}

class CustomSerializer : public Serializer<int>
{
public:
	explicit inline CustomSerializer() : Serializer({.name="test_int", .required=true})
	{
	}

	void on_validation_error(const std::string& field_name) const override
	{
		throw std::runtime_error("missing '" + field_name + "' field");
	}
};

TEST(TestsConfig_Serializer, validate_ThrowCustomValidationError)
{
	CustomSerializer serializer;

	ASSERT_THROW(
		serializer.validate(nlohmann::json(nlohmann::json::value_t::object)),
		std::runtime_error
	);
}

TEST(TestsConfig_Serializer, validate_ThrowInFieldValidator)
{
	Serializer<int> serializer({
		.name="test_int",
		.required=false,
		.validator=[](auto value)
		{
			if (value / 100 != 1)
			{
				throw std::runtime_error("first number is not 1");
			}
		}
	});

	ASSERT_THROW(serializer.validate({{"test_int", 777}}), std::runtime_error);
}

struct CustomModel
{
	int id;
};

class CustomModelSerializer : public ModelSerializer<CustomModel, int>
{
public:
	CustomModelSerializer() : ModelSerializer(
		{.name="test_int", .required=true}
	)
	{
	}

	CustomModel process(std::optional<int> id) override
	{
		return {id.value()};
	}
};

TEST(TestsConfig_ModelSerializer, process)
{
	CustomModelSerializer serializer;
	int expected = 777;
	auto model = serializer.process(std::make_optional(expected));

	ASSERT_EQ(model.id, expected);
}

TEST(TestsConfig_ModelSerializer, save)
{
	CustomModelSerializer serializer;
	int expected = 777;
	default_initializable auto model = serializer.save({{"test_int", expected}});

	ASSERT_EQ(model.id, expected);
}
