#pragma once

#include <xalwart.orm/interfaces.h>


class SQLQueryBuilderMock : public xw::orm::ISQLQueryBuilder
{
public:
	[[nodiscard]]
	std::string sql_insert(
		const std::string& table_name, const std::string& columns, const std::list<std::string>& rows
	) const override
	{
		return "";
	}

	[[nodiscard]]
	std::string sql_select_(
		const std::string& table_name,
		const std::string& columns,
		bool distinct,
		const std::list<xw::orm::q::Join>& joins,
		const xw::orm::q::Condition& where_cond,
		const std::list<xw::orm::q::Ordering>& order_by_cols,
		long int limit,
		long int offset,
		const std::list<std::string>& group_by_cols,
		const xw::orm::q::Condition& having_cond
	) const override
	{
		return "";
	}

	[[nodiscard]]
	std::string sql_select(
		const std::string& table_name,
		const std::list<std::string>& columns,
		bool distinct,
		const std::list<xw::orm::q::Join>& joins,
		const xw::orm::q::Condition& where_cond,
		const std::list<xw::orm::q::Ordering>& order_by_cols,
		long int limit,
		long int offset,
		const std::list<std::string>& group_by_cols,
		const xw::orm::q::Condition& having_cond
	) const override
	{

		return "";
	}

	[[nodiscard]]
	std::string sql_update(
		const std::string& table_name, const std::string& columns_data, const xw::orm::q::Condition& condition
	) const override
	{
		return "";
	}

	[[nodiscard]]
	std::string sql_delete(const std::string& table_name, const xw::orm::q::Condition& where_cond) const override
	{
		return "";
	}
};

class SQLSchemaEditorMock : public xw::orm::db::ISchemaEditor
{
protected:
	[[nodiscard]]
	std::string quote_name(const std::string& s) const override
	{
		return "";
	}

public:
	void create_table(const xw::orm::db::TableState& table) const override
	{
	}

	void drop_table(const std::string& name) const override
	{
	}

	void rename_table(
		const xw::orm::db::TableState& table, const std::string& old_name, const std::string& new_name
	) const override
	{
	}

	void create_column(const xw::orm::db::TableState& table, const xw::orm::db::ColumnState& column) const override
	{
	}

	void drop_column(const xw::orm::db::TableState& table, const xw::orm::db::ColumnState& column) const override
	{
	}

	void alter_column(
		const xw::orm::db::TableState& table, const xw::orm::db::ColumnState& from_column, const xw::orm::db::ColumnState& to_column, bool strict
	) const override
	{
	}
};

class DatabaseConnectionMock : public xw::orm::IDatabaseConnection
{
public:
	[[nodiscard]]
	std::string dbms_name() const override
	{
		return "";
	}

	void run_query(
		const std::string& sql_query,
		const std::function<void(const std::map<std::string, char*>& /* columns */)>& map_handler,
		const std::function<void(const std::vector<char*>& /* columns */)>& vector_handler
	) const override
	{
		if (map_handler)
		{
			for (const auto& row_data : this->_map_rows)
			{
				map_handler(row_data);
			}
		}

		if (vector_handler)
		{
			for (const auto& vector_row : this->_vector_rows)
			{
				vector_handler(vector_row);
			}
		}
	}

	void run_query(const std::string& sql_query, std::string& last_row_id) const override
	{
		last_row_id = this->_last_row_id;
	}

	void begin_transaction() const override
	{
	}

	void end_transaction() const override
	{
	}

	void rollback_transaction() const override
	{
	}

	void set_last_row_id(std::string last_row_id)
	{
		this->_last_row_id = std::move(last_row_id);
	}

	void set_map_rows_data(std::vector<std::map<std::string, char*>> data)
	{
		this->_map_rows = std::move(data);
	}

	void set_vector_rows_data(std::vector<std::vector<char*>> data)
	{
		this->_vector_rows = std::move(data);
	}

private:
	std::string _last_row_id;
	std::vector<std::map<std::string, char*>> _map_rows;
	std::vector<std::vector<char*>> _vector_rows;
};

class SQLBackendMock : public xw::orm::ISQLBackend
{
public:
	SQLBackendMock()
	{
		this->_schema_editor = std::make_shared<SQLSchemaEditorMock>();
		this->_query_builder = std::make_shared<SQLQueryBuilderMock>();
	}

	void create_pool() override
	{
	}

	std::shared_ptr<xw::orm::IDatabaseConnection> get_connection() override
	{
		return this->_connection;
	}

	void release_connection(const std::shared_ptr<xw::orm::IDatabaseConnection>& connection) override
	{
	}

	[[nodiscard]]
	std::string dbms_name() const override
	{
		return "";
	}

	[[nodiscard]]
	std::vector<std::string> get_table_names() override
	{
		return {};
	}

	xw::orm::ConnectionWrapper wrap_connection() override
	{
		return xw::orm::ConnectionWrapper(this);
	}

	// Returns SQL schema editor related to driver.
	[[nodiscard]]
	xw::orm::db::ISchemaEditor* schema_editor() const override
	{
		return this->_schema_editor.get();
	}

	// Returns SQL query builder related to driver.
	[[nodiscard]]
	xw::orm::ISQLQueryBuilder* sql_builder() const override
	{
		return this->_query_builder.get();
	}

	void set_connection(std::shared_ptr<xw::orm::IDatabaseConnection> connection)
	{
		this->_connection = std::move(connection);
	}

private:
	std::shared_ptr<SQLSchemaEditorMock> _schema_editor = nullptr;
	std::shared_ptr<SQLQueryBuilderMock> _query_builder = nullptr;
	std::shared_ptr<xw::orm::IDatabaseConnection> _connection = nullptr;
};
