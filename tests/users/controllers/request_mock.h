/*
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#pragma once

#include <list>
#include <string>

#include <xalwart/http/interfaces.h>


class HttpRequestMock : public xw::http::IRequest
{
public:
	[[nodiscard]]
	inline std::string method() const override
	{
		return "";
	}

	[[nodiscard]]
	inline const xw::http::URL& url() const override
	{
	}

	[[nodiscard]]
	inline Proto proto() const override
	{
		return {};
	}

	[[nodiscard]]
	inline const std::map<std::string, std::string>& headers() const override
	{
	}

	[[nodiscard]]
	inline std::string user_agent() const override
	{
		return "";
	}

	[[nodiscard]]
	inline bool has_header(const std::string& key) const override
	{
		return false;
	}

	[[nodiscard]]
	inline std::string get_header(const std::string& key, const std::string& default_value) const override
	{
		return "";
	}

	inline void set_header(const std::string& key, const std::string& value) override
	{
	}

	[[nodiscard]]
	inline bool is_json() const override
	{
		return false;
	}

	[[nodiscard]]
	inline std::vector<xw::http::Cookie> cookies() const override
	{
		return {};
	}

	[[nodiscard]]
	inline std::optional<xw::http::Cookie> cookie(const std::string& name) const override
	{
		return {};
	}

	[[nodiscard]]
	inline std::string referer() const override
	{
		return "";
	}

	inline const xw::http::Query& form() override
	{
	}

	inline const xw::http::mime::multipart::Form& multipart_form() override
	{
	}

	inline nlohmann::json json() override
	{
		return this->_json_data;
	}

	[[nodiscard]]
	inline std::string host() const override
	{
		return "";
	}

	[[nodiscard]]
	inline ssize_t content_length() const override
	{
		return 0;
	}

	[[nodiscard]]
	inline const std::vector<std::string>& transfer_encoding() const override
	{
	}

	[[nodiscard]]
	inline const std::map<std::string, std::string>& environment() const override
	{
	}

	[[nodiscard]]
	inline std::string scheme(
		const std::optional<xw::conf::Secure::Header>& secure_proxy_ssl_header
	) const override
	{
		return "";
	}

	inline std::string get_host(
		const std::optional<xw::conf::Secure::Header>& secure_proxy_ssl_header,
		bool use_x_forwarded_host, bool use_x_forwarded_port,
		bool debug, std::vector<std::string> allowed_hosts
	) override
	{
		return "";
	}

	[[nodiscard]]
	inline bool is_secure(const std::optional<xw::conf::Secure::Header>& secure_proxy_ssl_header) const override
	{
		return false;
	}

	void set_json(nlohmann::json data)
	{
		this->_json_data = std::move(data);
	}

private:
	nlohmann::json _json_data = nullptr;
};
