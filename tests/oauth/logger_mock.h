/**
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#pragma once

#include <gtest/gtest.h>

#include <xalwart.base/interfaces/base.h>


class LoggerMock : public xw::ILogger
{
public:
	inline void enable_colors() override {};
	inline void disable_colors() override {};

	inline void info(const std::string& msg, int line, const char* function, const char* file) override
	{
		if (this->_info_assertion_func)
		{
			this->_info_assertion_func(msg, line, function, file);
		}
	};

	inline void info(const std::string& msg) override
	{
		this->info(msg, 0, "", "");
	};

	inline void error(const std::string& msg, int line, const char* function, const char* file) override
	{
		if (this->_error_assertion_func)
		{
			this->_error_assertion_func(msg, line, function, file);
		}
	};

	inline void error(const std::string& msg) override
	{
		this->error(msg, 0, "", "");
	};

	inline void debug(const std::string& msg, int line, const char* function, const char* file) override
	{
		if (this->_debug_assertion_func)
		{
			this->_debug_assertion_func(msg, line, function, file);
		}
	};

	inline void debug(const std::string& msg) override
	{
		this->debug(msg, 0, "", "");
	};

	void warning(const std::string& msg, int line, const char* function, const char* file) override
	{
		if (this->_warning_assertion_func)
		{
			this->_warning_assertion_func(msg, line, function, file);
		}
	};

	inline void warning(const std::string& msg) override
	{
		this->warning(msg, 0, "", "");
	};

	inline void trace(const std::string& msg, int line, const char* function, const char* file) override
	{
		if (this->_trace_assertion_func)
		{
			this->_trace_assertion_func(msg, line, function, file);
		}
	};

	inline void trace(const std::string& msg) override
	{
		this->trace(msg, 0, "", "");
	};

	inline void print(const std::string& msg, Color colour, char end) override
	{
		if (this->_print_assertion_func)
		{
			this->_print_assertion_func(msg, colour, end);
		}
	};

	inline void print(const std::string& msg, Color colour) override
	{
		this->print(msg, colour, '\0');
	};

	inline void print(const std::string& msg) override
	{
		this->print(msg, xw::ILogger::Color::Default, '\0');
	};

	void info(const xw::BaseException& exc) override {};

	void debug(const xw::BaseException& exc) override {};

	void warning(const xw::BaseException& exc) override {};

	void error(const xw::BaseException& exc) override {};

	inline void set_info_assertion_function(std::function<void(const std::string&, int, const char*, const char*)> func)
	{
		this->_info_assertion_func = std::move(func);
	}

	inline void set_error_assertion_function(std::function<void(const std::string&, int, const char*, const char*)> func)
	{
		this->_error_assertion_func = std::move(func);
	}

	inline void set_debug_assertion_function(std::function<void(const std::string&, int, const char*, const char*)> func)
	{
		this->_debug_assertion_func = std::move(func);
	}

	inline void set_warning_assertion_function(std::function<void(const std::string&, int, const char*, const char*)> func)
	{
		this->_warning_assertion_func = std::move(func);
	}

	inline void set_trace_assertion_function(std::function<void(const std::string&, int, const char*, const char*)> func)
	{
		this->_trace_assertion_func = std::move(func);
	}

	inline void set_print_assertion_function(std::function<void(const std::string&, Color, char)> func)
	{
		this->_print_assertion_func = std::move(func);
	}

private:
	std::function<void(const std::string&, int, const char*, const char*)> _info_assertion_func = nullptr;
	std::function<void(const std::string&, int, const char*, const char*)> _error_assertion_func = nullptr;
	std::function<void(const std::string&, int, const char*, const char*)> _debug_assertion_func = nullptr;
	std::function<void(const std::string&, int, const char*, const char*)> _warning_assertion_func = nullptr;
	std::function<void(const std::string&, int, const char*, const char*)> _trace_assertion_func = nullptr;
	std::function<void(const std::string&, Color, char)> _print_assertion_func = nullptr;
};
