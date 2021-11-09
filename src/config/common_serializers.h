/*
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#pragma once

// STL
#include <string>
#include <concepts>
#include <memory>
#include <functional>
#include <optional>

// xalwart
#include <xalwart.base/vendor/nlohmann/json.h>
#include <xalwart.base/utility.h>
#include <xalwart.base/exceptions.h>

// oauth-service
#include "./exceptions.h"


template <class Type>
struct SerializerField
{
	using type = Type;
	std::string name;
	bool required = false;
	std::function<void(type)> validator = nullptr;

	[[nodiscard]]
	inline bool is_valid() const
	{
		return !this->name.empty();
	}
};

template <class ...Args>
class Serializer
{
public:
	explicit inline Serializer(SerializerField<Args>&& ...fields) :
		fields(std::make_tuple(std::forward<SerializerField<Args>>(fields)...))
	{
	}

	virtual inline void on_validation_error(const std::string& field_name) const
	{
	}

	inline std::tuple<std::optional<Args>...> validate(nlohmann::json data)
	{
		if (!data.is_object())
		{
			throw ValidationError(
				"JSON object is required, got " + std::string(data.type_name()),
				_ERROR_DETAILS_
			);
		}

		struct field_info
		{
			nlohmann::json data;
			bool exists = false;
		};

		std::vector<field_info> ordered_data;
		xw::util::tuple_for_each(this->fields, [&data, &ordered_data, this](size_t index, const auto& field)
		{
			if (!field.is_valid())
			{
				throw xw::ArgumentError("serializer field is invalid", _ERROR_DETAILS_);
			}

			using field_type = typename std::remove_reference<decltype(field)>::type;
			using type = typename field_type::type;
			bool field_exists = false;
			if (data.template contains(field.name))
			{
				if (field.validator)
				{
					field.validator(data[field.name].template get<type>());
				}

				field_exists = true;
			}
			else if (field.required)
			{
				this->on_validation_error(field.name);
				throw ValidationError("missing '" + field.name + "' field", _ERROR_DETAILS_);
			}
			else
			{
				data[field.name] = type();
			}

			ordered_data.push_back({data[field.name], field_exists});
		});

		std::tuple<std::optional<Args>...> validated_data;
		xw::util::tuple_for_each(validated_data, [&](size_t i, auto& elem) -> void {
			auto item = ordered_data[i];
			if (item.exists)
			{
				elem = item.data.template get<typename std::remove_reference<decltype(*elem)>::type>();
			}
		});

		return validated_data;
	}

protected:
	std::tuple<SerializerField<Args>...> fields;
};

template<class T>
concept default_initializable =
	std::is_constructible_v<T> &&
	requires { T{}; } &&
	requires { ::new (static_cast<void*>(nullptr)) T; };

template <default_initializable ModelType, class ...Args>
class ModelSerializer : public Serializer<Args...>
{
public:
	explicit inline ModelSerializer(SerializerField<Args>&& ...fields) :
		Serializer<Args...>(std::forward<SerializerField<Args>>(fields)...)
	{
	}

	[[nodiscard]]
	virtual inline ModelType save(const nlohmann::json& data)
	{
		auto validated_data = this->validate(data);
		return std::apply(
			[this](std::optional<Args>... a) -> auto { return this->process(a...); },
			std::forward<std::tuple<std::optional<Args>...>>(validated_data)
		);
	}

	virtual ModelType process(std::optional<Args>...) = 0;
};
