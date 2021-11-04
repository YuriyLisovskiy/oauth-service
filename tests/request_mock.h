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
		return this->_method;
	}

	[[nodiscard]]
	inline const xw::http::URL& url() const override
	{
		return this->_url;
	}

	[[nodiscard]]
	inline Proto proto() const override
	{
		return this->_proto;
	}

	[[nodiscard]]
	inline const std::map<std::string, std::string>& headers() const override
	{
		return this->_headers;
	}

	[[nodiscard]]
	inline std::string user_agent() const override
	{
		return this->_user_agent;
	}

	[[nodiscard]]
	inline bool has_header(const std::string& key) const override
	{
		return this->_headers.contains(key);
	}

	[[nodiscard]]
	inline std::string get_header(const std::string& key, const std::string& default_value) const override
	{
		return this->_headers.contains(key) ? this->_headers.at(key) : default_value;
	}

	inline void set_header(const std::string& key, const std::string& value) override
	{
		this->_headers[key] = value;
	}

	[[nodiscard]]
	inline bool is_json() const override
	{
		return this->_is_json;
	}

	[[nodiscard]]
	inline std::vector<xw::http::Cookie> cookies() const override
	{
		return this->_cookies;
	}

	[[nodiscard]]
	inline std::optional<xw::http::Cookie> cookie(const std::string& name) const override
	{
		auto cookie = std::find_if(
			this->_cookies.begin(), this->_cookies.end(),
			[name](const xw::http::Cookie& item) -> bool
			{
				return item.name() == name;
			}
		);
		if (cookie != this->_cookies.end())
		{
			return *cookie;
		}

		return {};
	}

	[[nodiscard]]
	inline std::string referer() const override
	{
		return this->_referer;
	}

	inline const xw::http::Query& form() override
	{
		return this->_form;
	}

	inline const xw::http::mime::multipart::Form& multipart_form() override
	{
		return this->_multipart_form;
	}

	inline nlohmann::json json() override
	{
		return this->_json_data;
	}

	[[nodiscard]]
	inline std::string host() const override
	{
		return this->_host;
	}

	[[nodiscard]]
	inline ssize_t content_length() const override
	{
		return this->_content_length;
	}

	[[nodiscard]]
	inline const std::vector<std::string>& transfer_encoding() const override
	{
		return this->_transfer_encoding;
	}

	[[nodiscard]]
	inline const std::map<std::string, std::string>& environment() const override
	{
		return this->_environment;
	}

	[[nodiscard]]
	inline std::string scheme(
		const std::optional<xw::conf::Secure::Header>& secure_proxy_ssl_header
	) const override
	{
		return this->_scheme;
	}

	inline std::string get_host(
		const std::optional<xw::conf::Secure::Header>& secure_proxy_ssl_header,
		bool use_x_forwarded_host, bool use_x_forwarded_port,
		bool debug, std::vector<std::string> allowed_hosts
	) override
	{
		return this->_parameterized_host;
	}

	[[nodiscard]]
	inline bool is_secure(const std::optional<xw::conf::Secure::Header>& secure_proxy_ssl_header) const override
	{
		return this->_is_secure;
	}

	void set_method(std::string method)
	{
		this->_method = std::move(method);
	}

	void set_url(xw::http::URL url)
	{
		this->_url = std::move(url);
	}

	void set_proto(Proto proto)
	{
		this->_proto = std::move(proto);
	}

	void set_headers(std::map<std::string, std::string> headers)
	{
		this->_headers = std::move(headers);
	}

	void set_user_agent(std::string user_agent)
	{
		this->_user_agent = std::move(user_agent);
	}

	void set_is_json(bool value)
	{
		this->_is_json = value;
	}

	void set_cookies(std::vector<xw::http::Cookie> cookies)
	{
		this->_cookies = std::move(cookies);
	}

	void set_referer(std::string referer)
	{
		this->_referer = std::move(referer);
	}

	void set_form(xw::http::Query form)
	{
		this->_form = std::move(form);
	}

	void set_multipart_form(xw::http::mime::multipart::Form multipart_form)
	{
		this->_multipart_form = std::move(multipart_form);
	}

	void set_json(nlohmann::json data)
	{
		this->_json_data = std::move(data);
	}

	void set_host(std::string host)
	{
		this->_host = std::move(host);
	}

	void set_content_length(ssize_t content_length)
	{
		this->_content_length = content_length;
	}

	void set_transfer_encoding(std::vector<std::string> transfer_encoding)
	{
		this->_transfer_encoding = std::move(transfer_encoding);
	}

	void set_environment(std::map<std::string, std::string> environment)
	{
		this->_environment = std::move(environment);
	}

	void set_scheme(std::string scheme)
	{
		this->_scheme = std::move(scheme);
	}

	void set_parameterized_host(std::string parameterized_host)
	{
		this->_parameterized_host = std::move(parameterized_host);
	}

	void set_is_secure(bool value)
	{
		this->_is_secure = value;
	}

private:
	std::string _method;
	xw::http::URL _url;
	Proto _proto;
	std::map<std::string, std::string> _headers;
	std::string _user_agent;
	bool _is_json;
	std::vector<xw::http::Cookie> _cookies;
	std::string _referer;
	xw::http::Query _form;
	xw::http::mime::multipart::Form _multipart_form;
	nlohmann::json _json_data = nullptr;
	std::string _host;
	ssize_t _content_length;
	std::vector<std::string> _transfer_encoding;
	std::map<std::string, std::string> _environment;
	std::string _scheme;
	std::string _parameterized_host;
	bool _is_secure;
};
