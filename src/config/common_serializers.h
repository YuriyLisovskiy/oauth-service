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


template <class Type>
struct SerializerField
{
	using type = Type;
	std::string name;
	bool required = false;
	std::function<bool(type)> validator = nullptr;

	[[nodiscard]]
	inline bool is_valid() const
	{
		return !this->name.empty();
	}
};

template<class T>
concept default_initializable =
	std::is_constructible_v<T> &&
	requires { T{}; } &&
	requires { ::new (static_cast<void*>(nullptr)) T; };

template <default_initializable ModelType, class ...Args>
class Serializer
{
public:
	explicit inline Serializer(SerializerField<Args>&& ...fields) :
		fields(std::make_tuple(std::forward<SerializerField<Args>>(fields)...))
	{
	}

	inline std::tuple<std::optional<Args>...> validate(nlohmann::json data)
	{
		struct field_info
		{
			nlohmann::json data;
			bool exists = false;
		};

		std::vector<field_info> ordered_data;
		xw::util::tuple_for_each(this->fields, [&data, &ordered_data](size_t index, const auto& field)
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
				// TODO: throw validation error
				throw xw::BaseException(
					std::string("field '" + field.name + "' is required").c_str(), _ERROR_DETAILS_
				);
			}
			else
			{
				data[field.name] = type();
			}

			ordered_data.push_back({data[field.name], field_exists});
		});

		std::tuple<Args...> validated_data;
		xw::util::tuple_for_each(validated_data, [&](size_t i, auto& elem) -> void {
			auto item = ordered_data[i];
			if (item.exists)
			{
				elem = item.data.template get<typename std::remove_reference<decltype(elem)>::type>();
			}
		});

		return validated_data;
	}

	[[nodiscard]]
	virtual ModelType save(nlohmann::json data) = 0;

protected:
	std::tuple<SerializerField<Args>...> fields;
};

template <default_initializable ModelType, class ...Args>
class CreateModelSerializer : public Serializer<ModelType, Args...>
{
public:
	explicit inline CreateModelSerializer(SerializerField<Args> ...fields) :
		Serializer<ModelType, Args...>(std::forward<SerializerField<Args>>(fields)...)
	{
	}

	virtual inline ModelType create(std::optional<Args>...)
	{
		return {};
	}

	[[nodiscard]]
	inline ModelType save(nlohmann::json data) override
	{
		auto validated_data = this->validate(data);
		return std::apply(
			[this](std::optional<Args>... a) -> auto { return this->create(a...); },
			validated_data
		);
	}
};

template <default_initializable ModelType, class ...Args>
class UpdateModelSerializer : public Serializer<ModelType, Args...>
{
public:
	explicit inline UpdateModelSerializer(SerializerField<Args> ...fields) :
		Serializer<ModelType, Args...>(std::forward<SerializerField<Args>>(fields)...)
	{
	}

	virtual inline ModelType update(std::optional<Args>...)
	{
		return {};
	}

	[[nodiscard]]
	inline ModelType save(nlohmann::json data) override
	{
		auto validated_data = this->validate(data);
		return std::apply(
			[this](std::optional<Args>... a) -> auto { return this->update(a...); },
			validated_data
		);
	}
};
