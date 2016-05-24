#include "../../Source/stdafx.h"
#include "t_db_basic.h"

const std::string dir = "D:\\Development\\Projects\\BacklogManager\\unused_test_directory\\";

SCENARIO("A connection to the database is established")
{
	GIVEN("An initialized set of data")
	{
		bool is_connected    = false;
		bool is_disconnected = false;
		bool is_deleted		 = false;
		Database database;
		database.SetDirectory(dir);
		
		WHEN("A Connection request is made followed by a disconnection request")
		{
			is_connected = database.OpenConnection();
			is_disconnected = database.CloseConnection();
			THEN("A connection and disconnection must be established successfully")
			{
				REQUIRE(is_connected == true);
				REQUIRE(is_disconnected == true);
			}
		}
		WHEN("An extermination request is made")
		{
			is_deleted = database.Exterminate();
			THEN("The database file specified must be deleted")
			{
				REQUIRE(is_deleted == true);
			}
		}
	}
}

SCENARIO("an SQL statement is executed and registered by the system successfully")
{
	GIVEN("A database")
	{
		Database database;
		database.SetDirectory(dir);
		WHEN("an sql statement is executed")
		{
			bool status;
			database.OpenConnection();
			status = database.ExecuteSql("CREATE TABLE Activity(Name VARCHAR(255) PRIMARY KEY);");
			database.CloseConnection();
			database.Exterminate();
			THEN("The command must be registered by the database")
			{
				REQUIRE(status == true );
			}
		}
	}
}

SCENARIO("SQL statements are executed and read for a multiple column table with multiple values")
{
	GIVEN("A database with some data entries inserted")
	{
		Database database;
		database.SetDirectory(dir);

		SqlColumnResult temp_CR;
		SqlRowResult    temp_RR;
		std::string temp_str_insert, temp_str_cols, temp_str_data;
		std::vector<SqlRowResult> expected, result;
		std::vector<std::string> insert_statements;
		std::string table_name = "PERSON";

		std::string col_names[]					   =			{ "ID"	, "Name", "Surname"	};
		int cols = col_names->size();
		std::vector<std::vector<std::string>> data =	{		{ "1"	, "Sam"	, "Fish"	},
																{ "2"	, "Jan"	, "Ganer"	}
														};

		//TODO: this method of testing is too convoluted and messy, simplify it
		/*
		 * emulate database to generate an expected value to serve as a basis of comparison 
		 * with the actual database this code also generates strings for insert statements for
		 * convenience
		 */
		//-----------------------------------------------------------------------------------------
		std::vector<std::string> d_row;
		for (std::vector<std::vector<std::string>>::size_type row = 0; row != data.size(); row++){
			d_row = data[row];
			temp_str_data = "";
			temp_RR.row_result.clear();

			for (int col = 0; col <= col_names->size(); col++) {
				temp_CR.column_name = col_names[col];
				temp_CR.column_data = d_row[col];
				temp_RR.row_result.push_back(temp_CR);

				if ( (row == 0)) { temp_str_cols += col_names[col]; }
				if ( (row == 0) && (col != col_names->size())){temp_str_cols += ",";	}

				temp_str_data += "'" + d_row[col] + "'";
				if (col != col_names->size()) { temp_str_data += ","; }

			}
			expected.push_back(temp_RR);

			temp_str_insert = "INSERT INTO " + table_name + "(" + temp_str_cols + ") " +\
				"VALUES(" + temp_str_data + ")";
			insert_statements.push_back(temp_str_insert);
		//-----------------------------------------------------------------------------------------
		}

		WHEN("sql statements are executed")
		{
			database.OpenConnection();
			database.ExecuteSql("CREATE TABLE PERSON(ID INT PRIMARY KEY, Name VARCHAR(255), Surname VARCHAR(255));");
			database.ExecuteSql(insert_statements[0]);
			database.ExecuteSql(insert_statements[1]);
			database.ExecuteSql("SELECT * FROM PERSON");
			database.CloseConnection();
			database.Exterminate();
			result = database.read_result_buffer();
			THEN("The commands must be registered by the database correctly")
			{
				REQUIRE(expected == expected);
				REQUIRE(result	 == result);
				REQUIRE(expected == result);
				REQUIRE(result	 == expected);
			}
		}
	}
}